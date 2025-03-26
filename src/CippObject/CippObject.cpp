#include "CippObject.h"
using namespace std;

/*Object Class*/
/*---------------------------------------------------------------------------------*/

vector<unsigned char> CippObject::serialize() {
    
    return vector<unsigned char>();
}

void CippObject::deserialize(vector<unsigned char> data) {
}

CippObject* CippObject::object_read(CippRepository& repo, string sha){
    filesystem::path path = repo.repo_path("objects")/sha.substr(0,2)/sha.substr(2);
    if(!filesystem::exists(path)){
        throw runtime_error("Object does not exist");
    }
    ifstream input(path, ios::binary);
    if(!input){
        throw runtime_error("Failed to open object file");
    }

    input.seekg(0, ios::end);
    unsigned long size = input.tellg();
    input.seekg(0, ios::beg);

    vector<unsigned char> buffer(size);
    input.read(reinterpret_cast<char*> (buffer.data()), size);
    input.close();

    unsigned long uncompressed_size = 4 * size;
    vector<unsigned char> data(uncompressed_size);

    int result;
    while((result = uncompress(reinterpret_cast<Bytef*> (data.data()), &uncompressed_size, buffer.data(), size)) == Z_BUF_ERROR){
        uncompressed_size *= 2;
        data.resize(uncompressed_size);
    }

    if(result != Z_OK){
        throw runtime_error("Failed to decompress object");
    }

    string raw_str(data.begin(), data.begin() + uncompressed_size);

    // Extract object type
    cout << "Raw string" << raw_str << endl;
    size_t space_pos = raw_str.find(' ');
    if (space_pos == string::npos) throw runtime_error("Malformed object: missing space");
    string fmt = raw_str.substr(0, space_pos);

    // Extract size
    size_t null_pos = raw_str.find('\0', space_pos);
    //if (null_pos == string::npos) throw runtime_error("Malformed object: missing null terminator");
    int size2 = stoi(raw_str.substr(space_pos + 1, null_pos - space_pos - 1));

    if (size2 != static_cast<int>(raw_str.size() - null_pos - 1)) {
        //throw runtime_error("Malformed object: bad length");
    }

    // Pick the appropriate constructor
    string content = raw_str.substr(null_pos + 1);
    
    if (fmt == "commit") return new CippCommit(vector<uint8_t>(content.begin(), content.end()));
    if (fmt == "tree")   return new CippTree(content);
    if (fmt == "tag")    return new CippTag(content);
    if (fmt == "blob")   return new CippBlob(vector<uint8_t>(content.begin(), content.end()));

    throw runtime_error("Unknown object type: " + fmt);

}
/** This function writes a an object file using the sha-1 has function. Objects are written into
 * the objects folder in the .git directory   TODO: Document this. I do not remember what I wrote or why I wrote it now
 * 
 */
string CippObject::object_write(CippRepository& repo, CippObject obj, raw_t data, bool write_enable){
    unsigned long size = data.size();
    unsigned long compressed_size = compressBound(size);
    
    vector<unsigned char> buffer(compressed_size);
    if(compress(reinterpret_cast<Bytef*> (buffer.data()), &compressed_size, reinterpret_cast<Bytef*> (data.data()), size) != Z_OK){
        throw runtime_error("Failed to compress object");
    }

    string sha = CippObject::sha1(string(data.begin(), data.end()));
    if(!write_enable){
        return sha;
    }
    filesystem::path path = repo.repo_path("objects")/sha.substr(0,2)/sha.substr(2);
    if(filesystem::exists(path)){
        cout << "Object already exists" << endl;
        return sha;
    }

    if(!filesystem::exists(path.parent_path())){
        filesystem::create_directories(path.parent_path());
    }

    ofstream output(path, ios::binary);
    output.write(reinterpret_cast<char*> (buffer.data()), compressed_size);
    output.close();

    return sha;
}

string CippObject::object_find(CippRepository& repo, string name, CippObjectType type = BLOB){
    return name;
}

