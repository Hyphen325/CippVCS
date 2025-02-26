#include <unordered_map>
#include <string>
#include <fstream>
#include <filesystem>
#ifndef CIPP_REPO_H
/*Includes the Library containing the class for initialization*/
#define CIPP_REPO_H

using namespace std;



class CippRepository {
    public:

        filesystem::path worktree;
        filesystem::path gitdir;
        unordered_map<string, unordered_map<string, string>> conf;

        filesystem::path repo_file(filesystem::path );
        /*Finds the repository in the specified path, and returns the repository object*/
        static CippRepository repo_find(filesystem::path, bool);
        /*Returns the path of the .git folder in the desired repo*/
        static filesystem::path repo_path(CippRepository, filesystem::path);
        /*Returns an error if the specified path is not a directory, and creates a directory at
        said location if specified by the parameters*/
        static filesystem::path  repo_dir(filesystem::path, filesystem::path, bool);
        /*Creates a new repository at the specified path*/
        static CippRepository createRepo(filesystem::path path);


        CippRepository(filesystem::path, bool force = false);

    private:

        /*Takes a local path for the repository, and returns the
        system path by appending it to the repo path*/

        void readConfig(filesystem::path );

        std::string trim(const std::string& );

        static void write_config(ofstream& , unordered_map<string, unordered_map<string, string>> );

        /*Creates the default configuration map for a new repository*/
        static unordered_map<string, unordered_map<string, string>> default_config();



};



#include "cippRepo.cpp"

#endif

