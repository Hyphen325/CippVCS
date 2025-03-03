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
    Group cat_file_args(cat_file, "flags");
    Positional<string> cat_file_type(cat_file_args, "type", "The type of object to display");
    Positional<string> cat_file_object(cat_file_args, "object", "The object to display");

    /*hash-file command with it's positional arguments*/
    Command hash_object(commands, "hash-object", "Compute object ID and optionally creates a blob from a file");
    Group hash_object_args(hash_object, "flags");
    Group hash_object_flags(hash_object, "flags");
    /*Hash command flags*/
    Flag hash_object_write(hash_object_flags, "w", "Write the object into the database", {'w'});
    ValueFlag<string> hash_object_path(hash_object_flags, "TYPE", "Specify the object type", {'t'});
    Positional<string> hash_object_file(hash_object_args, "file", "The file to hash");

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