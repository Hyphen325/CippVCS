#include "CippObject.h"


#ifndef CIPPCOMMIT_H
#define CIPPCOMMIT_H

class CippCommit : public CippObject{
    public:
        CippCommit(kvlm_t&);
        CippCommit();

        vector<unsigned char> serialize() override;
        
        void deserialize(vector<unsigned char>) override;

    private:
        map<vector<uint8_t>, vector<uint8_t>> commit_data;
          
};

#include "CippCommit.cpp"

#endif // CIPPBLOB_H