
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

class vertualfs::GitFilesystem : public vertualfs::Filesystem
{

private:

	GitFilesystem( const std::filesystem::path& baseurl, git_repository* repository, git_commit* commit );

public:

	virtual ~GitFilesystem();

public:
	
	virtual std::filesystem::path baseurl() const override;
	virtual std::filesystem::path cwd() const override;
	virtual bool cd(const std::filesystem::path& path) override;
	virtual bool listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing) override;
	virtual bool ls(std::vector<std::pair<std::string, bool>>& out_listing) override;
	virtual bool lookupurl(const std::filesystem::path& path, std::filesystem::path& out_url) const override;

public:

	bool repo_version(std::string& out_version) const;
	bool lookup_remote_url(const std::string& name, std::filesystem::path& out_url) const;

public:

	static vertualfs::GitFilesystem* create(const std::filesystem::path& baseurl);

protected:

	std::filesystem::path fsbaseurl;
	git_repository* repository = nullptr;
	git_commit* commit = nullptr;
	std::filesystem::path fscwd;

};

namespace vertualfs
{
	void GitFilesystem_Shutdown();
	bool GitFilesystem_Startup();
}


#endif

