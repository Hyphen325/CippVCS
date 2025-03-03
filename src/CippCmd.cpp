#include "CippCmd.h"

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

int hash_object_cmd(string object, vector<FlagBase*> flags){
    if(object.empty()){
        cout << "Must provide a file" << endl;
        return 1;
    }

    vector<string> flag_values;
    string type = "blob";
    bool write_enable = false;
    // TODO: REFORMAT FOR USE WITH MANY FLAGS
    for(auto flag: flags){
        //-type flag
        if(dynamic_cast<ValueFlag<string>*>(flag)){
            ValueFlag<string>* f = dynamic_cast<ValueFlag<string>*>(flag);
            type = f->Get();
        }
        // -w flag
        if(dynamic_cast<Flag*>(flag)){
            Flag* f = dynamic_cast<Flag*>(flag);
            if(f->Get()){
            write_enable = true;
            }
        }
    }

    //defaults object and type to lowercase
    transform(object.begin(), object.end(), object.begin(), ::tolower);
    transform(type.begin(), type.end(), type.begin(), ::tolower);

    CippObjectType obj_type = object_type_map.at(type);

    CippRepository repo;
    if(write_enable){
        repo = CippRepository::repo_find();
    }
    
    //Looks for file in provided location
    filesystem::path local_path =  filesystem::current_path()/object;
    if(!filesystem::exists(local_path)){
        cout << "File does not exist" << endl;
        return 1;
    }

    //hashes the file at the path
    string sha = CippBlob::object_hash(local_path, obj_type, repo, write_enable);
    cout << "SHA: " << sha << endl;
    

    cout << "Type: " << type << endl;
    cout << "Object: " << object << endl;
    return 0;
}


