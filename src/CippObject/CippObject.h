#include "../libcipp.h"

#ifndef CIPP_OBJECT_H
#define CIPP_OBJECT_H


using namespace std;
using kvlm_t = map<vector<uint8_t>, vector<uint8_t>>;

enum CippObjectType{
    BLOB,
    TREE,
    COMMIT,
    TAG
};

static const map<string, CippObjectType> object_type_map = {
    {"blob", BLOB},
    {"tree", TREE},
    {"commit", COMMIT},
    {"tag", TAG}
};

class CippObject {
    public:

        virtual vector<unsigned char> serialize();
        virtual void deserialize(vector<unsigned char>);
        vector<uint8_t> get();

        static CippObject* object_read(CippRepository&, string);
        static string object_write(CippRepository&, CippObject, vector<unsigned char>, bool);
        static string object_find(CippRepository&, string);
        static string object_hash(filesystem::path, CippObjectType, CippRepository&, bool = false);
        static string sha1(const string& );      

        static map<vector<uint8_t>, vector<uint8_t>> kvlm_parse(vector<uint8_t>&, map<vector<uint8_t>, vector<uint8_t>>&,  int start=0 );
        static vector<uint8_t> kvlm_serialize(map<vector<uint8_t>, vector<uint8_t>>);

    private:
        vector<uint8_t> raw_data;

};

#include "CippBlob.h"

#include "CippCommit.h"

#include "CippObject.cpp"




#endif 

