#include "CippBlob.h"

/*Blob Class*/
/*---------------------------------------------------------------------------------*/
CippBlob::CippBlob(const char* data, int size){
    blob_data = vector<unsigned char>(data, data + size);
}

CippBlob::CippBlob(){
    blob_data = vector<unsigned char>();
}

vector<unsigned char> CippBlob::serialize(){
    return blob_data;
}

void CippBlob::deserialize(vector<unsigned char> data){
    blob_data = data;
}
