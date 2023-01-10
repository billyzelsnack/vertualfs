

#include <lyra/arg.hpp>
#include <lyra/cli.hpp>
#include <lyra/command.hpp>
#include <lyra/opt.hpp>

#include "commands.hpp"
#include "commandscli.hpp"





struct command_create_volume
{
    std::string name;
    command_create_volume(lyra::cli& cli)
    {
        cli
        .add_argument(lyra::command("create", [this](const lyra::group& g) { return vertualfstool::create_volume(name); }).optional()
        .add_argument(lyra::command("volume").required())
        .add_argument(lyra::arg(name, "name").required()));
    }
};

struct command_ls_volumes
{
    command_ls_volumes(lyra::cli& cli)
    {
        cli
        .add_argument(lyra::command("ls", [this](const lyra::group& g) { return vertualfstool::ls_volumes(); }).optional()
        .add_argument(lyra::command("volumes").required()));
    }
};

struct command_cd_volume
{
    std::string indexorname;
    command_cd_volume(lyra::cli& cli)
    {
        cli
        .add_argument(lyra::command("cd").optional())
        .add_argument(lyra::command("volume", [this](const lyra::group& g) { return vertualfstool::cd_volume(indexorname); }).required()
        .add_argument(lyra::arg(indexorname, "indexorname").required()));
    }
};

struct command_ensureavailable_repository
{
    std::string url;
    std::string tag;
    command_ensureavailable_repository(lyra::cli& cli)
    {
        cli
        .add_argument
        ( 
            lyra::command("ensureavailable", [this](const lyra::group& g) { return vertualfstool::volume_ensureavailable_repository(url, tag); } )
            .add_argument
            (
                lyra::command("repository").required()
                .add_argument
                (
                    lyra::arg(url, "url").required()).add_argument(lyra::arg(tag, "tag").optional()
                )
            )
        );
    }
};


struct command_ls_repositories
{
    command_ls_repositories(lyra::cli& cli)
    {
        cli
        .add_argument
        (
            lyra::command("ls", [this](const lyra::group& g) { return vertualfstool::volume_ls_repositories(); })
            .add_argument
            (
                lyra::command("repositories").required()
            )
        );
    }
};



bool vertualfstool::commandscli(int argc, const char** argv, std::string& out_result)
{
    out_result.clear();

    auto cli = lyra::cli();
    cli.add_argument(lyra::command("help", [&](const lyra::group& g) { vertualfstool::help(""); }));
    cli.add_argument(lyra::command("exit", [&](const lyra::group& g) { vertualfstool::exit(); }));
    command_create_volume ca{ cli };
    command_ls_volumes    cb{ cli };
    command_cd_volume     cc{ cli };
    command_ensureavailable_repository cd{ cli };
    command_ls_repositories ce{ cli };
    //select_mount_command select_mount_cmd{ cli };
    //list_mounts_command mount_list_cmd{ cli };
    //mount_ensureadded_command mount_ensureadded_cmd{ cli };
    //mount_ls_command{ cli };

    auto result = cli.parse({ argc, argv });

    return true;
}

bool vertualfstool::commandscli(const char* argv0, const std::string& line, std::string& out_result)
{
    std::string unconstline = line;

    std::vector<const char*> lineargv; lineargv.push_back(argv0);
    bool ready = true;
    char* linedata = unconstline.data();
    for (int ii = 0; ii < unconstline.length(); ii++)
    {
        if (linedata[ii] != ' ') { if (ready) { lineargv.push_back(&linedata[ii]); ready = false; } }
        else { linedata[ii] = 0; ready = true; }
    }
    //for (int ii = 0; ii < lineargv.size(); ii++) { printf("[%d][%s]\n", ii, lineargv[ii]); }

    return commandscli((int)lineargv.size(), lineargv.data(), out_result);
}



