#include "CippCmd.h"
#include "CippObject/CippObject.h"
#include "CippObject/CippCommit.h"
#include "CippObject/CippBlob.h"
#include <any>
#include <filesystem>
#include <iostream>

using namespace args;
using namespace std;


int cat_file_cmd(string type, string object){
    if(type.empty() || object.empty()){
        std::cout << "Must provide a type and object" << endl;
        return 1;
    }

    transform(object.begin(), object.end(), object.begin(), ::tolower);
    transform(type.begin(), type.end(), type.begin(), ::tolower);
    

    std::cout << "Finding Repository" << endl;
    CippRepository repo = CippRepository::repo_find();
    std::cout << "Examining Characters" << endl;
    if(object == "blob"){
        CippObject* blob = CippBlob::object_read(repo, CippBlob::object_find(repo, object));
        vector<uint8_t> blob_text = blob->serialize();
        std::cout << "Blob" <<  string(blob_text.begin(), blob_text.end()) << endl;
    }

    std::cout << "Type: " << type << endl;
    std::cout << "Object: " << object << endl;
    return 0;
}

int hash_object_cmd(string object, vector<FlagBase*> flags){
    if(object.empty()){
        std::cout << "Must provide a file" << endl;
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
            type = f->Get().empty() ? "blob" : f->Get();
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
    std::cout << "Object: " << object << endl << "Type: " <<type<<endl;
    transform(object.begin(), object.end(), object.begin(), ::tolower);
    transform(type.begin(), type.end(), type.begin(), ::tolower);

    CippObjectType obj_type = object_type_map.at(type);

    CippRepository repo;
    if(write_enable){
        repo = CippRepository::repo_find();
    }
    
    //Looks for file in provided location
    std::filesystem::path local_path =  filesystem::current_path()/object;
    if(!filesystem::exists(local_path)){
        std::cout << "File does not exist" << endl;
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

    if(commit.empty()){
        cout << "Missing valid commit to view" << endl;
        return 1;
    }
    cout << "digraph cipplog{" << endl << "\t node[shape=rect]" << endl;
    log_graphviz(repo, CippObject::object_find(repo, commit), set<string>());
    cout << "}" << endl;

}

int ls_tree_cmd(string tree, vector<FlagBase*> flags){
    if(tree.empty()){ cout << "Must provide a tree" << endl; return 1;}

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
    if(commit.empty()){
        cout << "Must provide a commit" << endl; return 1;
    }
    if(path.empty()){
        path = ".";
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

int show_refs_cmd(){
    CippRepository repo = CippRepository::repo_find();
    
    //gather the refs to show
    auto refs = ref_list(repo, "");

    //display references
    show_ref(repo, refs, "refs");

    return 0;
}

/**
 * The tag command
 */
int tag_cmd(Group& arguments, vector<FlagBase*> flags){

    CippRepository repo = CippRepository::repo_find();
    //gets all arguments
    auto args = arguments.GetAllPositional();

    string name = "";
    string object = "";
    bool create = false; 

    for(PositionalBase* pos : args){
        //ALL FLAGS I WRITE FOR NOW START AS STRINGS. THIS WILL CHANGE IF 
        //THE TYPE INPUTTED TO THE FLAG COMMAND IS NOT A STRING (IDK WHEN THAT HAPPENS)
        Positional<string>* flag = static_cast<Positional<string>*>(pos);
        if(!flag->Get().empty()) 
        {
            //TODO: Change to have some better way of checking arguments than just comparing strings
            //This is nasty. Maybe compare to reference of argument in group?
            if(flag->Name() == "name"){
                cout << "Name: " << flag->Get() << endl;
                name = flag->Get();
            }

            else if(flag->Name() == "object"){
                cout << "Object: " << flag->Get() << endl;
            }
        }
    }
    //I take it all back I love this argument parsing tool. It's awesome
    
    if(!name.empty()){
        tag_create(repo, name, object, create); //
    }else{
        auto refs = ref_list(repo, ".");
        //idk just hope this works, ill make it better soon
        try{
            auto tags = std::any_cast<std::unordered_map<std::filesystem::path, std::any>>(refs["tags"]);
            show_ref(repo, tags, "", false);
        }catch(exception bad_any_cast){
            cout << "Missing any tags" << endl;
        }
        
    }

    return 0;
}


int rev_parse_cmd(args::Group& args, std::vector<args::FlagBase*> flags){
    cout << "Rev Parse" << endl;
    string type = "blob";
    //finds the type of object to search for (default blob)
    for(FlagBase* flag : flags){
        //-type flag
        if(dynamic_cast<ValueFlag<string>*>(flag)){
            ValueFlag<string>* f = dynamic_cast<ValueFlag<string>*>(flag);
            type = f->Get();
        }
    }
    
    //name of the object to search for
    string name;
    for(auto arg : args.GetAllPositional()){
        //checks of off the name of the argument
        if(arg->Name() == "name"){
            if(auto name_arg = dynamic_cast<Positional<string>*>(arg)){
                name = name_arg->Get();
            }
        }
    }
    if(name.empty()){
        cout << "Must provide a name!" << endl;
        return 1;
    }

    CippRepository repo = CippRepository::repo_find();
    cout << CippObject::object_find(repo, name, (object_type_map.find(type))->second, true) << endl;
    return 0;
}



/*Helper functions*/
/*----------------------------------------------------------------------------------------*/

/**Creates a tag object if specified in the command */
void tag_create(CippRepository repo, string name, string object, bool create_tag_object){
    auto sha = CippObject::object_find(repo, name);

    if(create_tag_object){
        CippTag tag = CippTag();

        tag.commit_data = kvlm_t();
        string object = "object";
        string type = "type";
        string tag_s = "tag";
        string tagger = "tagger";
        tag.commit_data[raw_t(object.begin(), object.end())] = raw_t(sha.begin(), sha.end());
        tag.commit_data[raw_t(type.begin(), type.end())] = raw_t('commit');
        tag.commit_data[raw_t(tag_s.begin(), tag_s.end())] = raw_t(name.begin(), name.end());

        tag.commit_data[raw_t(tagger.begin(), tagger.end())] = raw_t('Cipp User');

        auto tag_sha = CippObject::object_write(repo, tag, tag.serialize(), true);

        ref_create(repo, "tags/" + name, tag_sha);
    }else{
        ref_create(repo, "tags/" + name, sha);
    }
}

void ref_create(CippRepository repo, string ref_name, string sha){
    //
    std::ofstream file(repo.repo_file(filesystem::path("refs") / ref_name));
    file.write((char*)&*(sha + '\n').begin(), sha.length()+1);
    file.close();
}

/**Sometimes, an indirect reference may be broken. This is normal behaviour
 * in a specific case: when there is the HEAD of a new repository with no commits.
 * In that case, .git/HEAD points to "refs: refs/heads/main", but .git/refs/heads/main 
 * doesn't exist yet (since theres no vommit for it to refer to)
 */
raw_t ref_resolve(CippRepository repo, filesystem::path path = ""){
    path = repo.repo_file(path);

    if(filesystem::is_directory(path)){
        return raw_t();
    }
    if(!filesystem::exists(path)){
        return raw_t();
    }

    //creates a file reader
    ifstream reader(path);
    reader.seekg(0, ios::end);
    unsigned long size = reader.tellg();
    reader.seekg(0, ios::beg);

    raw_t buffer(size);
    reader.read(reinterpret_cast<char*> (buffer.data()), size);
    reader.close();

    //buffer now contains all data from the file.
    if(buffer[0] == 'r' && buffer[1] == 'e' && buffer[2] == 'f' && buffer[3] ==':' && buffer[4] == ' '){
        return ref_resolve(repo, ".git/" + string(buffer.begin()+5, buffer.end()));
    }else{
        return buffer;
    }
}

/**
 * This function displays the ref. The type of the refs can be either itself or a raw_t
 */
void show_ref(CippRepository repo, std::unordered_map<std::filesystem::path, std::any> refs, filesystem::path path, bool hash){
    if(!path.empty()){
        path= path / "/";
    }

    //taks each value in the map and casts it to handle it in a specific way
    for(auto map : refs){
        try{
            auto value = (any_cast<raw_t>(map.second));
            string ref_str = string(value.begin(), value.end());
            string key_str = map.first.c_str();
            if(hash){
                cout << ref_str << " " << path.c_str() << key_str << endl;
            }else{
                cout << path.c_str() << key_str << endl;
            }
            
        }catch(const bad_any_cast&){
            //calls the function again with the nested map
            if (auto nested_map = std::any_cast<std::unordered_map<std::filesystem::path, std::any>>(&map.second)) {
                show_ref(repo, *nested_map, path / map.first, hash);
            }
        }
    }
}

/**This function is tricky, because it needs to return a map of a element that could either
 * be another map, or just a raw_t type. Possible solutions to this include a custom tree data
 * stucture? Is there anything in the stl that can help here?
 * 
 * Structure ideas: Key -> (Union: Itself, raw_t)??
 * I'll try and use std::any but TODO: Change to use a more safe method of dynamic types
 */
std::unordered_map<std::filesystem::path, std::any> ref_list(CippRepository repo, filesystem::path path = "."){
    if(path == "."){
        path = CippRepository::repo_dir(repo, "refs");
    }
    //placeholder
    unordered_map<filesystem::path, any> ret;
    //kvlm_t ret = kvlm_t();

    for(auto f : filesystem::directory_iterator(path)){
        filesystem::path can = f.path();
        if(f.is_directory()){
            //sets ret[f] to be another map
            ret[can] = ref_list(repo, can);
        }else{
            //sets ret[f] to be a raw_t
            ret[can] = ref_resolve(repo, ".git/" / can);
        }
    }

    return ret;
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
            //this line is horrific
            output.write((char*)&*blob.begin(), blob.size());
            output.close();
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
    //TODO: Can be combined into one check for more efficient code
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






