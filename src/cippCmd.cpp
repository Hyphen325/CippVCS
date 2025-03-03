#include "cippCmd.h"

using namespace args;

extern ArgumentParser parser;

extern Flag hash_object_write;
extern ValueFlag<string> hash_object_path;

int cat_file_cmd(string type, string object){
    if(type.empty() || object.empty()){
        cout << "Must provide a type and object" << endl;
        return 1;
    }

    transform(object.begin(), object.end(), object.begin(), ::tolower);
    transform(type.begin(), type.end(), type.begin(), ::tolower);
    

    cout << "Finding Repository" << endl;
    CippRepository repo = CippRepository::repo_find();
    cout << "Examining Characters" << endl;
    if(object == "blob"){
        vector<unsigned char> blob = CippBlob::object_read(repo, CippBlob::object_find(repo, object));
        cout << "Blob" <<  string(blob.begin(), blob.end()) << endl;
    }

    cout << "Type: " << type << endl;
    cout << "Object: " << object << endl;
    return 0;
}

int hash_object_cmd(string object, vector<FlagBase*> flags){
    if(object.empty()){
        cout << "Must provide a file" << endl;
        return 1;
    }

    vector<string> flag_values;
    
    for(auto flag: flags){
        cout << flag->Name() << endl;
        cout << "Value" << "" << endl;
        flag_values.clear();
    }

    string type = "blob";

    transform(object.begin(), object.end(), object.begin(), ::tolower);
    transform(type.begin(), type.end(), type.begin(), ::tolower);

    cout << "Finding Repository" << endl;
    CippRepository repo = CippRepository::repo_find();
    cout << "Examining Characters" << endl;
    
    filesystem::path local_path =  filesystem::current_path()/object;
    if(!filesystem::exists(local_path)){
        cout << "File does not exist" << endl;
        return 1;
    }
    string sha = CippBlob::object_hash(local_path, type, repo);
    cout << "SHA: " << sha << endl;
    

    cout << "Type: " << type << endl;
    cout << "Object: " << object << endl;
    return 0;
}


