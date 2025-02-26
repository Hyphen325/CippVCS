#ifndef CIPP_CMD_H
#define CIPP_CMD_H
#include "libcipp.h"
using namespace args;


int cat_file_cmd(string, string);
int hash_object_cmd(string, vector<FlagBase*>);



#include "cippCmd.cpp"


#endif // CIPP_CMD_H