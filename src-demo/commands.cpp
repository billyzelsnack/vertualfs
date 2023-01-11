

#include "commands.hpp"

#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include <lyra/arg.hpp>
#include <lyra/command.hpp>
#include <lyra/cli.hpp>
#include <lyra/opt.hpp>

#include <vertualfs/Hub.hpp>
#include <vertualfs/Filesystem.hpp>
#include <vertualfs/Repository.hpp>




bool gexited=false;

/*
std::string gprompt = ">";



//-- todo: change all the std::string path to std::filesystem::path
//-- todo: make sure that all names are valid file names
//-- todo: redo commands now that you understand command groups better



vertualfs::Volume gvolume;



static bool get_index_by_indexorname(const std::string& indexorname, int& inout_index)
{
    //-- yeah i did that
    try{ inout_index=std::stoi(indexorname); return true; } catch (...) { return false; }
}

static vertualfs::GitRepository* get_repository_by_urlandtag(vertualfs::Volume* volume, const std::string& url, const std::string& tag)
{
    if (volume == nullptr) { return nullptr; }
    if (url.empty()) { return nullptr; }
   
    for (auto* repository : volume->availableRepositories) 
    {             
        vertualfs::GitFilesystem* fs=vertualfs::GitFilesystem::create(repository);
        if (fs != nullptr)
        {
            std::string testurl;
            if(fs->lookup_remote_url("origin", testurl))
            {
                //printf("[%s]?=[%s]\n", testurl.c_str(), url.c_str());
                if (testurl == url) { delete fs; return repository; }
            }
        }
        delete fs;
    }

    return nullptr;
}

*/

std::string vertualfstool::prompt()
{
    std::string prompt;
/*
    vertualfs::Volume* volume = &gvolume;
    if (volume != nullptr){ prompt += "volume-"+std::string("name"); }

    //if (!prompt.empty()) { prompt += "/"; }
*/
    return prompt + ">";
}

bool vertualfstool::help(const std::string& arg) 
{ 
    printf("help [exit]\n");
    printf("exit");

    printf("volume-name>help [ensureavailable|ls|cd|ensureunavailable]\n");
    printf("volume-name>ensureavailable repository <url [tag]>\n");
    printf("volume-name>ls repositories\n");
    printf("volume-name>cd repository <#|url [tag]>\n");
    printf("volume-name>ensureunavailable repository <#|url [tag]>\n");
    printf("volume-name>create filesystem <#|url [tag]> <name>\n");
    printf("volume-name>ls filesystems\n");
    printf("volume-name>cd filesystem <#|name>\n");
    printf("volume-name>delete filesystem <#|name>\n");

    printf("volume-name/filesystem-name>help [ls|cd|status|commit|push]\n");
    printf("volume-name/filesystem-name>ls\n");
    printf("volume-name/filesystem-name>cd <#|name>\n");
    printf("volume-name/filesystem-name>status\n");
    printf("volume-name/filesystem-name>commit [-m <msg>]\n");
    printf("volume-name/filesystem-name>push\n");

    return true; 
}

bool vertualfstool::exit() 
{ 
    gexited = true;
    return true;
}

bool vertualfstool::exited()
{
    return gexited;
}

bool vertualfstool::volume_help(const std::string& arg) { return false; }

/*

bool vertualfstool::volume_ensureavailable_repository(const std::string& url, const std::string& tag) 
{ 
    printf("{volume ensureavailable repository [%s] [%s]}\n",url.c_str(),tag.c_str());

    vertualfs::Volume* volume = &gvolume;
    if (volume == nullptr)
    {
        printf("volume==nullptr");
        return false;
    }

    vertualfs::GitRepository* repository = get_repository_by_urlandtag(volume, url, tag);
    if(repository == nullptr)
    {
        repository=vertualfs::GitRepository_Create(url);
        volume->availableRepositories.push_back(repository);
    }

    return true;
}



bool vertualfstool::volume_ls_repositories() 
{ 
    printf("{volume ls repositories}\n");

    vertualfs::Volume* volume = &gvolume;
    if (volume == nullptr)
    {
        printf("volume==nullptr");
        return false;
    }

    for (int ii=0; ii<volume->availableRepositories.size(); ii++)
    {
        vertualfs::GitFilesystem* fs = vertualfs::GitFilesystem::create(volume->availableRepositories[ii]);
        if (fs != nullptr)
        {
            std::string url;
            if (fs->lookup_remote_url("origin", url))
            {
                printf("%d [%s]\n", ii, url.c_str());
            }
        }
        delete fs;
    }

    return true;
}


bool vertualfstool::volume_cd_repository(const std::string& indexorurl, const std::string& tag) { return false; }
bool vertualfstool::volume_ensureunavailable_repository(const std::string& indexorurl, const std::string& tag) { return false; }
bool vertualfstool::volume_create_filesystem(const std::string& indexorurl, const std::string& tag, const std::string& name) { return false; }
bool vertualfstool::volume_ls_filesystems() { return false; }
bool vertualfstool::volume_cd_filesystem(const std::string& indexorname) { return false; }
bool vertualfstool::volume_delete_filesystem(const std::string& indexorname) { return false; }

bool vertualfstool::volume_filesystem_help(const std::string& arg) { return false; }
bool vertualfstool::volume_filesystem_ls() { return false; }
bool vertualfstool::volume_filesystem_cd(const std::string& indexorname) { return false; }
bool vertualfstool::volume_filesystem_status() { return false; }
bool vertualfstool::volume_filesystem_commit(const std::string& arg) { return false; }
bool vertualfstool::volume_filesystem_push() { return false; }


*/