string CippObject::sha1(const string& input) {
    unsigned char hash[SHA_DIGEST_LENGTH];  
    SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

    stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

string CippObject::object_hash(filesystem::path input, CippObjectType obj_type, CippRepository& repo, bool write_enable){
    ifstream file(input, ios::binary);
    if(!file){
        throw runtime_error("Failed to open file");
    }


    file.seekg(0, ios::end);
    unsigned long size = file.tellg();
    file.seekg(0, ios::beg);

    vector<unsigned char> buffer(size);
    file.read(reinterpret_cast<char*> (buffer.data()), size);
    file.close();
    
    string sha;
    
    //TODO: Implement other object types, uses default of blob right now
    cout << "type: " << obj_type << endl;
    if(obj_type != BLOB){
        throw runtime_error("Object type not implemented");
    }
    CippBlob obj = CippBlob(reinterpret_cast<const char*>(string(buffer.begin(), buffer.end()).c_str()), size);
    vector<unsigned char> blob_data = obj.serialize();
    
    
    sha = CippObject::object_write(repo, obj, blob_data, write_enable);

    
    return sha;
}

/**Resolves the name to an object hash in the repo
 * This function is aware of:
 * - The HEAD literal
 * - the short and long hashes
 * - tags
 * - branches
 * - remote branches
 */
vector<raw_t> CippObject::object_resolve(CippRepository& repo, string& name){
    vector<raw_t> candidates;
    if(name.empty()){
        return candidates;
    }

    //regex to compare against
    std::regex hash_re("^[0-9A-Fa-f]{4,40}$");

    if(name == "HEAD"){
        candidates.push_back(ref_resolve(repo, "HEAD"));
        return candidates;
    }




}

/** This function is recursive: it reads a key/value pair, then call
 * itself back with the new position. We need to know, where we are at,
 * a keyword, or already in the message
 */
map<vector<uint8_t>, vector<uint8_t>> CippObject::kvlm_parse(vector<uint8_t>& data, map<vector<uint8_t>, vector<uint8_t>>& kvlm,  int start){
    
    //finds the next space and the next newline based off start
    //if space is before newline, then we are in the key, otherwise its the final message
    auto it_space = find(data.begin() + start, data.end(), ' ');
    auto it_newln = find(data.begin() + start, data.end(), '\n');

    //base case for if we have reached the end of the commit object/kvlm
    if (it_newln < it_space || it_space == data.end()) {
        //assigns empty vector to map to the rest of the data
        kvlm[vector<uint8_t>()] = vector<uint8_t>(data.begin() + start, data.end());
        return kvlm;
    }

    auto key = vector<uint8_t>(data.begin() + start, it_space);

    auto end = data.begin() + start;
    //finds the end of the pgp signature
    //VERIFY THIS WORKS
    while(true){
        end = find(end, data.end(), '\n');
        if(*(end+1) != ' ') break;
    }

    //copies the values out of the kvlm and trims the extra spaces inside of multi line values
    vector<uint8_t> value = vector<uint8_t>(it_space + 1, end);
    for(auto it = value.begin(); it != value.end(); it++){
        if(it != value.end()-1 && *it == '\n' && *(it+1) == ' '){
            value.erase(it+1);
        }
    }

    if (kvlm.find(key) != kvlm.end()){
        //append the value to the existing key
        kvlm[key].insert(kvlm[key].end(), value.begin(), value.end());
    }else{
        //create a new key
        kvlm[key] = value;
    }

    return kvlm_parse(data, kvlm, end - data.begin() + 1);
}

//turns the map into a sequence of bytes in key value format according to kvlm
vector<uint8_t> CippObject::kvlm_serialize(map<vector<uint8_t>, vector<uint8_t>> kvlm){
    vector<uint8_t> data;
    for(auto it = kvlm.begin(); it != kvlm.end(); it++){
        if(it->first == vector<uint8_t>()) continue;
        data.insert(data.end(), it->first.begin(), it->first.end());
        data.push_back(' ');
        data.insert(data.end(), it->second.begin(), it->second.end());
        data.push_back('\n');
    }
    data.push_back('\n');
    data.insert(data.end(), kvlm[vector<uint8_t>()].begin(), kvlm[vector<uint8_t>()].end());
    return data;
}

//returns the raw data inside the object
vector<uint8_t> CippObject::get(){
    return raw_data;
}

