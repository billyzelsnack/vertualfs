
#include "vertualfs.hpp"

#include <cstdio>
#include <easylogging++.h>

#include "GitFileSystem.hpp"


static bool started = false;




bool vertualfs_startup()
{
	if(started){ return true; }
	LOG(INFO) << "vertualfs_startup";

	if(!GitFileSystem_startup()){return false;}
	started = true;

	return true;
}

void vertualfs_shutdown()
{
	if (!started) { return; }
	LOG(INFO) << "vertualfs_shutdown";

	GitFileSystem_shutdown();
}



