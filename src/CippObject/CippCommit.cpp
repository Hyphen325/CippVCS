#include "CippCommit.h"

CippCommit::CippCommit(){
    commit_data = kvlm_t();
    fmt = "commit";
}

CippCommit::CippCommit(kvlm_t& kvlm){
    commit_data = kvlm;
    fmt = "commit";
}

CippCommit::CippCommit(string& data){
    deserialize(raw_t(data.begin(), data.end()));
    fmt = "commit";
}

CippCommit::CippCommit(const vector<uint8_t>& data){
    deserialize(data);
    fmt = "commit";
}

vector<uint8_t> CippCommit::serialize(){
    return CippObject::kvlm_serialize(commit_data);
}
        
void CippCommit::deserialize(vector<uint8_t> data){
    kvlm_t map;
    commit_data = CippObject::kvlm_parse(data, map);
}

