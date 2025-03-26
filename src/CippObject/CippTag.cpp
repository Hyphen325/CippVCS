#include "CippTag.h"

CippTag::CippTag(){
    commit_data = kvlm_t();
}

CippTag::CippTag(string data){
    deserialize(raw_t(data.begin(), data.end()));
}
