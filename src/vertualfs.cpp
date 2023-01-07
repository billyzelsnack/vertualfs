
#include <cstdio>

#include "git.hpp"
#include "vertualfs.hpp"

static bool started = false;



	
vertualfs::FileSystem::FileSystem(git_repository* repo) : repo(repo)
{
	std::string version;
	if (git_repo_version(repo, version))
	{
		printf("[%s]\n", version.c_str());
	}
}

bool vertualfs::FileSystem::list(std::vector<std::pair<std::string, bool>>& out_listing)
{
	out_listing.clear();

	printf("list\n");

	out_listing.push_back({ "afile",false });
	out_listing.push_back({ "adir",true });


	return true;
}

vertualfs::FileSystem::~FileSystem()
{
	git_repo_close(repo);
}

vertualfs::FileSystem* vertualfs::FileSystem::create(const std::string& localfullpath)
{
	if (!started) { return nullptr; }

	git_repository* repo = nullptr;
	if (!git_repo_open(&repo, localfullpath)) { return nullptr; }

	vertualfs::FileSystem* filesystem = new vertualfs::FileSystem(repo);

	return filesystem;
};


bool vertualfs_startup()
{
	if(started){ return true; }

	printf("vertualfs_startup\n");
	if(!git_startup()){return false;}

	started = true;

	return true;
}

void vertualfs_shutdown()
{
	if (!started) { return; }
	printf("vertualfs_shutdown\n");

	git_shutdown();
}



