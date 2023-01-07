
#include <cstdio>
#include <cstdlib>

#include <vertualfs.hpp>





bool vertualfstool_startup()
{
    printf("vertualfstool startup\n");

    if (!vertualfs_startup()) { return false; }


    return true;
}

void vertualfstool_shutdown()
{
    printf("vertualfstool shutdown\n");

    vertualfs_shutdown();
}



int main(int argc, char** argv[])
{
    printf("vertualfstool\n");

    if(!vertualfstool_startup()){return EXIT_FAILURE;}


    vertualfstool_shutdown();

    return EXIT_SUCCESS;
}





