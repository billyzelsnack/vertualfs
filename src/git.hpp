

#ifndef VERTUALFS_GIT_HPP
#define VERTUALFS_GIT_HPP

#include <string>
#include <vector>



//-- todo: get rid of this wrap and put the git2 calls inside of FileSystem


bool git_startup();
bool git_repo_open(struct git_repository** repo, const std::string& path);
bool git_repo_tags(struct git_repository* repo, const std::string& match, std::vector<std::string>& out_tags);
bool git_repo_version(struct git_repository* repo, std::string& out_version); //-- not really a version
void git_repo_close(struct git_repository* repo);
void git_shutdown();



#endif

