
#ifndef VERTUALFS_VOLUME_HPP
#define VERTUALFS_VOLUME_HPP

#include <filesystem>
#include <map>


namespace vertualfs
{
    struct GitRepository;
    struct Volume;
}

struct vertualfs::Volume
{
    std::filesystem::path root;
    //std::unordered_map<std::filesystem::path, vertualfs::GitRepository*> availableRepositories;
    std::vector<vertualfs::GitRepository*> availableRepositories;
};



#endif
