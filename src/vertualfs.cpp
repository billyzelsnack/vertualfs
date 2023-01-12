
#include "vertualfs/vertualfs.hpp"
#include "vertualfs/Filesystem.hpp"




static bool started = false;




bool vertualfs_startup()
{
	if(started){ return true; }

	if(!vertualfs::Filesystem_Startup()){return false;}
	started = true;

	return true;
}

void vertualfs_shutdown()
{
	if (!started) { return; }

	vertualfs::Filesystem_Shutdown();
}



