
#ifndef VERTUALFS_GITAPIFILESYSTEM_HPP
#define VERTUALFS_GITAPIFILESYSTEM_HPP

#include <vertualfs/Filesystem.hpp>
#include <nlohmann/json.hpp>

using json_t = nlohmann::json;



namespace vertualfs
{
	class  GitApiFilesystem;
}


class vertualfs::GitApiFilesystem : public vertualfs::Filesystem
{

private:

	GitApiFilesystem(const std::filesystem::path& url);

public:

	~GitApiFilesystem();

public:

	virtual bool cd(const std::filesystem::path& relativepath) override;
	virtual bool listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing) override;
	virtual bool ls(std::vector<std::pair<std::string, bool>>& out_listing) override;

	static vertualfs::GitApiFilesystem* create(const std::filesystem::path& url);

private:

	std::filesystem::path url;
	std::filesystem::path cwd;
	json_t cwdjson;

};


#endif

