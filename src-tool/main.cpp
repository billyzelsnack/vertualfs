
#include <iostream>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>
#include <IconsFontAwesome4.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vertualfs/vertualfs.hpp>
#include <vertualfs/Volume.hpp>

#include "commands.hpp"
#include "commandscli.hpp"





extern vertualfs::Volume gvolume;




int main2(int argc, const char** argv)
{
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

    vertualfs_shutdown();

    return EXIT_SUCCESS;
}




std::string crumbsbrowser(const std::vector<std::string>& crumbs)
{
    std::string selection;

    ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ImColor(0, 64, 0).Value);
    int crumbflags = ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV;//ImGuiTableFlags_NoKeepColumnsVisible
    int numcols = 2 * int(crumbs.size());
    ImGui::AlignTextToFramePadding();
    if (ImGui::BeginTable("crumbs", numcols, crumbflags, ImVec2(ImGui::GetWindowWidth(), 0)))
    {
        for (int ii = 0; ii < numcols; ii++) { ImGui::TableSetupColumn(std::to_string(ii).c_str(), ImGuiTableColumnFlags_WidthFixed); }
        const float ROW_HEIGHT = ImGui::GetTextLineHeightWithSpacing() + 6;
        ImGui::TableNextRow(ImGuiTableRowFlags_None, ROW_HEIGHT);
        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImColor(0, 64, 0));

        for (int ii = 0; ii < crumbs.size(); ii++)
        {
            ImGui::TableSetColumnIndex(ii * 2);
            bool selected = false;
            ImGui::AlignTextToFramePadding();
            if (ImGui::Selectable(crumbs[ii].c_str(), &selected, ImGuiSelectableFlags_None)) { selection = crumbs[ii]; }
            ImGui::TableNextColumn();
            ImGui::Text("/");
        }

        ImGui::EndTable();
    }
    ImGui::PopStyleColor();

    return selection;
}

std::string listingbrowser(const std::vector<std::tuple<std::string,bool>>& listing)
{
    std::string selection;

    ImGuiTableFlags flags = ImGuiTableFlags_None;// ImGuiTableFlags_BordersInnerH;
    if (ImGui::BeginTable("maintable", 1, flags))
    {
        const float ROW_HEIGHT = ImGui::GetTextLineHeightWithSpacing() + 6;
        ImGui::TableNextRow(ImGuiTableRowFlags_None, ROW_HEIGHT);
        ImGui::TableSetColumnIndex(0);

        for (auto& [name, isfolder] : listing)
        {
            std::string label = name;
            if (isfolder) { label = std::string(ICON_FA_FOLDER) + " " + name; }
            bool selected = false;
            ImGui::AlignTextToFramePadding();
            if(ImGui::Selectable(label.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
            {
                selection = name;
            }
        }

        ImGui::EndTable();
    }

    return selection;
}




void volumebrowser(vertualfs::Volume* volume)
{
    std::vector<std::string> crumbs;
    crumbs.push_back("avertualfs");
    crumbs.push_back("binclude");
    crumbs.push_back("bvertualfs");
    std::string crumbselected = crumbsbrowser(crumbs);
    if (!crumbselected.empty())
    {
        printf("crumbselected[%s]\n", crumbselected.c_str());
    }

    std::string listingselected = "";
    {
        std::vector<std::tuple<std::string, bool>> listing;
        listing.push_back({ "include",true });
        listing.push_back({ "src",true });
        listing.push_back({ "src-tool",true });
        listingselected += listingbrowser(listing);
    }
    {
        std::vector<std::tuple<std::string, bool>> listing;
        listing.push_back({ "LICENSE.txt",false });
        listing.push_back({ "premake5.lua",false });
        listingselected += listingbrowser(listing);
    }
    if (!listingselected.empty())
    {
        printf("listingselected[%s]\n", listingselected.c_str());
    }

}















static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit()) { return 1; }

#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL) { return 1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    io.Fonts->AddFontDefault();
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    {
        ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = false; icons_config.GlyphOffset = ImVec2(0, 2);
        io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FA, 16.0f, &icons_config, icons_ranges);
    }

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        std::string title="Volume ["+gvolume.root.string()+"]";
        ImGui::Begin(title.c_str());
        
        volumebrowser(&gvolume);
        
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}














