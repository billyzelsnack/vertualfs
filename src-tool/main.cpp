
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

    std::string cliresult;
    vertualfstool::commandscli(argc, argv, cliresult);
    vertualfstool::commandscli(argv[0], "help", cliresult);
    vertualfstool::commandscli(argv[0], "create volume nachos", cliresult);
    vertualfstool::commandscli(argv[0], "ls volumes", cliresult);
    vertualfstool::commandscli(argv[0], "cd volume nachos", cliresult);
    vertualfstool::commandscli(argv[0], "ensureavailable repository c:/repos/headtest tag", cliresult);
    vertualfstool::commandscli(argv[0], "ensureavailable repository https://github.com/billyzelsnack/simple_onshape_exportstls.git tag", cliresult);
    vertualfstool::commandscli(argv[0], "ensureavailable repository https://gitlab.com/telemotor/users/billy/headtest tag", cliresult);
    vertualfstool::commandscli(argv[0], "ensureavailable repository c:/repos/headtest tag", cliresult);
    vertualfstool::commandscli(argv[0], "ls repositories", cliresult);

    while(!vertualfstool::exited())
    {
        std::cout << vertualfstool::prompt() << std::flush;
        std::string line;
        std::getline(std::cin, line);

        if(!vertualfstool::commandscli(argv[0], line, cliresult))
        { 
            printf("cliresult [%s]\n",cliresult.c_str()); 
        }
    }

    LOG(INFO) << "vertualfstool shutdown";
    vertualfs_shutdown();

    return EXIT_SUCCESS;
}

