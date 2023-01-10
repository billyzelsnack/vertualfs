
#ifndef VERTUALFS_GITFILESYSTEM_HPP
#define VERTUALFS_GITFILESYSTEM_HPP


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

	GitFilesystem(GitRepository* repository);

public:

	~GitFilesystem();

public:

	bool listing(const std::string& path, std::vector<std::pair<std::string, bool>>& out_files);
	bool repo_version(std::string& out_version) const;
	bool lookup_remote_url(const std::string& name, std::string& out_url) const;

	static GitFilesystem* create(vertualfs::GitRepository* repository);

public:

	vertualfs::GitRepository* repository = nullptr;

};


#endif

