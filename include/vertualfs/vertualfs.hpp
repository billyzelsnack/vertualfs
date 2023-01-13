

#ifndef VERTUALFS_HPP
#define VERTUALFS_HPP

#include<filesystem>

namespace vertualfs
{
	struct Vertualfs;
}


namespace vertualfs
{
	std::filesystem::path make_preferred(const std::filesystem::path& path);
	void shutdown();
	bool startup();
}










#endif