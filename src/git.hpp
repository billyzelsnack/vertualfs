

#ifndef VERTUALFS_GIT_HPP
#define VERTUALFS_GIT_HPP

#include <string>
#include <vector>



//-- todo: get rid of this wrap and put the git2 calls inside of FileSystem?

bool git_repo_clone(const std::string& url, const std::string& path, struct git_repository*& out_repo);
void git_repo_close(struct git_repository*& inout_repo);
bool git_repo_open(const std::string& path, struct git_repository*& out_repo);
bool git_repo_tags(struct git_repository* repo, const std::string& match, std::vector<std::string>& out_tags);
bool git_repo_version(struct git_repository* repo, std::string& out_version); //-- not really a version
void git_shutdown();
bool git_startup();



bool git_repo_listing(struct git_repository* repo, const std::string& path, std::vector<std::pair<std::string, bool>>& out_listing);









#endif

