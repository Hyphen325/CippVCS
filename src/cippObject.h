
#ifndef CIPP_OBJECT_H
#define CIPP_OBJECT_H
#include "../libcipp.h"


class CippObject {
    public:

        CippObject(char*, int);
        CippObject();

        virtual void serialize(CippRepository);
        
        virtual void deserialize(CippRepository);

    private:
        vector<unsigned char> object_read(CippRepository, string);

        string object_write(CippRepository, CippObject, vector<unsigned char>);

        string sha1(const string& );
};

class CippBlob : public CippObject{
    public:
        CippBlob(char*, int);
        CippBlob();

        void serialize(CippRepository) override;
        
        void deserialize(CippRepository) override;

    private:
        vector<unsigned char> blog_data;
        

   

        
};

#endif 