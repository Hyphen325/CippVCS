// filepath: /home/noah/Projects/CippVCS/src/CippObject/CippTag.h
#ifndef CIPPTAG_H
#define CIPPTAG_H

#include "CippObject.h"
#include "CippCommit.h"
#include <string>
#include <vector>

using namespace std;

class CippTag : public CippCommit {
public:
    CippTag();
    CippTag(string);
    ~CippTag();

};


#include "CippTag.cpp"

#endif // CIPPTAG_H