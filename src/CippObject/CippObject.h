#include "../libcipp.h"

#ifndef CIPP_OBJECT_H
#define CIPP_OBJECT_H


//type for commit object
using kvlm_t = std::map<std::vector<uint8_t>, std::vector<uint8_t>>;
//type for raw data (string of characters)
using raw_t = std::vector<uint8_t>;

//needed here because it is used on the object files
raw_t ref_resolve(CippRepository, std::filesystem::path);


enum CippObjectType{
    BLOB,
    TREE,
    COMMIT,
    TAG,
    NONE
};

static const std::map<std::string, CippObjectType> object_type_map = {
    {"blob", BLOB},
    {"tree", TREE},
    {"commit", COMMIT},
    {"tag", TAG},
    {"none", NONE}
};

class CippObject {
    public:
        /*String to identify object*/
        /*TODO: Switch this to only operate off the enum type*/
        std::string fmt;

        virtual std::vector<unsigned char> serialize();
        virtual void deserialize(std::vector<unsigned char>);
        std::vector<uint8_t> get();

        static CippObject* object_read(CippRepository&, std::string);
        static string object_write(CippRepository&, CippObject, std::vector<unsigned char>, bool);
        static string object_find(CippRepository&, std::string, CippObjectType = NONE, bool = true);
        static string object_hash(std::filesystem::path, CippObjectType, CippRepository&, bool = false);
        static vector<raw_t> object_resolve(CippRepository&, string&);
        static string sha1(const string& );      

        static map<vector<uint8_t>, vector<uint8_t>> kvlm_parse(vector<uint8_t>&, map<vector<uint8_t>, vector<uint8_t>>&,  int start=0 );
        static vector<uint8_t> kvlm_serialize(map<vector<uint8_t>, vector<uint8_t>>);

    private:
        vector<uint8_t> raw_data;

};

#include "CippBlob.h"
#include "CippTag.h"
#include "CippCommit.h"
#include "CippTree.h"
#include "CippIndex.h"

#include "CippObject.cpp"




#endif 

