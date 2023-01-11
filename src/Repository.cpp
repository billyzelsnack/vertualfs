
#include "vertualfs/Repository.hpp"

#include <filesystem>
#include <regex>
#include <string>

#include <git2.h>


//-- after an open need to check if the remote has changed



static bool started = false;

void vertualfs::GitRepository_Shutdown()
{
	if (!started) { return; }

	git_libgit2_shutdown();
	started = false;

	return;
}

bool vertualfs::GitRepository_Startup()
{
	if (started) { return true; }

	if (0 == git_libgit2_init()) { return false; }
	started = true;

	return true;
}


vertualfs::GitRepository::GitRepository(git_repository* repo, git_commit* commit, git_tree* tree) : repo(repo), commit(commit), tree(tree)
{

}

vertualfs::GitRepository::~GitRepository()
{
	git_tree_free(tree);
	git_commit_free(commit);
	git_repository_free(repo);
}

vertualfs::GitRepository* vertualfs::GitRepository_Create(const std::string& path)
{
	if (!started) { return nullptr; }

	std::filesystem::path stdpath(path);	
	stdpath.replace_extension(); //-- unify to never having the .git extension
	std::string remoteurl = stdpath.string();

	std::string protocol=stdpath.root_name().string();
	std::string domain=stdpath.root_directory().string();
	std::regex url_regex("(https?)://([^/]+)(.*)");
	std::smatch url_match;
	if(std::regex_match(remoteurl, url_match, url_regex))
	{
		protocol=url_match[1];
		domain=url_match[2];
		std::string modpath = remoteurl;
		modpath.erase(0, protocol.length() + 2 + domain.length() + 1);
		stdpath = std::filesystem::path(modpath);
	}
	else
	{
		domain = protocol;
		domain.erase(std::remove(domain.begin(), domain.end(), ':'), domain.end());
	}
	//std::cout << "Protocol: " << protocol << std::endl;
	//std::cout << "Domain: " << domain << std::endl;
	//std::cout << "Path: " << stdpath.relative_path() << std::endl;
	//std::cout << "Filename: " << stdpath.filename() << std::endl;
	//std::cout << "Filename stem: " << stdpath.stem() << std::endl;
	//std::cout << "Filename extension: " << stdpath.extension() << std::endl;

	std::string openpath = "vertualfs";
	if (!domain.empty()){openpath += "/" + domain;}
	openpath+="/" + stdpath.relative_path().string();

	//printf("[%s] [%s]\n", remoteurl.c_str(), openpath.c_str());
	//return nullptr;

	git_repository* repo = nullptr;
	printf("attempt git_repository_open [%s]\n", openpath.c_str());
	if(0!=git_repository_open(&repo, openpath.c_str()))
	{
		printf("attempt git_repository_clone [%s] to [%s]\n", remoteurl.c_str(), openpath.c_str());
		if (0 != git_clone(&repo, remoteurl.c_str(), openpath.c_str(), nullptr))
		{
			const git_error* error = git_error_last();
			printf("Error: %s\n", error->message);
			return nullptr;
		}
	}

	git_reference* commitref = nullptr;
	git_repository_head(&commitref, repo);
	git_commit* commit = nullptr;
	git_reference_peel((git_object**)&commit, commitref, GIT_OBJ_COMMIT);
	git_reference_free(commitref); //-- todo: can i delete this immediately
	if(commit == nullptr)
	{
		git_repository_free(repo);
		return nullptr;
	}

	git_tree* tree=nullptr;
	git_commit_tree(&tree, commit);
	if (tree == nullptr)
	{
		git_commit_free(commit);
		git_repository_free(repo);
	}

	return new vertualfs::GitRepository(repo, commit, tree);
};

