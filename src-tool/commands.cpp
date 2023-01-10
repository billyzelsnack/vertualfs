
#include<filesystem>
#include<string>
#include<vector>

#include <lyra/arg.hpp>
#include <lyra/cli.hpp>
#include <lyra/command.hpp>
#include <lyra/opt.hpp>

#include "GitFilesystem.hpp"
#include "GitRepository.hpp"

#include "commands.hpp"


bool gexited=false;




//-- todo: change all the std::string path to std::filesystem::path

struct Volume
{
    std::vector<vertualfs::GitRepository*> repositories;
    std::vector<vertualfs::GitFilesystem*> filesystems;
};

static Volume* gvolume = new Volume();




bool volume_repository_ensureavailable(const std::string& path)
{
    std::filesystem::path stdpath(path);
    stdpath.replace_extension(); //-- unify to never having the .git extension

    for (const auto repository : gvolume->repositories)
    {
        vertualfs::GitFilesystem* fs = vertualfs::GitFilesystem::create(repository);
        std::string url;
        fs->lookup_remote_url("origin", url);
        delete fs;
        std::filesystem::path stdurl(url);
        stdurl.replace_extension(); //-- unify to never having the .git extension

        if (stdpath == stdurl) { return true; }
    }

    vertualfs::GitRepository* repository = vertualfs::GitRepository_Create(stdpath.string());
    if (repository == nullptr) { return false; }
    gvolume->repositories.push_back(repository);

    return true;
}

bool volume_filesystem_create(int repositoryIndex)
{
    if (repositoryIndex < 0) { return false; }
    if (repositoryIndex < gvolume->filesystems.size())
    {
        auto repository = gvolume->repositories[repositoryIndex];
        vertualfs::GitFilesystem* filesystem = vertualfs::GitFilesystem::create(repository);
        if (filesystem == nullptr) { return false; }
        gvolume->filesystems.push_back(filesystem);
        return true;
    }

    return false;
}

void volume_repository_listing()
{
    for (int ii = 0; ii < gvolume->repositories.size(); ii++)
    {
        const auto repository = gvolume->repositories[ii];
        std::string url;
        vertualfs::GitFilesystem* fs = vertualfs::GitFilesystem::create(repository);
        fs->lookup_remote_url("origin", url);
        delete fs;
        std::filesystem::path stdurl(url);
        stdurl.replace_extension(); //-- unify to never having the .git extension

        printf("%d [%s]\n", ii, stdurl.string().c_str());
    }
}



bool vertualfstool::help(const std::string& arg) 
{ 
    printf("help [create|ls|cd|delete|exit]\n");
    printf("create volume <name>\n");
    printf("ls volume\n");
    printf("cd volume <#|name>\n");
    printf("delete volume <#|name>\n");
    printf("exit");

    printf("volume-name>help [ensureavailable|ls|cd|ensureunavailable]\n");
    printf("volume-name>ensureavailable repository <url[:tag]>\n");
    printf("volume-name>ls repository\n");
    printf("volume-name>cd repository <#|url[:tag]>\n");
    printf("volume-name>ensureunavailable repository <#|url[:tag]>\n");
    printf("volume-name>create filesystem <#|url:[tag]> <name>\n");
    printf("volume-name>ls filesystem\n");
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

bool vertualfstool::create_volume(const std::string& indexorname) { return false; }
bool vertualfstool::ls_volume() { return false; }
bool vertualfstool::cd_volume(const std::string& indexorname) { return false; }

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
bool vertualfstool::volume_ensureavailable_repository(const std::string& url, const std::string& tag) { return false; }
bool vertualfstool::volume_ls_repository() { return false; }
bool vertualfstool::volume_cd_repository(const std::string& indexorurl, const std::string& tag) { return false; }
bool vertualfstool::volume_ensureunavailable_repository(const std::string& indexorurl, const std::string& tag) { return false; }
bool vertualfstool::volume_create_filesystem(const std::string& indexorurl, const std::string& tag, const std::string& name) { return false; }
bool vertualfstool::volume_ls_filesystem() { return false; }
bool vertualfstool::volume_cd_filesystem(const std::string& indexorname) { return false; }
bool vertualfstool::volume_delete_filesystem(const std::string& indexorname) { return false; }

bool vertualfstool::volume_filesystem_help(const std::string& arg) { return false; }
bool vertualfstool::volume_filesystem_ls() { return false; }
bool vertualfstool::volume_filesystem_cd(const std::string& indexorname) { return false; }
bool vertualfstool::volume_filesystem_status() { return false; }
bool vertualfstool::volume_filesystem_commit(const std::string& arg) { return false; }
bool vertualfstool::volume_filesystem_push() { return false; }

