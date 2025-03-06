#include "CippCmd.h"
#include "CippObject/CippObject.h"
#include "CippObject/CippCommit.h"
#include "CippObject/CippBlob.h"

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
        CippObject* blob = CippBlob::object_read(repo, CippBlob::object_find(repo, object));
        vector<uint8_t> blob_text = blob->serialize();
        cout << "Blob" <<  string(blob_text.begin(), blob_text.end()) << endl;
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
    cout << "Object: " << object << endl << "Type: " <<type<<endl;
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

int log_cmd(string commit){
    CippRepository repo = CippRepository::repo_find();

    cout << "digraph cipplog{" << endl << "\t node[shape=rect]" << endl;
    log_graphviz(repo, CippObject::object_find(repo, commit), set<string>());
    cout << "}" << endl;

}

void log_graphviz(CippRepository repo, string sha, set<string> seen = set<string>()){
    if(seen.find(sha) != seen.end()){
        return;
    }
    seen.insert(sha);

    cout << "Reading" << endl;;
    auto commit = dynamic_cast<CippCommit*>(CippObject::object_read(repo, sha));
    if (commit == nullptr) {
        cerr << "Error: Commit object is null" << endl;
        return;
    }    vector<uint8_t> message = commit->serialize();
    //Can be combined into one check for more efficient code
    for(auto it = message.begin(); it != message.end();it++){
        it = find(it, message.end(), 92);
        message.insert(it, 92);
    }

    for(auto it = message.begin(); it != message.end();it++){
        it = find(it, message.end(), '\"');
        message.insert(it, 92);   
    }

    vector<uint8_t>::iterator it = find(message.begin(), message.end(), '\n');
    if(it != message.end()){
        message.erase(it, message.end());
    }

    cout << "  c_" << sha << " [label=\"" << sha.substr(0,7) << ": " << string(message.begin(), message.end())
    << "\"]" << endl;

    if (commit->commit_data.find(vector<uint8_t>('parent')) == commit->commit_data.end()) return;

    vector<uint8_t> parents = commit->commit_data[vector<uint8_t>('parent')];

    for(auto p : parents){
        cout << "  c_" << sha << " -> c_" << (char)p << ";" << endl;
        log_graphviz(repo, string(1, (char)p), seen);
    }


}






