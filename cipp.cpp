#include "src/libcipp.h"

using namespace std;
using namespace args;


int main(int argc, char **argv){
    ArgumentParser parser("This is a git clone program.", "This is after the options");

    Group commands(parser, "commands");
    Command init(commands, "init", "initializes the repo");
    Command add(commands, "add", "adds file to be staged");
    Command commit(commands, "commit", "Creates a new commit");

    /*cat-file command with it's potitional arguments*/
    Command cat_file(commands, "cat-file", "Provide content of repository objects");
    Group cat_file_flags(cat_file, "flags");
    Positional<string> cat_file_type(cat_file_flags, "type", "The type of object to display");
    Positional<string> cat_file_object(cat_file_flags, "object", "The object to display");
    

    Group arguments(parser, "arguments", Group::Validators::DontCare, Options::Global);
    HelpFlag help(parser, "help", "Display this help menu", {"h","help"});

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
        std::cerr << parser;
        return 1;
    }
    return 0;
}

