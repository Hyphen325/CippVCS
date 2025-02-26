#include "cippObject.h"

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

string CippObject::sha1(const string& input) {
    unsigned char hash[SHA_DIGEST_LENGTH];  
    SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

    
    stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}





