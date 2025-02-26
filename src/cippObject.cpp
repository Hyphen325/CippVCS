#include "cippObject.h"
using namespace std;

/*Object Class*/
/*---------------------------------------------------------------------------------*/

vector<unsigned char> CippObject::serialize() {
    
    return vector<unsigned char>();
}

void CippObject::deserialize(vector<unsigned char> data) {
}

vector<unsigned char> CippObject::object_read(CippRepository repo, string sha){
    filesystem::path path = repo.repo_file("objects")/sha.substr(0,2)/sha.substr(2);
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
    while(result = uncompress(reinterpret_cast<Bytef*> (data.data()), &uncompressed_size, buffer.data(), size) == Z_BUF_ERROR){
        uncompressed_size *= 2;
        data.resize(uncompressed_size);
    }

    if(result != Z_OK){
        throw runtime_error("Failed to decompress object");
    }

    data.resize(uncompressed_size);
    return data;

}

string CippObject::object_write(CippRepository repo, CippObject obj, vector<unsigned char> data){
    unsigned long size = data.size();
    unsigned long compressed_size = compressBound(size);
    
    vector<unsigned char> buffer(compressed_size);
    if(compress(reinterpret_cast<Bytef*> (buffer.data()), &compressed_size, reinterpret_cast<Bytef*> (data.data()), size) != Z_OK){
        throw runtime_error("Failed to compress object");
    }

    string sha = CippObject::sha1(string(data.begin(), data.end()));

    filesystem::path path = repo.repo_file("objects")/sha.substr(0,2)/sha.substr(2);
    if(filesystem::exists(path)){
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

string CippObject::object_find(CippRepository repo, string name){
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

string CippObject::object_hash(filesystem::path input, string type, CippRepository repo){
    ifstream file(input, ios::binary);
    if(!input){
        throw runtime_error("Failed to open file");
    }

    file.seekg(0, ios::end);
    unsigned long size = file.tellg();
    file.seekg(0, ios::beg);

    vector<unsigned char> buffer(size);
    file.read(reinterpret_cast<char*> (buffer.data()), size);
    file.close();

    CippBlob blob = CippBlob(reinterpret_cast<char*>(string(buffer.front(), buffer.end()).c_str()), size);
    vector<unsigned char> blob_data = blob.serialize();
    string sha = CippBlob::object_write(repo, blob, blob_data);
    return sha;
}





/*Blob Class*/
/*---------------------------------------------------------------------------------*/
CippBlob::CippBlob(char* data, int size){
    blob_data = vector<unsigned char>(data, data + size);
}

CippBlob::CippBlob(){
    blob_data = vector<unsigned char>();
}

vector<unsigned char> CippBlob::serialize(){
    return blob_data;
}

void CippBlob::deserialize(vector<unsigned char> data){
    blob_data = data;
}







