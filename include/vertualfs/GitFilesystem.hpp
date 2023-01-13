
#ifndef VERTUALFS_GITFILESYSTEM_HPP
#define VERTUALFS_GITFILESYSTEM_HPP

#include "Filesystem.hpp"

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

class vertualfs::GitFilesystem : public Filesystem
{

private:

	GitFilesystem( git_repository* repository, git_commit* commit );

public:

	~GitFilesystem();

public:

	bool cd(const std::filesystem::path& relativepath) override;
	bool listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing) override;
	bool ls(std::vector<std::pair<std::string, bool>>& out_listing) override;

public:

	bool repo_version(std::string& out_version) const;
	bool lookup_remote_url(const std::string& name, std::string& out_url) const;

public:

	static vertualfs::GitFilesystem* create(const std::string& path);

public: //-- todo: make accessors so these can be private

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

