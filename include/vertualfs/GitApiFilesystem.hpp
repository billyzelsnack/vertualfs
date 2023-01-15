
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

	GitApiFilesystem(const std::filesystem::path& baseurl);

public:

	virtual ~GitApiFilesystem();

public:

	virtual std::filesystem::path baseurl() const override;
	virtual std::filesystem::path cwd() const override;
	virtual bool cd(const std::filesystem::path& path) override;
	virtual bool listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing) override;
	virtual bool ls(std::vector<std::pair<std::string, bool>>& out_listing) override;
	virtual bool lookupurl(const std::filesystem::path& path, std::filesystem::path& out_url) const override;

	static vertualfs::GitApiFilesystem* create(const std::filesystem::path& baseurl);

private:

	std::filesystem::path fsbaseurl;
	std::filesystem::path fscwd;
	json_t fscwdjson;

};


#endif

