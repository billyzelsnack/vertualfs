
#include "vertualfs/vertualfs.hpp"
#include "vertualfs/GitFilesystem.hpp"




static bool started = false;




bool vertualfs_startup()
{
	if(started){ return true; }

	if(!vertualfs::GitFilesystem_Startup()){return false;}
	started = true;

	return true;
}

void vertualfs_shutdown()
{
	if (!started) { return; }

	vertualfs::GitFilesystem_Shutdown();
}



