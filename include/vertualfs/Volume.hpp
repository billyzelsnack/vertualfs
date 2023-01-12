
#ifndef VERTUALFS_VOLUME_HPP
#define VERTUALFS_VOLUME_HPP

#include <filesystem>
#include <map>


namespace vertualfs
{
    class Filesystem;
    struct Volume;
}

struct vertualfs::Volume
{
    std::vector<vertualfs::Filesystem*> availableFilesystems;

    static vertualfs::Volume* create(const std::filesystem::path& path);
private:
    Volume() {}
};


#endif
