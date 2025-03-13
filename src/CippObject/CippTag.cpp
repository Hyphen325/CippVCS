#include "CippTag.h"


CippTag::CippTag(string data){
    deserialize(raw_t(data.begin(), data.end()));
}
