#include "CippTree.h"
using namespace std;


CippTreeLeaf::CippTreeLeaf(uint64_t mode, filesystem::path path, string sha){
    this->mode = mode;
    this->path = path;
    this->sha = sha;
}

tuple<int, CippTreeLeaf> CippTree::tree_parse_one(vector<uint8_t> data, int start = 0){
    
}

vector<CippTreeLeaf> CippTree::tree_parse(vector<uint8_t>data){

}
filesystem::path CippTree::tree_leaf_sort_key(CippTreeLeaf leaf){

}
