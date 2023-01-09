
#ifndef VERTUALFS_GITFILESYSTEM_HPP
#define VERTUALFS_GITFILESYSTEM_HPP

#include <string>
#include <vector>


namespace vertualfs
{
	class GitFileSystem;
}

struct git_repository;
struct git_commit;
struct git_tree;

class vertualfs::GitFileSystem
{

private:

	GitFileSystem(struct git_repository* repo, struct git_commit* commit, struct git_tree* tree);
	~GitFileSystem();

public:

	bool listing(const std::string& path, std::vector<std::pair<std::string, bool>>& out_files);

	bool repo_version(std::string& out_version) const;


	bool lookup_remote_url(const std::string& name, std::string& out_url) const;


	static GitFileSystem* create(const std::string& path);

public:

	git_repository* repo = nullptr;
	git_commit* commit = nullptr;
	git_tree* tree = nullptr;

};


void GitFileSystem_shutdown();
bool GitFileSystem_startup();


#endif

