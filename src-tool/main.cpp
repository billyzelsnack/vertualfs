
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <vertualfs.hpp>
#include <cxxopts.hpp>



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

int main(int argc, char** argv)
{
    cxxopts::Options options( "vertualfstool", "vertualfstool description");
    options.add_options()
    ("m,mount", "mount", cxxopts::value<std::string>())
    ("l,list", "list", cxxopts::value<bool>()->default_value("false"))
    ("h,help", "Print usage")
    ;
    options.custom_help("<--mount <localpath>> [OPTION...]");
    auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        printf("%s", options.help().c_str());
        return EXIT_SUCCESS;
    }

    std::string mount = "c:/repos/flatbuffers";
    if (result.count("mount")) { mount = result["mount"].as<std::string>(); }
    if (mount.empty()) { return EXIT_SUCCESS; }


    printf("vertualfstool\n");

    if(!vertualfstool_startup()){return EXIT_FAILURE;}


    vertualfs::FileSystem* fs = vertualfs::FileSystem::create(mount);
    if(fs!=nullptr)
    {
        if (result["list"].as<bool>())
        {
            std::vector<std::pair<std::string, bool>> listing;
            if (fs->list(listing))
            {
            }
        }
    }


    vertualfstool_shutdown();

    return EXIT_SUCCESS;
}













