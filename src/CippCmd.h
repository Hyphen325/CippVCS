#ifndef CIPP_CMD_H
#define CIPP_CMD_H
#include "libcipp.h"
#include <any>

using namespace std;
using namespace args;



int cat_file_cmd(string, string);
int hash_object_cmd(string, vector<FlagBase*>);
int log_cmd(string);
int ls_tree_cmd(string, vector<FlagBase*>);
int checkout_cmd(string, string);
int show_ref_cmd(); //this command has no args
int tag_cmd(Group, vector<FlagBase*>);

void log_graphviz(CippRepository, string, set<string>);
void ls_tree(CippRepository , string , bool, filesystem::path="");
void tree_checkout(CippRepository, CippObject*, filesystem::path);
void show_ref();
void show_ref(CippRepository, std::unordered_map<std::filesystem::path, std::any>, filesystem::path = "", bool = true);
unordered_map<filesystem::path, any> ref_list(CippRepository, filesystem::path);
raw_t ref_resolve(CippRepository, filesystem::path);



#include "CippCmd.cpp"


#endif // CIPP_CMD_H