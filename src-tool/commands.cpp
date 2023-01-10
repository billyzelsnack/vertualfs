
#include<filesystem>

#include <lyra/arg.hpp>
#include <lyra/cli.hpp>
#include <lyra/command.hpp>
#include <lyra/opt.hpp>

#include "GitFileSystem.hpp"

#include "commands.hpp"



//-- todo: change all the std::string path to std::filesystem::path
static vertualfs::GitFileSystem* selectedMount = nullptr;
static std::vector<vertualfs::GitFileSystem*> mounts;





bool mount_ensureadded(const std::string& path)
{
    std::filesystem::path stdpath(path);
    stdpath.replace_extension(); //-- unify to never having the .git extension

    for (const auto mount : mounts)
    {
        std::string url;
        mount->lookup_remote_url("origin", url);
        std::filesystem::path stdurl(url);
        stdurl.replace_extension(); //-- unify to never having the .git extension

        if (stdpath == stdurl) { return true; }
    }

    vertualfs::GitFileSystem* fs = vertualfs::GitFileSystem::create(stdpath.string());
    if (fs == nullptr) { return false; }
    mounts.push_back(fs);

    return true;
}

void list_mounts()
{
    for (int ii = 0; ii < mounts.size(); ii++)
    {
        const auto mount = mounts[ii];
        std::string url;
        mount->lookup_remote_url("origin", url);
        std::filesystem::path stdurl(url);
        stdurl.replace_extension(); //-- unify to never having the .git extension

        printf("%d [%s]\n", ii, stdurl.string().c_str());
    }
}


bool select_mount(int index)
{
    if (index < 0) { return false; }
    if (index < mounts.size()) { selectedMount = mounts[index]; return true; }
    return false;
}


bool mount_ls()
{
    if (selectedMount == nullptr) { return false; }

    std::vector<std::pair<std::string, bool>> listing;
    if (!selectedMount->listing("", listing)) { return false; }

    return true;
}
