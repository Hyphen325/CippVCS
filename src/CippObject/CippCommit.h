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

        //key value map of two uint8 vectors
        kvlm_t commit_data;
          
};

#include "CippCommit.cpp"

#endif // CIPPBLOB_H