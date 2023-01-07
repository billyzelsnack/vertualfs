
#include <cstdio>

#include "git.hpp"
#include "vertualfs.hpp"



bool vertualfs_startup()
{
	printf("vertualfs_startup\n");
	if(!git_startup()){return false;}


	return true;
}

void vertualfs_shutdown()
{
	printf("vertualfs_shutdown\n");

	git_shutdown();
}









