
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

#include <easylogging++.h>

#include <vertualfs.hpp>

#include "commands.hpp"
#include "GitFileSystem.hpp"

INITIALIZE_EASYLOGGINGPP


//-- todo: change all the std::string path to std::filesystem::path


vertualfs::GitFileSystem* selectedMount = nullptr;
std::vector<vertualfs::GitFileSystem*> mounts;






bool vertualfstool_startup()
{
    printf("vertualfstool startup\n");

    if (!vertualfs_startup()) { return false; }


    return true;
}

void vertualfstool_shutdown()
{
    printf("vertualfstool shutdown\n");

    vertualfs_shutdown();
}





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
    for(int ii=0; ii< mounts.size(); ii++)
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
    if(index<0) { return false; }
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





int main(int argc, const char** argv)
{
    LOG(INFO) << "vertualfs";  
    if(!vertualfstool_startup()){return EXIT_FAILURE;}

    commands_process(argc, argv);

    mount_ensureadded("c:/repos/headtest");
    mount_ensureadded("https://github.com/billyzelsnack/simple_onshape_exportstls.git");
    mount_ensureadded("https://gitlab.com/telemotor/users/billy/headtest");

    commands_process(argv[0],"help");

    while(1)
    {
        std::string line;
        std::getline(std::cin, line);

        if(!commands_process(argv[0],line))
        {
            break;
        }
    }

    vertualfstool_shutdown();

    return EXIT_SUCCESS;
}













