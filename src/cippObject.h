#include "libcipp.h"

#ifndef CIPP_OBJECT_H
#define CIPP_OBJECT_H

using namespace std;

class CippObject {
    public:

        

        virtual vector<unsigned char> serialize();
        virtual void deserialize(vector<unsigned char>);

        static vector<unsigned char> object_read(CippRepository, string);
        static string object_write(CippRepository, CippObject, vector<unsigned char>);
        static string object_find(CippRepository, string);
        static string object_hash(CippObject);
        static string sha1(const string& );

    
        
};

class CippBlob : public CippObject{
    public:
        CippBlob(char*, int);
        CippBlob();

        vector<unsigned char> serialize() override;
        
        void deserialize(vector<unsigned char>) override;

    private:
        vector<unsigned char> blob_data;
        

   

        
};

#include "cippObject.cpp"



#endif 

