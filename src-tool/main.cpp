
#include <iostream>
#include <string>

#include <easylogging++.h>

#include <vertualfs.hpp>
#include "commandscli.hpp"

INITIALIZE_EASYLOGGINGPP



int main(int argc, const char** argv)
{
    LOG(INFO) << "vertualfstool startup";
    if (!vertualfs_startup()) { return false; }

    commands_process(argc, argv);
    commands_process(argv[0], "mount ensureadded c:/repos/headtest");
    commands_process(argv[0], "mount ensureadded https://github.com/billyzelsnack/simple_onshape_exportstls.git");
    commands_process(argv[0], "mount ensureadded https://gitlab.com/telemotor/users/billy/headtest");
    commands_process(argv[0], "help");

    while(1)
    {
        std::string line;
        std::getline(std::cin, line);

        if(!commands_process(argv[0],line)){ break; }
    }

    LOG(INFO) << "vertualfstool shutdown";
    vertualfs_shutdown();

    return EXIT_SUCCESS;
}

