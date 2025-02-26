#define MAX_WRITE 1000 //limits the numbers of characters you can write at once to any config files

using namespace std;


CippRepository CippRepository::repo_find(filesystem::path path=".", bool required=true){
    path = filesystem::absolute(path);

    if(filesystem::is_directory(path/".git")){
        return CippRepository(path);
    }

    filesystem::path parent_path = path.parent_path();
    if(parent_path == path){
        throw runtime_error("Not in a git directory");
    }

    return repo_find(parent_path, required);
}

filesystem::path CippRepository::repo_path(CippRepository repo, filesystem::path path){
    return repo.gitdir/path;
}


filesystem::path  CippRepository::repo_dir(filesystem::path project_path, filesystem::path dir_path, bool mkdir=false){
    auto path = project_path/dir_path;
    if(filesystem::exists(path)){
        if(filesystem::is_directory(path)){
            return path;
        }else{
            throw runtime_error("Path is not a directory!");
        }
    }

    if(mkdir){
        filesystem::create_directories(path);
            return path;
    }else{
        return filesystem::path("");
    }
}

CippRepository CippRepository::createRepo(filesystem::path path){

    CippRepository repo = CippRepository(path, true);

    if(filesystem::exists(repo.worktree)){
        if(!filesystem::is_directory(repo.worktree)){
            throw runtime_error("Path is not a directory!");
        }
        if(filesystem::exists(repo.gitdir) && !filesystem::is_empty(repo.gitdir)){
            runtime_error("Path is already full!");
        }
    }else{
        filesystem::create_directories(repo.worktree);
    }

    repo_dir(repo.gitdir,"branches", true);
    repo_dir(repo.gitdir,"objects", true);
    repo_dir(repo.gitdir,"refs/tags", true);
    repo_dir(repo.gitdir,"refs/heads", true);

    ofstream writer(repo.gitdir/"description");
    writer << "Unnamed Repository; edit this file 'description' to name the repository";
    writer.close();

    writer.open(repo.gitdir/"HEAD");
    writer << "ref: refs/heads/master\n";
    writer.close();

    writer.open(repo.gitdir/"config");
    auto repo_config = default_config();
    write_config(writer, repo_config);
    writer.close();

    cout << "Repo initialization complete" << endl;
    cout << "New worktree: " << repo.worktree.c_str() << endl;
    cout << "New git directory: " << repo.gitdir.c_str() << endl;


    return repo;


}


/*Constructor that inits a working repo on the supplied directory*/
CippRepository::CippRepository(filesystem::path path, bool force){
    this->worktree = path;
    
    this->gitdir = worktree / ".git";
    

    if(!(force || filesystem::is_directory(path))){
        throw(runtime_error("Not a git repotitory "));
    }

    filesystem::path configPath = repo_file("config");

    if(!configPath.empty() && filesystem::exists(configPath)){
        readConfig(configPath);
    } else if(!force){
        throw runtime_error("Config file missing");
    }

    if(!force) {
        bool flag = true;
        for(auto it1: conf){
            auto it2 = it1.second.find("core.repositoryformatversion");
            if(it2 != it1.second.end()){
                int vers = stoi(it2->second);
                flag = false;
                if(vers != 0){
                    throw runtime_error("Unsupported repository format version");
                }
            }
        }
        if(flag){
            throw runtime_error("Missing reporitory format version in config file");
        }
}


}


filesystem::path CippRepository::repo_file(filesystem::path local_path){
    return worktree / local_path;
}

void CippRepository::readConfig(filesystem::path configPath) {
    std::ifstream file(configPath);
    if (!file) {
        throw std::runtime_error("Failed to open config file");
    }
    std::string line, currentSection;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == ';' || line[0] == '#') {
            continue;
        }
        if (line.front() == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.size() - 2);
        } else {
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                conf[currentSection][trim(key)] = trim(value);
            }
        }
    }
}
std::string CippRepository::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

void CippRepository::write_config(ofstream& writer, unordered_map<string, unordered_map<string, string>> config){
    for(auto it : config){
        writer << "[" << it.first.c_str() << "]" << endl;
        for(auto pairs : it.second){
            writer << pairs.first << "=" << pairs.second << endl;
        }
    }
}

/*Creates the default configuration map for a new repository*/
unordered_map<string, unordered_map<string, string>> CippRepository::default_config(){
    unordered_map<string, unordered_map<string, string>> ret;
    ret["core"]["repositoryformatversion"] = "0";
    ret["core"]["filemode"] = "false";
    ret["core"]["bare"] = "false";
    return ret;
}

