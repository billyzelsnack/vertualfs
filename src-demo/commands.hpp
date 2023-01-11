
#ifndef VERTUALFSTOOL_COMMANDS_HPP
#define VERTUALFSTOOL_COMMANDS_HPP

#include <string>

namespace vertualfstool
{
    bool help(const std::string& arg);
    bool exit();

    bool volume_help(const std::string& arg);
    bool volume_ensureavailable_repository(const std::string& url, const std::string& tag);
    bool volume_ls_repositories();
    bool volume_cd_repository(const std::string& indexorurl, const std::string& tag);
    bool volume_ensureunavailable_repository(const std::string& indexorurl, const std::string& tag);
    bool volume_create_filesystem(const std::string& indexorurl, const std::string& tag, const std::string& name);
    bool volume_ls_filesystems();
    bool volume_cd_filesystem(const std::string& indexorname);
    bool volume_delete_filesystem(const std::string& indexorname);

    bool volume_filesystem_help(const std::string& arg);
    bool volume_filesystem_ls();
    bool volume_filesystem_cd(const std::string& indexorname);
    bool volume_filesystem_status();
    bool volume_filesystem_commit(const std::string& arg);
    bool volume_filesystem_push();

    bool exited();
    std::string prompt();
}




#endif
