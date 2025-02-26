#include "cippCmd.h"

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