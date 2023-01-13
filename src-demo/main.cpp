
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <GLFW/glfw3.h>
#include <IconsFontAwesome4.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vertualfs/GitFilesystem.hpp>
#include <vertualfs/vertualfs.hpp>


std::unordered_map<std::filesystem::path,std::string> gselectedfiles;



bool fileviewer(const std::filesystem::path& localpath)
{
    //ImGui::Text("[%s]", localpath.string().c_str());
    std::ifstream file(localpath);

    if (!file.good()) { return false; }
    file.seekg(0, std::ios::end);
    std::streamoff filesize = file.tellg();
    file.seekg(0, std::ios::beg);
    if (filesize > 10000) { ImGui::Text("[filesize %zd>5000]", filesize); return false; }
    if (!file.good()) { return false; }
    std::string extension = localpath.extension().string();   
    if ( //-- silliness
        (extension.empty()) ||
        (extension == ".h") ||
        (extension == ".c") ||
        (extension == ".md") ||
        (extension == ".js") ||
        (extension == ".hpp") ||
        (extension == ".cpp") ||
        (extension == ".lua") ||
        (extension == ".txt") ||
        (extension == ".html") ||
        (extension == ".json")
        )
    {
        std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        ImGui::TextUnformatted(text.c_str(), text.c_str() + text.size());
    }
    else
    {
        ImGui::Text("[unknown file extension]");
    }

    file.close();

    return true;
}

std::string crumbsbrowser(const std::string& rootname, const std::vector<std::string>& crumbs)
{
    if (crumbs.empty()) { return ""; }

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
            std::string name = crumbs[ii];
            if(name=="/"){name=rootname;}
            if (ImGui::Selectable(name.c_str(), &selected, ImGuiSelectableFlags_None)) { selection = crumbs[ii]; }
            ImGui::TableNextColumn();
            ImGui::Text("/");
        }

        ImGui::EndTable();
    }
    ImGui::PopStyleColor();

    return selection;
}

std::string listingbrowser(const std::vector<std::tuple<std::string,bool>>& listing, const std::string& selectedlisting)
{
    if(listing.empty()) { return ""; }

    std::string selection;

    ImGuiTableFlags flags = ImGuiTableFlags_None;// | ImGuiTableFlags_BordersInnerH;
    if (ImGui::BeginTable("maintable", 1, flags))
    {
        const float ROW_HEIGHT = ImGui::GetTextLineHeightWithSpacing() + 6;
        ImGui::TableNextRow(ImGuiTableRowFlags_None, ROW_HEIGHT);
        ImGui::TableSetColumnIndex(0);

        for (auto& [name, isfolder] : listing)
        {
            bool selected = selectedlisting == name;
            std::string label = name;
            if (isfolder) { label = std::string(ICON_FA_FOLDER) + " " + name; }
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

//-- rootname should be computed and not passed in
void filesystembrowser(const std::string& rootname, vertualfs::GitFilesystem* filesystem)
{
    std::vector<std::pair<std::string, bool>> listing;
    filesystem->ls(listing);

    std::vector<std::string> crumbs;
    for (const auto& subpath : filesystem->cwd){ crumbs.push_back(subpath.string().c_str()); }
    std::string crumbselected = crumbsbrowser(rootname,crumbs);
    if(!crumbselected.empty())
    {
        std::filesystem::path cwd;
        for (const auto& subpath : filesystem->cwd)
        {
            cwd = cwd/subpath.string();
            if (subpath.string() == crumbselected) { break; }
        }

        printf("crumbselected[%s][%s]\n", crumbselected.c_str(),cwd.string().c_str());
        filesystem->cwd = cwd;
    }

    std::string fileselected;
    if(gselectedfiles.count(filesystem->cwd) > 0){ fileselected = gselectedfiles[filesystem->cwd]; }

    std::vector<std::tuple<std::string, bool>> folderslisting;
    for (auto& [name, isfolder] : listing) {if(isfolder){ folderslisting.push_back({ name,isfolder }); }}
    std::string folderselected = listingbrowser(folderslisting,"");

    std::vector<std::tuple<std::string, bool>> fileslisting;
    for (auto& [name, isfolder] : listing){if(!isfolder){ fileslisting.push_back({ name,isfolder }); }}
    fileselected=listingbrowser(fileslisting, fileselected);
    
    if(!folderselected.empty())
    {
        filesystem->cd(folderselected);
    }

    //ImGui::Text("cwd[%s]", volume->filesystem->cwd.string().c_str());

    if(!fileselected.empty())
    {
        if(gselectedfiles.count(filesystem->cwd)>0 && gselectedfiles[filesystem->cwd]==fileselected)
        {
            gselectedfiles.erase(filesystem->cwd);
        }
        else
        {
            gselectedfiles[filesystem->cwd] = fileselected;
        }        
    }

    if(gselectedfiles.count(filesystem->cwd) > 0) 
    { 
        std::string remoteurl;
        if(filesystem->lookup_remote_url("origin", remoteurl))
        {
            std::string localpath = filesystem->cwd.string();
            localpath=vertualfs::GitFilesystem_CreateLocalPath(remoteurl)+localpath+"/"+gselectedfiles[filesystem->cwd];
            //printf("[%s]\n", localpath.c_str());
            ImGui::Separator();
            fileviewer(localpath);
        }
    }

}





int mainwin(int argc, const char**, GLFWwindow* window)
{
    if (!vertualfs_startup()) { return EXIT_FAILURE; }

    std::filesystem::path repositoryurl = "https://gitlab.com/telemotor/users/billy/headtest";
    vertualfs::GitFilesystem* filesystem=vertualfs::GitFilesystem::create(repositoryurl.string());
    std::string nextrepositoryurl = repositoryurl.string();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("volumebrowser", nullptr, ImGuiWindowFlags_NoScrollbar);        
        ImGui::BeginChild("Scrolling Region", ImVec2(0, ImGui::GetWindowHeight()-60), false, ImGuiWindowFlags_NoResize);
        if(filesystem!= nullptr)
        {
            std::filesystem::path urlcopy = repositoryurl;
            ImGui::Text((urlcopy.replace_filename("")).string().c_str());
            filesystembrowser(repositoryurl.stem().string(), filesystem);
        }
        ImGui::EndChild();
        ImGui::Text("repository:"); ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        char buffer[1024]; strcpy_s(buffer, 1024, repositoryurl.string().c_str());
        if (ImGui::InputText("", buffer, 1024, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            nextrepositoryurl = buffer;
            repositoryurl = nextrepositoryurl;
            delete filesystem;
            filesystem = vertualfs::GitFilesystem::create(repositoryurl.string());
        }
        ImGui::PopItemWidth();
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

    vertualfs_shutdown();
    return EXIT_SUCCESS;
}

void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int argc, const char** argv)
{
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit()) { return EXIT_FAILURE; }

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

    GLFWwindow* window = glfwCreateWindow(1280, 720, "virtualfsdemo", NULL, NULL);
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

    int result = mainwin(argc, argv, window);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return result;
}














