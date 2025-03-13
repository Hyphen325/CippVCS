#include "CippTree.h"
using namespace std;

std::vector<uint8_t> uint64_to_vector(uint64_t value) {
    std::vector<uint8_t> result(sizeof(value));
    for (size_t i = 0; i < sizeof(value); ++i) {
        result[i] = (value >> (i * 8)) & 0xFF;
    }
    std::reverse(result.begin(), result.end()); // Optional: reverse for desired byte order
    return result;
}


CippTreeLeaf::CippTreeLeaf(uint64_t mode, filesystem::path path, string sha){
    this->mode = mode;
    this->path = path;
    this->sha = sha;
}

CippTree::CippTree(string data){
    raw_t raw = raw_t(data.begin(), data.end());
    deserialize(raw);
}

tuple<int, CippTreeLeaf> CippTree::tree_parse_one(const vector<uint8_t>& data, int start = 0){
    
    string raw_str(data.begin(), data.end());

    auto space = raw_str.find(' ');

    //finds the mode of the tree
    auto mode = raw_str.substr(start, space);
    if(mode.length() == 5){
        mode.insert(mode.begin(), 1, '0');
    }

    auto null_term = raw_str.find('\0', space + 1);

    filesystem::path path = raw_str.substr(space+1, null_term - (space+1));

    string sha;
    for (uint32_t i = null_term + 1; i < null_term + 21; ++i) {
        char buffer[3];
        snprintf(buffer, sizeof(buffer), "%02x", static_cast<unsigned char>(data[i]));
        sha += buffer;
    }

    CippTreeLeaf leaf(stoi(mode), path, sha);
    return make_tuple(null_term + 21, leaf);
}

vector<CippTreeLeaf> CippTree::tree_parse(const vector<uint8_t>&data){
    int pos = 0;
    int max = data.size();
    vector<CippTreeLeaf> ret;
    while(pos < max){
        tuple<int, CippTreeLeaf> func = tree_parse_one(data, pos);
        pos = std::get<0>(func);
        ret.push_back(std::get<1>(func));
    }
    return ret;

}
bool CippTree::tree_leaf_sort_key(CippTreeLeaf& leaf1, CippTreeLeaf& leaf2){
    // If leafs are directories, appends / after them so they do not sort before normal files
    if(!((leaf1.mode & (3UL << 62)) == 2)){
        leaf1.path/ "/";
    }

    if(!((leaf2.mode & (3UL << 62)) == 2)){
        leaf2.path/ "/";
    }

    return leaf1.path < leaf2.path;
}

vector<uint8_t> CippTree::serialize(){
    sort(tree_data.begin(), tree_data.end(), tree_leaf_sort_key);

    vector<uint8_t> ret;

    for(CippTreeLeaf leaf : tree_data){
        vector<uint8_t> vec = uint64_to_vector(leaf.mode);

        //inserts the mode for return
        ret.insert(ret.end(), vec.begin(), vec.end());
        ret.push_back(' ');

        string utfpath = leaf.path.u8string();

        ret.insert(ret.end(), utfpath.begin(), utfpath.end());

        ret.push_back(0);ret.push_back(0);

        //inserts the sha key
        ret.insert(ret.end(), leaf.sha.begin(), leaf.sha.end());
    }
    return ret;
}

void CippTree::deserialize(raw_t data){
    tree_data = CippTree::tree_parse(data);
}




