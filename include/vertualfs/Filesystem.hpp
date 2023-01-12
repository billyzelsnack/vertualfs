
#ifndef VERTUALFS_FILESYSTEM_HPP
#define VERTUALFS_FILESYSTEM_HPP


#include<filesystem>
#include<string>
#include<vector>


struct git_repository;
struct git_commit;
struct git_tree;

namespace vertualfs
{
	class  Filesystem;
}

class vertualfs::Filesystem
{

private:

	Filesystem( git_repository* repository, git_commit* commit );

public:

	~Filesystem();

public:

	bool listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing);
	bool repo_version(std::string& out_version) const;
	bool lookup_remote_url(const std::string& name, std::string& out_url) const;

	bool ls(std::vector<std::pair<std::string, bool>>& out_listing);
	bool cd(const std::filesystem::path& relativepath);

	static vertualfs::Filesystem* create(const std::string& path);

public:

	git_repository* repository = nullptr;
	git_commit* commit = nullptr;
	std::filesystem::path cwd = "/";

};

namespace vertualfs
{
	std::string Filesystem_CreateLocalPath(const std::string& url);
	void Filesystem_Shutdown();
	bool Filesystem_Startup();
}


#endif

