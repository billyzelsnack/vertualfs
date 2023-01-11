
#include "vertualfs.hpp"

#include <cstdio>

#include "Repository.hpp"


static bool started = false;




bool vertualfs_startup()
{
	if(started){ return true; }

	if(!vertualfs::GitRepository_Startup()){return false;}
	started = true;

	return true;
}

void vertualfs_shutdown()
{
	if (!started) { return; }

	vertualfs::GitRepository_Shutdown();
}



