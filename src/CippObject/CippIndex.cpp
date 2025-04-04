#include "CippIndex.h"

CippIndexEntry::CippIndexEntry(std::time_t ctime, std::time_t mtime, uint64_t dev, uint64_t ino,
    ObjectModeType mode_type, uint32_t mode_perms, uint32_t uid, uint32_t gid,
    uint64_t file_size, std::string sha, bool flag_assume_valid, uint32_t flag_stage,
    string name 
) : ctime(ctime),
    mtime(mtime),
    dev(dev),
    ino(ino),
    mode_type(mode_type),
    mode_perms(mode_perms),
    uid(uid),
    gid(gid),
    file_size(file_size),
    sha(sha),
    flag_assume_valid(flag_assume_valid),
    flag_stage(flag_stage),
    name(name)
{  
}

CippIndex::CippIndex(uint32_t version, std::vector<CippIndexEntry> entries) : version(version), entries(entries){}

