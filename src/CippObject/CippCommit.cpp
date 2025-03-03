#include "CippCommit.h"

CippCommit::CippCommit(){
    commit_data = kvlm_t();
}

CippCommit::CippCommit(kvlm_t& kvlm){
    commit_data = kvlm;
}

vector<uint8_t> CippCommit::serialize(){
    return CippObject::kvlm_serialize(commit_data);
}
        
void CippCommit::deserialize(vector<uint8_t> data){
    kvlm_t map;
    commit_data = CippObject::kvlm_parse(data, map);
}