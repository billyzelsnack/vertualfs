

#ifndef VERTUALFS_HPP
#define VERTUALFS_HPP

#include<filesystem>



namespace vertualfs
{
	std::filesystem::path make_preferred(const std::filesystem::path& path);
	std::string make_string(const std::vector<std::string>& path);
	std::vector<std::string> make_pathvector(const std::filesystem::path& path);
	std::filesystem::path make_path(const std::vector<std::string>& pathvector);
	void print_path(const std::filesystem::path& path);

	void shutdown();
	bool startup();
}










#endif