#ifndef CIPPTREE_H
#define CIPPTREE_H
using namespace std;
#include "CippObject.h"

class CippTreeLeaf {
    public:
        CippTreeLeaf(uint64_t, filesystem::path, string);
    
        uint64_t mode;
        filesystem::path path;
        string sha;
    
};


class CippTree : public CippObject{
public:
    CippTree();
    

   
    void deserialize(vector<uint8_t>) override;
    vector<uint8_t> serialize() override;

    static tuple<int, CippTreeLeaf> tree_parse_one(vector<uint8_t>, int);
    static vector<CippTreeLeaf> tree_parse(vector<uint8_t>);
    static filesystem::path tree_leaf_sort_key(CippTreeLeaf);

    
private:
    vector<CippTreeLeaf> tree_data;
};

#include "CippTree.cpp"

#endif // CIPPTREE_H