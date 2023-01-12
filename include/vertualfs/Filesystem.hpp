
#ifndef VERTUALFS_FILESYSTEM_HPP
#define VERTUALFS_FILESYSTEM_HPP


#include<filesystem>
#include<string>
#include<vector>


namespace vertualfs
{
	class  Filesystem;
	struct Repository;
}

class vertualfs::Filesystem
{

private:

	Filesystem(vertualfs::Repository* repository);

public:

	~Filesystem();

public:

	bool listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing);
	bool repo_version(std::string& out_version) const;
	bool lookup_remote_url(const std::string& name, std::string& out_url) const;

	bool ls(std::vector<std::pair<std::string, bool>>& out_listing);
	bool cd(const std::filesystem::path& relativepath);

	static vertualfs::Filesystem* create(vertualfs::Repository* repository);

public:

	vertualfs::Repository* repository = nullptr;
	std::filesystem::path cwd="/";

};


#endif

