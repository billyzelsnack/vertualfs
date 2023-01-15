
#include "vertualfs/vertualfs.hpp"
#include "vertualfs/GitFilesystem.hpp"

#include <iostream>




static bool started = false;


std::filesystem::path vertualfs::make_preferred(const std::filesystem::path& path)
{
	std::string spath = path.string();
	std::replace(spath.begin(), spath.end(), '\\', '/');
	return std::filesystem::path(spath);
}

std::string vertualfs::make_string(const std::vector<std::string>& path)
{
	std::string ss;
	for (auto& vv : path) { ss.append(vv); ss.append("/"); }
	if (!ss.empty()) { ss.pop_back(); }
	return ss;
}

std::vector<std::string> vertualfs::make_pathvector(const std::filesystem::path& path)
{
	std::vector<std::string> vec;
    for (const auto& entry : path)
    {
        std::string name = entry.relative_path().string();
        vec.push_back(name);
    }
    return vec;
}

void vertualfs::print_path(const std::filesystem::path& path)
{
	std::cout << "Protocol(root_name): " << path.root_name().string() << std::endl;
	std::cout << "Domain(root_directory): " << path.root_directory().string() << std::endl;
	std::cout << "Path(relative_path): " << path.relative_path() << std::endl;
	std::cout << "Filename(filename): " << path.filename() << std::endl;
	std::cout << "Filename(stem): " << path.stem() << std::endl;
	std::cout << "Filename(extension): " << path.extension() << std::endl;
}

/*
std::string vertualfs::extension(const std::vector<std::string>& path)
{
	std::filesystem::path fspath = vertualfs::make_filesystem(path);	
	return fspath.extension().string();
}

bool vertualfs::same_path(const std::vector<std::string>& patha, const std::vector<std::string>& pathb)
{
	printf("same_path not implemented\n");
	return false;
}
*/

std::filesystem::path vertualfs::make_path(const std::vector<std::string>& pathvec)
{
	std::filesystem::path path;
	for (auto& subpath : pathvec){ path.append(subpath); }
	return path;
}

/*
std::vector<std::string> vertualfs::append_path(const std::vector<std::string>& patha, const std::vector<std::string>& pathb)
{
	std::vector<std::string> pathc = patha;
	pathc.insert(pathc.end(), pathb.begin(), pathb.end());
	return pathc;
}
*/

/*
bool vertualfs::filesystem_subpath(const std::filesystem::path& path, size_t subpathindex, std::string& out_subpath)
{
	out_subpath = "";
	int index = 0;
	for(const auto& subpath : path)
	{ 
		if (index == subpathindex) 
		{ 
			out_subpath = path.string();
			return true;
		} 
		index++; 
	}

	return false;
}
*/



void vertualfs::shutdown()
{
	printf("shutdown\n");
	if (!started) { return; }

	vertualfs::GitFilesystem_Shutdown();
}

bool vertualfs::startup()
{
	printf("startup\n");
	if(started){ return true; }

	if(!vertualfs::GitFilesystem_Startup()){return false;}
	started = true;

	return true;
}

