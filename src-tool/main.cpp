
#include <iostream>
#include <string>

#include <easylogging++.h>

#include <vertualfs.hpp>
#include "commands.hpp"
#include "commandscli.hpp"

INITIALIZE_EASYLOGGINGPP



int main(int argc, const char** argv)
{
    LOG(INFO) << "vertualfstool startup";
    if (!vertualfs_startup()) { return false; }

    //vertualfstool::cli(argc, argv);
    //vertualfstool::cli(argv[0], "mount ensureadded c:/repos/headtest");
    //vertualfstool::cli(argv[0], "mount ensureadded https://github.com/billyzelsnack/simple_onshape_exportstls.git");
    //vertualfstool::cli(argv[0], "mount ensureadded https://gitlab.com/telemotor/users/billy/headtest");
    //vertualfstool::cli(argv[0], "help");

    while(!vertualfstool::exited())
    {
        std::string line;
        std::getline(std::cin, line);

        if (!vertualfstool::cli(argv[0], line)) { printf("cli error\n"); }
    }

    LOG(INFO) << "vertualfstool shutdown";
    vertualfs_shutdown();

    return EXIT_SUCCESS;
}

