
#ifndef VERTUALFS_REPOSITORY_HPP
#define VERTUALFS_REPOSITORY_HPP

#include <filesystem>
#include <string>

struct git_repository;
struct git_commit;
struct git_tree;

namespace vertualfs
{
	struct Repository;
}

struct vertualfs::Repository
{
	git_repository* repository = nullptr;
	git_commit* commit = nullptr;

	Repository(git_repository* repo, git_commit* commit);
	~Repository();
};

namespace vertualfs
{
	std::string Repository_CreateLocalPath(const std::string& url);
	vertualfs::Repository* Repository_Create(const std::string& path);
	void Repository_Shutdown();
	bool Repository_Startup();
}

#endif

