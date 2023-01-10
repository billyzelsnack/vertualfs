

#include <lyra/arg.hpp>
#include <lyra/cli.hpp>
#include <lyra/command.hpp>
#include <lyra/opt.hpp>

#include "commands.hpp"
#include "commandscli.hpp"




/*
volume
{
    repo{ }
    repo{ }
    filesystem{ }
    filesystem{ }
}
*/






struct list_mounts_command
{
    list_mounts_command(lyra::cli& cli)
    {
        cli.add_argument
        (
            lyra::command("list")
        )
            .add_argument
            (
                lyra::command("mounts", [this](const lyra::group& g) {this->command(g); })
            );
    }

    void command(const lyra::group& g)
    {
        list_mounts();
    }

};

struct mount_ensureadded_command
{
    std::string path;

    mount_ensureadded_command(lyra::cli& cli)
    {
        cli.add_argument
        (
            lyra::command("mount")
        )
            .add_argument
            (
                lyra::command("ensureadded", [this](const lyra::group& g) { this->command(g); })
                .add_argument
                (
                    lyra::arg(path, "path").required()
                )
            );
    }

    void command(const lyra::group& g)
    {
        mount_ensureadded(path);
    }

};



struct select_mount_command
{
    int index = 0;

    select_mount_command(lyra::cli& cli)
    {
        cli.add_argument
        (
            lyra::command("select")
        )
            .add_argument
            (
                lyra::command("mount", [this](const lyra::group& g) { this->command(g); })
                .add_argument
                (
                    lyra::arg(index, "index").required()
                )
            );
    }

    void command(const lyra::group& g)
    {
        if (select_mount(index))
        {
            printf("selected mount:%d\n", index);
        }
    }

};






struct mount_ls_command
{
    int index = 0;

    mount_ls_command(lyra::cli& cli)
    {
        cli.add_argument
        (
            lyra::command("mount")
        )
            .add_argument
            (
                lyra::command("ls", [this](const lyra::group& g) { this->command(g); })
            );
    }

    void command(const lyra::group& g)
    {
        mount_ls();
    }

};



static void showhelp()
{
    printf("help\n");
    printf("list mounts\n");
    printf("mount ensureadded <path>\n");
    printf("select mount <index>\n");
    printf("mount ls\n");
    printf("mount cd <relative directory>\n");
}


bool commands_process(int argc, const char** argv)
{
    bool noexit = true;
    
    auto cli = lyra::cli();
    cli.add_argument(lyra::command("help", [&](const lyra::group& g) { showhelp(); }));
    cli.add_argument(lyra::command("exit", [&](const lyra::group& g) { noexit = false; }));
    select_mount_command select_mount_cmd{ cli };
    list_mounts_command mount_list_cmd{ cli };
    mount_ensureadded_command mount_ensureadded_cmd{ cli };
    mount_ls_command{ cli };

    auto result = cli.parse({ argc, argv });

    return noexit;
}

bool commands_process(const char* argv0, const std::string& line)
{
    std::string aline = line;

    std::vector<const char*> lineargv; lineargv.push_back(argv0);
    bool ready = true;
    char* linedata = aline.data();
    for (int ii = 0; ii < aline.length(); ii++)
    {
        if (linedata[ii] != ' ') { if (ready) { lineargv.push_back(&linedata[ii]); ready = false; } }
        else { linedata[ii] = 0; ready = true; }
    }
    //for (int ii = 0; ii < lineargv.size(); ii++) { printf("[%d][%s]\n", ii, lineargv[ii]); }

    return commands_process((int)lineargv.size(), lineargv.data());
}



