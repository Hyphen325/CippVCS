#include "cippCmd.h"

using namespace args;

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

int hash_object_cmd(string type, string object, vector<FlagBase*> flags){
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
        CippBlob blob = CippBlob((char*)object.c_str(), object.size());
        vector<unsigned char> blob_data = blob.serialize();
        string sha = CippBlob::object_write(repo, blob, blob_data);
        cout << "SHA: " << sha << endl;
    }

    cout << "Type: " << type << endl;
    cout << "Object: " << object << endl;
    return 0;
}


