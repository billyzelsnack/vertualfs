
#ifndef VERTUALFS_FILESYSTEM_HPP
#define VERTUALFS_FILESYSTEM_HPP


#include<filesystem>
#include<string>
#include<vector>


namespace vertualfs
{
	class  Filesystem;
}

class vertualfs::Filesystem
{

public:

	//-- todo: can it be reasonably be assumed that all implementations would be able to const these?
	virtual bool cd(const std::filesystem::path& relativepath)=0;
	virtual bool listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing)=0;
	virtual bool ls(std::vector<std::pair<std::string, bool>>& out_listing)=0;

};


#endif

