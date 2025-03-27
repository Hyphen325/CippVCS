#include "CippTag.h"

CippTag::CippTag(){
    commit_data = kvlm_t();
    fmt = "tag";
}

CippTag::CippTag(string data){
    deserialize(raw_t(data.begin(), data.end()));
}

CippTag::CippTag(raw_t data){
    deserialize(data);
}
