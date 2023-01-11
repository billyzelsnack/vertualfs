
#ifndef VERTUALFS_GITFILESYSTEM_HPP
#define VERTUALFS_GITFILESYSTEM_HPP


#include<filesystem>
#include<string>
#include<vector>

namespace vertualfs
{
	class  GitFilesystem;
	struct GitRepository;
}

class vertualfs::GitFilesystem
{

private:

	GitFilesystem(vertualfs::GitRepository* repository);

public:

	~GitFilesystem();

public:

	bool listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing);
	bool repo_version(std::string& out_version) const;
	bool lookup_remote_url(const std::string& name, std::string& out_url) const;

	bool ls(std::vector<std::pair<std::string, bool>>& out_listing);
	bool cd(const std::filesystem::path& relativepath);

	static vertualfs::GitFilesystem* create(vertualfs::GitRepository* repository);

public:

	vertualfs::GitRepository* repository = nullptr;
	std::filesystem::path cwd="/";

};


#endif

