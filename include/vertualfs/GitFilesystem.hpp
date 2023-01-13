
#ifndef VERTUALFS_GITFILESYSTEM_HPP
#define VERTUALFS_GITFILESYSTEM_HPP


#include<filesystem>
#include<string>
#include<vector>


struct git_repository;
struct git_commit;
struct git_tree;

namespace vertualfs
{
	class  GitFilesystem;
}

class vertualfs::GitFilesystem
{

private:

	GitFilesystem( git_repository* repository, git_commit* commit );

public:

	~GitFilesystem();

public:

	bool listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing);
	bool repo_version(std::string& out_version) const;
	bool lookup_remote_url(const std::string& name, std::string& out_url) const;

	bool ls(std::vector<std::pair<std::string, bool>>& out_listing);
	bool cd(const std::filesystem::path& relativepath);

	static vertualfs::GitFilesystem* create(const std::string& path);

public:

	git_repository* repository = nullptr;
	git_commit* commit = nullptr;
	std::filesystem::path cwd = "/";

};

namespace vertualfs
{
	std::string GitFilesystem_CreateLocalPath(const std::string& url);
	void GitFilesystem_Shutdown();
	bool GitFilesystem_Startup();
}


#endif

