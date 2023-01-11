
#ifndef VERTUALFS_GITREPOSITORY_HPP
#define VERTUALFS_GITREPOSITORY_HPP

#include <string>

struct git_repository;
struct git_commit;
struct git_tree;

namespace vertualfs
{
	struct GitRepository;
}

struct vertualfs::GitRepository
{
	git_repository* repo = nullptr;
	git_commit* commit = nullptr;
	git_tree* tree = nullptr;

	GitRepository(git_repository* repo, git_commit* commit, git_tree* tree);
	~GitRepository();
};

namespace vertualfs
{
	vertualfs::GitRepository* GitRepository_Create(const std::string& path);
	void GitRepository_Shutdown();
	bool GitRepository_Startup();
}

#endif

