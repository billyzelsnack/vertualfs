
#include "vertualfs/vertualfs.hpp"
#include "vertualfs/GitFilesystem.hpp"




static bool started = false;



std::filesystem::path vertualfs::make_preferred(const std::filesystem::path& path)
{
	std::string spath = path.string();
	std::replace(spath.begin(), spath.end(), '\\', '/');
	return std::filesystem::path(spath);
}

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

