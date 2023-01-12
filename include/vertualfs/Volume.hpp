
#ifndef VERTUALFS_VOLUME_HPP
#define VERTUALFS_VOLUME_HPP

#include <filesystem>
#include <map>


namespace vertualfs
{
    class Filesystem;
    struct Repository;
    struct Volume;
}

struct vertualfs::Volume
{
    //std::unordered_map<std::filesystem::path, vertualfs::GitRepository*> availableRepositories;
    std::vector<vertualfs::Repository*> availableRepositories;
    vertualfs::Filesystem* filesystem = nullptr;

    static vertualfs::Volume* create(const std::filesystem::path& hubpath);
private:
    Volume() {}
};


#endif
