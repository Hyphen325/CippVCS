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

int ls_tree_cmd(string tree, vector<FlagBase*> flags){
    if(tree.empty()) cout << "Must provide a tree" << endl; return 1;

    vector<string> flag_values;
    bool recursive = false;
    // TODO: REFORMAT FOR USE WITH MANY FLAGS
    for(auto flag: flags){
        // -r flag
        if(dynamic_cast<Flag*>(flag)){
            Flag* f = dynamic_cast<Flag*>(flag);
            if(f->Get()){
                recursive = true;
            }
        }
    }
    

    CippRepository repo = CippRepository::repo_find();

    ls_tree(repo, tree, recursive);
}

int checkout_cmd(string commit, string path){
    //checks if the provided arguments are empty
    if(commit.empty() || path.empty()){
        cout << "Must provide a commit and path" << endl; return 1;
    }

    //finds the repository in the local directory
    CippRepository repo = CippRepository::repo_find();

    //finds the object and instantiates it as a generic object type.
    CippObject* obj = CippObject::object_read(repo, CippObject::object_find(repo, commit));

    //if the object is a commit
    if(dynamic_cast<CippCommit*>(obj)){
        //gets the tree data from the kvlm inside the commit object
        raw_t tree = (dynamic_cast<CippCommit*>(obj)->commit_data[raw_t(tree.begin(), tree.end())]);

        //read the object to have the tree data (CHECK IF tree raw data is correct)
        obj = CippObject::object_read(repo, string(tree.begin(), tree.end()));
    }

    if(filesystem::exists(path)){
        if(!filesystem::is_directory(path)){
            throw runtime_error("Not a directory");
        }if(filesystem::is_empty(path)){
            throw runtime_error("Not empty");
        }
    }else{
        filesystem::create_directories(path);
    }

    tree_checkout(repo, obj, path);
    
}


/** Helper function to call recursively. TODO: Refactor to not use so many dynamic casts
* My entire polymorphism structure needs to be redone as it is very very messy
*/
void tree_checkout(CippRepository repo, CippObject* obj, filesystem::path path){
    /*Converts the object to a tree*/
    CippTree* tree = dynamic_cast<CippTree*>(obj);
    if(!tree){
        throw new runtime_error("Not a tree object");
    }

    for(auto leaf : tree->tree_data){
        /*Creates an object from the sha in the leaf*/
        CippObject* leaf_obj = CippObject::object_read(repo, leaf.sha);
        
        //Creates the path of the associated new object
        filesystem::path obj_path = path / leaf.path;

        //if the object is another tree, call the function again
        if(dynamic_cast<CippTree*>(leaf_obj)){
            filesystem::create_directories(obj_path);
            tree_checkout(repo, leaf_obj, obj_path);
        }
        /**assumes the object is a blob instead. The casts are so bad here
        * what is even the main differences between dynamic and static casts. Idk
        * I think one is slower than the other. I just know I gotta do them
        * TODO: Fix everything
        */
        else if(dynamic_cast<CippBlob*>(leaf_obj)){
            ofstream output(path, ios::binary);
            auto blob = static_cast<CippBlob*>(leaf_obj)->blob_data;
            output.write((char*)&*blob.begin(), blob.size());
        }

    }
}

void ls_tree(CippRepository repo, string tree, bool recursive = false, filesystem::path path){
    string sha = CippObject::object_find(repo, tree);
    CippTree *obj = static_cast<CippTree*>(CippObject::object_read(repo, sha));

    for(auto item : obj->tree_data){
        uint64_t type;
        if(item.mode&0xFFFFF){
            type = item.mode&0xFF;
        }else{
            type = item.mode&0xF;
        }
        CippObjectType obj_type;
        switch(type){
            case 0x04 : obj_type = TREE; break;
            case 0x10 : obj_type = BLOB; break;
            case 0x12 : obj_type = BLOB; break;
            case 0x16 : obj_type = COMMIT; break;
            default : throw runtime_error("Weird tree leaf mode" + item.mode); break;
        }

        if(recursive && obj_type == TREE) {
            ls_tree(repo, item.sha, recursive, path / item.path);
        }else{
            cout << (item.mode) <<"\t"<<  obj_type <<"\t"<<  item.sha<<"\t" << path/ item.path << endl;
        }
    }
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






