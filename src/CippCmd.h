#ifndef CIPP_CMD_H
#define CIPP_CMD_H
#include "libcipp.h"
using namespace args;


int cat_file_cmd(string, string);
int hash_object_cmd(string, vector<FlagBase*>);
int log_cmd(string);
int ls_tree_cmd(string, vector<FlagBase*>);
int checkout_cmd(string, string);

void log_graphviz(CippRepository, string, set<string>);
void ls_tree(CippRepository , string , bool, filesystem::path="");
void tree_checkout(CippRepository, CippObject*, filesystem::path);



#include "CippCmd.cpp"


#endif // CIPP_CMD_H