
#ifndef VERTUALFS_OPENAIFILESYSTEM_HPP
#define VERTUALFS_OPENAIFILESYSTEM_HPP

#include "vertualfs/Filesystem.hpp"

#include <nlohmann/json.hpp>

using json_t = nlohmann::json;



namespace vertualfs
{
	class  OpenaiFilesystem;
}


class vertualfs::OpenaiFilesystem : public vertualfs::Filesystem
{

private:

	OpenaiFilesystem(const std::string& authorization, const std::filesystem::path& baseurl);

public:

	virtual ~OpenaiFilesystem();

public:

	virtual std::filesystem::path baseurl() const override;
	virtual std::filesystem::path cwd() const override;
	virtual bool cd(const std::filesystem::path& path) override;
	virtual bool listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing) override;
	virtual bool ls(std::vector<std::pair<std::string, bool>>& out_listing) override;
	virtual bool lookupurl(const std::filesystem::path& path, std::filesystem::path& out_url) const override;

	static vertualfs::OpenaiFilesystem* create(const std::string& secret_key, const std::filesystem::path& baseurl);

private:

	std::string authorization;
	std::filesystem::path fsbaseurl;
	std::filesystem::path fscwd;
	json_t fscwdjson;

};


#endif

