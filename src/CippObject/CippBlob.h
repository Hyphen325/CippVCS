#include "CippObject.h"


#ifndef CIPPBLOB_H
#define CIPPBLOB_H



class CippBlob : public CippObject{
    public:
        CippBlob(const char*, int);
        CippBlob();

        vector<unsigned char> serialize() override;
        
        void deserialize(vector<unsigned char>) override;

    private:
        vector<unsigned char> blob_data;
          
};

#include "CippBlob.cpp"

#endif // CIPPBLOB_H