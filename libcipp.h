
#ifndef LIBCIPP_H
/*Includes the Library containing the class for initialization*/
#define LIBCIPP_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "dependencies/args.hxx"
#include <filesystem>
#include <fstream>
#include <map>

using namespace std;



class CippRepository {
    public:
        

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

        filesystem::path worktree;
        filesystem::path gitdir;
        unordered_map<string, map<string, string>> conf;

        /*Takes a local path for the repository, and returns the
        system path by appending it to the repo path*/
        filesystem::path repo_file(filesystem::path );

        void readConfig(filesystem::path );

        std::string trim(const std::string& );

        static void write_config(ofstream& , unordered_map<string, unordered_map<string, string>> );

        /*Creates the default configuration map for a new repository*/
        static unordered_map<string, unordered_map<string, string>> default_config();



};

class CippObject {
    public:

        CippObject(char*, int);
        CippObject();

        virtual void serialize(CippRepository);
        
        virtual void deserialize(CippRepository);

    private:
        vector<char*> object_read(CippRepository, char*);

        char* object_write(CippRepository, char*, char*, int);
};



#endif

