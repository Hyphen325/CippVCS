#ifndef CIPP_CMD_H
#define CIPP_CMD_H
#include "libcipp.h"
#include <any>




int cat_file_cmd(std::string, std::string);
int hash_object_cmd(std::string, std::vector<args::FlagBase*>);
int log_cmd(std::string);
int ls_tree_cmd(std::string, std::vector<args::FlagBase*>);
int checkout_cmd(std::string, std::string);
int show_refs_cmd(); //this command has no args
int tag_cmd(args::Group&, std::vector<args::FlagBase*>);
int rev_parse_cmd(args::Group&, std::vector<args::FlagBase*>);

void log_graphviz(CippRepository, std::string, std::set<std::string>);
void ls_tree(CippRepository , std::string , bool, std::filesystem::path="");
void tree_checkout(CippRepository, CippObject*, std::filesystem::path);
void show_ref();
void show_ref(CippRepository, std::unordered_map<std::filesystem::path, std::any>, filesystem::path = "", bool = true);
unordered_map<filesystem::path, any> ref_list(CippRepository, std::filesystem::path);
void tag_create(CippRepository, std::string, std::string, bool=false);
void ref_create(CippRepository, std::string, std::string);





#include "CippCmd.cpp"


#endif // CIPP_CMD_H