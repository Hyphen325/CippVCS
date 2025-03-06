#include "CippBlob.h"

/*Blob Class*/
/*---------------------------------------------------------------------------------*/
CippBlob::CippBlob(const char* data, int size){
    blob_data = vector<unsigned char>(data, data + size);
}

CippBlob::CippBlob(){
    blob_data = vector<unsigned char>();
}

CippBlob::CippBlob(const vector<uint8_t>& data){
    blob_data = data;
}

vector<unsigned char> CippBlob::serialize(){
    return blob_data;
}

void CippBlob::deserialize(vector<unsigned char> data){
    blob_data = data;
}




