
#include "libcipp.h"


using namespace std;
using namespace args;

int main(int argc, char **argv){
    ArgumentParser parser("This is a git clone program.", "This is after the options");

    Group commands(parser, "commands");
    Command init(commands, "init", "initializes the repo");
    Command add(commands, "add", "adds file to be staged");
    Command commit(commands, "commit", "Creates a new commit");
    

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
        if()
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

