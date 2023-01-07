
#include <cstdio>
#include <cstdlib>

#include <vertualfs.hpp>










int main(int argc, char** argv[])
{
    printf("vertualfstool\n");

    if(!vertualfs_startup()){return EXIT_FAILURE;}


    vertualfs_shutdown();

    return EXIT_SUCCESS;
}