
#include "vertualfs/vertualfs.hpp"
#include "vertualfs/Repository.hpp"
#include <cstdio>



static bool started = false;




bool vertualfs_startup()
{
	if(started){ return true; }

	if(!vertualfs::Repository_Startup()){return false;}
	started = true;

	return true;
}

void vertualfs_shutdown()
{
	if (!started) { return; }

	vertualfs::Repository_Shutdown();
}



