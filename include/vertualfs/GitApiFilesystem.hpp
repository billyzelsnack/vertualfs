
#ifndef VERTUALFS_GITAPIFILESYSTEM_HPP
#define VERTUALFS_GITAPIFILESYSTEM_HPP

#include <vertualfs/Filesystem.hpp>


namespace vertualfs
{
	class  GitApiFilesystem;
}


class vertualfs::GitApiFilesystem : public vertualfs::Filesystem
{

public:

	virtual bool cd(const std::filesystem::path& relativepath) override;
	virtual bool listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing) override;
	virtual bool ls(std::vector<std::pair<std::string, bool>>& out_listing) override;

};






#endif

