#include "CippObject.h"


#ifndef CIPPCOMMIT_H
#define CIPPCOMMIT_H

class CippCommit : public CippObject{
    public:
        CippCommit();
        CippCommit(kvlm_t&);
        CippCommit(string&);
        CippCommit(const vector<uint8_t>&);

        vector<unsigned char> serialize() override;
        
        void deserialize(vector<unsigned char>) override;

        map<vector<uint8_t>, vector<uint8_t>> commit_data;
          
};

#include "CippCommit.cpp"

#endif // CIPPBLOB_H