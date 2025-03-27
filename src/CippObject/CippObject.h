#include "../libcipp.h"

#ifndef CIPP_OBJECT_H
#define CIPP_OBJECT_H


using namespace std;
//type for commit object
using kvlm_t = map<vector<uint8_t>, vector<uint8_t>>;
//type for raw data (string of characters)
using raw_t = vector<uint8_t>;

//needed here because it is used on the object files
raw_t ref_resolve(CippRepository, filesystem::path);


enum CippObjectType{
    BLOB,
    TREE,
    COMMIT,
    TAG,
    NONE
};

static const map<string, CippObjectType> object_type_map = {
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
        string fmt;

        virtual vector<unsigned char> serialize();
        virtual void deserialize(vector<unsigned char>);
        vector<uint8_t> get();

        static CippObject* object_read(CippRepository&, string);
        static string object_write(CippRepository&, CippObject, vector<unsigned char>, bool);
        static string object_find(CippRepository&, string, CippObjectType = NONE, bool = true);
        static string object_hash(filesystem::path, CippObjectType, CippRepository&, bool = false);
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

#include "CippObject.cpp"




#endif 

