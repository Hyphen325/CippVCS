#include "src/libcipp.h"

using namespace std;
using namespace args;



int main(int argc, char **argv){

    /*Arguments for the command line tool*/
    ArgumentParser parser("This is a git clone program.", "This is after the options");

    Group commands(parser, "commands");
    Command init(commands, "init", "initializes the repo");
    Command add(commands, "add", "adds file to be staged");
    Command commit(commands, "commit", "Creates a new commit");

    /*cat-file command with it's potitional arguments*/
    Command cat_file(commands, "cat-file", "Provide content of repository objects");
    Group cat_file_args(cat_file, "args");
    Positional<string> cat_file_type(cat_file_args, "type", "The type of object to display");
    Positional<string> cat_file_object(cat_file_args, "object", "The object to display");

    /*hash-file command with it's positional arguments*/
    Command hash_object(commands, "hash-object", "Compute object ID and optionally creates a blob from a file");
    Group hash_object_args(hash_object, "args");
    Group hash_object_flags(hash_object, "flags");
    Positional<string> hash_object_file(hash_object_args, "file", "The file to hash");

    /*Hash command flags*/
    Flag hash_object_write(hash_object_flags, "w", "Write the object into the database", {'w'});
    ValueFlag<string> hash_object_path(hash_object_flags, "TYPE", "Specify the object type", {'t'});

    /*Log command with it's positional arguments*/
    Command log(commands, "log", "Display history of a given commit");
    Group log_args(log, "args");
    Group log_flags(log, "flags");
    Positional<string> log_commit(log_args, "commit", "The commit to start at");

    /*Log command Flags*/

    /*ls tree command with its arguments*/
    Command ls_tree(commands, "ls-tree", "Pretty-print a tree object");
    Group ls_tree_args(ls_tree, "args");
    Group ls_tree_flags(ls_tree, "flags");
    Positional<string> ls_tree_tree(ls_tree_args, "tree", "A tree-ish object");

    /*ls tree command flags*/
    Flag ls_tree_recursive(ls_tree_flags, "r", "Recurse into sub-trees", {'r'});

    /*Checkout command*/
    Command checkout(commands, "checkout", "Checkout a commit inside of a directory");
    Group checkout_args(checkout, "args");
    Group checkout_flags(checkout, "flags");
    Positional<string> checkout_commit(checkout_args, "commit", "The commit or tree to checkout");
    Positional<string> checkout_path(checkout_args, "path", "The EMPTY directory to checkout on");

    /*The show-ref command*/
    Command show_ref(commands, "show-ref", "List references");

    /*The tag command*/
    Command tag(commands, "tag", "List and create tags");
    Group tag_args(tag, "args");
    Group tag_flags(tag, "flags");
    Positional<string> tag_name(tag_args, "name", "The new tag's name");
    Positional<string> tag_object(tag_args, "object", "The object the new tag will point to");
    Flag tag_create(tag_flags, "a", "Whether to create a tag object", {"a"});

    Command rev_parse(commands, "rev-parse", "Parse revision (or other object) identifiers");
    Group rev_parse_args(rev_parse, "args");
    Group rev_parse_flags(rev_parse, "flags");
    ValueFlag<std::string> rev_parse_type(rev_parse_flags, "type", "", {"cipp-type"});
    Positional<std::string> rev_parse_name(rev_parse_args, "name", "The name to parse");
    

    /*Global arguments*/
    Group arguments(parser, "arguments", Group::Validators::DontCare, Options::Global);
    HelpFlag help(parser, "help", "Display this help menu", {"h", "help"}, Options::Global);
    CompletionFlag completion(parser, {"complete"});
    
    try
    {
        parser.ParseCLI(argc, argv);
        
        if(init){
            filesystem::path local_path =  filesystem::current_path();
            CippRepository::createRepo(local_path);
        }
        if(cat_file){
            cat_file_cmd(cat_file_type.Get(), cat_file_object.Get());
        }
        if(hash_object){
            hash_object_cmd(hash_object_file.Get(), hash_object_flags.GetAllFlags());
        }
        if(log){
            log_cmd(log_commit.Get());
        }
        if(ls_tree){
            ls_tree_cmd(ls_tree_tree.Get(), ls_tree_args.GetAllFlags());  
        }
        if(checkout){
            checkout_cmd(checkout_commit.Get(), checkout_path.Get());
        }
        if(tag){
            tag_cmd(tag_args, tag_flags.GetAllFlags());
        }
        if(rev_parse){
            rev_parse_cmd(rev_parse_args, rev_parse_flags.GetAllFlags());
        }
    }
    
    catch (const args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (const ValidationError& e){
        cout << "Must Provide a Command!" << endl;
        cout << parser;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        
        return 1;
    }
    return 0;
}

