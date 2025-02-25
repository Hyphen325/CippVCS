#ifndef LIBCIPP_H
#include <iostream>
#include <cstdlib>
#include <string>
#include <args.hxx>
#include <filesystem>
#include <fstream>
#include <map>
/*Includes the Library containing the class for initialization*/
#define LIBCIPP_H

class CippRepository {
    public:
        filesystem::path worktree;
        filesystem::path gitdir;
        unordered_map<string, map<string, string>> conf;
        static CippRepository repo_find(filesystem::path path=".", bool required=true);

        static filesystem::path repo_path(CippRepository, filesystem::path);
        static filesystem::path  repo_dir(filesystem::path, filesystem::path, bool mkdir=false);

        static CippRepository createRepo(filesystem::path path);


        CippRepository(filesystem::path path, bool force = false);

    private:
    
        /*Takes a local path for the repository, and returns the
        system path by appending it to the repo path*/
        filesystem::path repo_file(filesystem::path local_path);

        void readConfig(filesystem::path configPath);

        std::string trim(const std::string& str);

        static void write_config(ofstream& writer, unordered_map<string, unordered_map<string, string>> config);

        /*Creates the default configuration map for a new repository*/
        static unordered_map<string, unordered_map<string, string>> default_config();



};

class CippObject {
    public:

        CippObject();

};



#endif

