
#ifndef VERTUALFS_VOLUME_HPP
#define VERTUALFS_VOLUME_HPP

#include <filesystem>
#include <map>


namespace vertualfs
{
    class GitFilesystem;
    struct GitRepository;
    struct Hub;
}

struct vertualfs::Hub
{
    //std::unordered_map<std::filesystem::path, vertualfs::GitRepository*> availableRepositories;
    std::vector<vertualfs::GitRepository*> availableRepositories;
    vertualfs::GitFilesystem* filesystem = nullptr;

    static vertualfs::Hub* create(const std::filesystem::path& hubpath);
private:
    Hub() {}
};


#endif
