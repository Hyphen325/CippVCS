#include "CippObject.h"


#ifndef CIPPINDEX_H
#define CIPPINDEX_H

enum ObjectModeType{
    REGULAR = 0b1000,
    SYMLINK = 0b1010,
    GITLINK = 0b1110
};

class CippIndexEntry{
    public:
        //unsure on implementation of flag variables
        CippIndexEntry(std::time_t ctime = NULL, std::time_t mtime = NULL, uint64_t dev = NULL, uint64_t ino = NULL,
            ObjectModeType mode_type = ObjectModeType(NULL), uint32_t mode_perms = NULL, uint32_t uid = NULL, uint32_t gid = NULL,
            uint64_t file_size = NULL, std::string sha = NULL, bool flag_assume_valid = NULL, uint32_t flag_stage = NULL,
            string name = NULL 
        );
        

        
        //The last time a files metadata changed (this is a pair)
        std::time_t ctime;
        std::time_t mtime;
        //the id of device containing the file
        uint64_t dev;
        //the files inode number
        uint64_t ino;
        //the object type (either regular, symlink, or gitlink)
        ObjectModeType mode_type;
        //the object permissions, an integer
        uint32_t mode_perms;
        //the user id of the owner
        uint32_t uid;
        //the group id of the owner
        uint32_t gid;
        //the size of the object in bytes
        uint64_t file_size;
        //the objects sha
        std::string sha;
        bool flag_assume_valid;
        uint32_t flag_stage;

        //the name of the object including the full path
        string name;
};

class CippIndex{
    public:
        CippIndex(uint32_t version = 2, std::vector<CippIndexEntry> entries = std::vector<CippIndexEntry>());
        
        uint32_t version;
        std::vector<CippIndexEntry> entries;

        //this function is crazy
        static CippIndex index_read(CippRepository);

          
};

#include "CippIndex.cpp"

#endif // CIPPBLOB_H