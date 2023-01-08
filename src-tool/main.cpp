
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <easylogging++.h>

//#include <lyra/lyra.hpp>
#include <lyra/cli.hpp>
#include <lyra/opt.hpp>

#include <vertualfs.hpp>


INITIALIZE_EASYLOGGINGPP


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
    LOG(INFO) << "vertualfs";

    std::string mount = "c:/repos/flatbuffers";

    bool help = false;
    bool listing=false;
    auto cli = lyra::cli()
        | lyra::opt(help)["-h"]["--help"]("display help")
        | lyra::opt(listing) ["-l"]["--listing"]("display listing")
        | lyra::opt(mount,"mount")["-m"]["--mount"]("set mount");    
    auto result = cli.parse({ argc, argv });
    if(!result)
    {
        std::cerr << "Error in command line: " << result.message() << std::endl;
        std::cout << "help = " << (help ? "true" : "false") << "\n";
        std::cout << "listing = " << (listing ? "true" : "false") << "\n";
        std::cout << "mount = " << mount << "\n";
        return EXIT_FAILURE;
    }
 
    if (mount.empty()) { return EXIT_FAILURE; }

    
    printf("vertualfstool\n");
    if(!vertualfstool_startup()){return EXIT_FAILURE;}
    vertualfs::FileSystem* fs = vertualfs::FileSystem::create(mount);
    if(fs!=nullptr)
    {
        if (listing)
        {
            std::vector<std::pair<std::string, bool>> listing;
            if (fs->listing(listing))
            {
            }
        }
    }
    vertualfstool_shutdown();

    return EXIT_SUCCESS;
}













