#include "src/libcipp.h"

using namespace std;
using namespace args;



int main(int argc, char **argv){
    
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

