#ifndef CIPP_CMD_H
#define CIPP_CMD_H
#include "libcipp.h"
using namespace args;


int cat_file_cmd(string, string);
int hash_object_cmd(string, vector<FlagBase*>);
int log_cmd(string);

void log_graphviz(CippRepository, string, set<string>);



#include "CippCmd.cpp"


#endif // CIPP_CMD_H