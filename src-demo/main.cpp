
#include "windowing.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <IconsFontAwesome4.h>
#include <imgui.h>
#include <vertualfs/Filesystem.hpp>
#include <vertualfs/GitApiFilesystem.hpp>
#include <vertualfs/GitFilesystem.hpp>
#include <vertualfs/vertualfs.hpp>

//-- todo: ensure that paths and urls are using std::filesystem::url instead and not std::string

std::unordered_map<std::filesystem::path,std::string> gselectedfiles;



bool fileviewer(const std::filesystem::path& url)
{
    //ImGui::Text("[%s]", localpath.string().c_str());
    std::ifstream file(url);

    if (!file.good()) { return false; }
    file.seekg(0, std::ios::end);
    std::streamoff filesize = file.tellg();
    file.seekg(0, std::ios::beg);
    if (filesize > 10000) { ImGui::Text("[filesize %zd>5000]", filesize); return false; }
    if (!file.good()) { return false; }
    std::string extension = url.extension().string();
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

std::vector<std::string> crumbsbrowser(std::vector<std::string>& crumbspathvec)
{
    std::vector<std::string> selectedcrumbspathvec;
        
    bool foundselected = false;
    ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ImColor(0, 64, 0).Value);
    ImGui::AlignTextToFramePadding();
    int crumbsflags = ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV;//ImGuiTableFlags_NoKeepColumnsVisible
    if (ImGui::BeginTable("crumbs", int(crumbspathvec.size()*2), crumbsflags, ImVec2(ImGui::GetWindowWidth(), 0)))
    {
        for (size_t ii = 0; ii < (crumbspathvec.size()*2); ii++) { ImGui::TableSetupColumn(std::to_string(ii).c_str(), ImGuiTableColumnFlags_WidthFixed); }
        const float ROW_HEIGHT = ImGui::GetTextLineHeightWithSpacing() + 6;
        ImGui::TableNextRow(ImGuiTableRowFlags_None, ROW_HEIGHT);
        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImColor(0, 64, 0));

        for(size_t ii=0; ii< crumbspathvec.size(); ii++)
        {
            ImGui::TableSetColumnIndex(int(ii * 2));
            bool selected = false;
            ImGui::AlignTextToFramePadding();            
            if(!foundselected){ selectedcrumbspathvec.push_back(crumbspathvec[ii]); }
            if(ImGui::Selectable(crumbspathvec[ii].c_str(), &selected, ImGuiSelectableFlags_None)){ foundselected = true; }
            ImGui::TableNextColumn();
            ImGui::Text("/");
        }

        ImGui::EndTable();
    }
    ImGui::PopStyleColor();

    if (!foundselected) { selectedcrumbspathvec.clear(); }
    return selectedcrumbspathvec;
}

std::string listingbrowser(const std::vector<std::tuple<std::string,bool>>& listing, std::string& selectedlisting)
{
    if (listing.empty()) { return ""; }

    std::string selection;

    ImGuiTableFlags flags = ImGuiTableFlags_None;// | ImGuiTableFlags_BordersInnerH;
    if (ImGui::BeginTable("maintable", 1, flags))
    {
        const float ROW_HEIGHT = ImGui::GetTextLineHeightWithSpacing() + 6;
        ImGui::TableNextRow(ImGuiTableRowFlags_None, ROW_HEIGHT);
        ImGui::TableSetColumnIndex(0);

        for (auto& [name, isfolder] : listing)
        {
            bool selected = selectedlisting==name;
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

void filesystembrowser(vertualfs::Filesystem* filesystem)
{
    if (filesystem == nullptr){ return; }

    std::string rootname = filesystem->baseurl().stem().string();

    std::vector<std::pair<std::string, bool>> listing;
    if (!filesystem->ls(listing)){ return; }
  
    std::vector<std::string> crumbspathvec = vertualfs::make_pathvector(filesystem->cwd().relative_path());
    crumbspathvec.insert(crumbspathvec.begin(), rootname);

    std::vector<std::string> selectedcrumbspathvec = crumbsbrowser(crumbspathvec);

    std::string foldersselected;
    std::string fileselected;
    if( gselectedfiles.count(filesystem->cwd()) >0)
    { 
        fileselected = gselectedfiles[filesystem->cwd()];
    }

    std::vector<std::tuple<std::string, bool>> folderslisting;
    for (auto& [name, isfolder] : listing) {if(isfolder){ folderslisting.push_back({ name,isfolder }); }}
    std::string folderselected = listingbrowser(folderslisting,foldersselected);

    std::vector<std::tuple<std::string, bool>> fileslisting;
    for (auto& [name, isfolder] : listing){if(!isfolder){ fileslisting.push_back({ name,isfolder }); }}
    fileselected=listingbrowser(fileslisting, fileselected);
    
    if(!folderselected.empty())
    {
        filesystem->cd(filesystem->cwd()/folderselected);
    }

    //ImGui::Text("cwd[%s]", volume->filesystem->cwd.string().c_str());

    if(!fileselected.empty())
    {
        if(gselectedfiles.count(filesystem->cwd())>0 && gselectedfiles[filesystem->cwd()] == fileselected)
        {
            gselectedfiles.erase(filesystem->cwd());
        }
        else
        {
            gselectedfiles[filesystem->cwd()] = fileselected;
        }        
    }

    if(gselectedfiles.count(filesystem->cwd())>0)
    {
        std::filesystem::path url;
        if(filesystem->lookupurl(gselectedfiles[filesystem->cwd()], url)){ fileviewer(url); }
    }

    if (!selectedcrumbspathvec.empty())
    {
        selectedcrumbspathvec.erase(selectedcrumbspathvec.begin());

        printf("selectedcrumbspath[%s]\n", vertualfs::make_string(selectedcrumbspathvec).c_str());
        filesystem->cd(std::filesystem::path("/")/vertualfs::make_path(selectedcrumbspathvec));
    }
}




int windowing_main(int argc, const char**)
{
    int filesystemtypeindex = 0;
    const char* filesystemtypes[] = { "GitApiFilesystem", "GitFilesystem" };

    if (!vertualfs::startup()) { return EXIT_FAILURE; }

    std::vector<std::pair<std::string,std::filesystem::path>> baseurlhistory;
    baseurlhistory.push_back({"GitFilesystem",   "https://gitlab.com/telemotor/users/billy/headtest"});
    baseurlhistory.push_back({"GitFilesystem",   "https://github.com/billyzelsnack/vertualfs"});
    baseurlhistory.push_back({"GitApiFilesystem","https://api.github.com/repos/billyzelsnack/vertualfs"});

    std::filesystem::path baseurl;
    vertualfs::Filesystem* filesystem = nullptr;

    while(1)
    {
        if (!windowing_framebegin()) { break; }
        
        std::filesystem::path baseurlnofilename = baseurl; baseurlnofilename.replace_filename("");

        std::string title;
        if (filesystem != nullptr){title = filesystem->baseurl().replace_filename("").string() + std::string("###filesystembrowser"); }
        ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);
        ImGui::BeginChild("Scrolling Region", ImVec2(0, ImGui::GetWindowHeight()-60), false, ImGuiWindowFlags_NoResize);

        if(filesystem!=nullptr)
        {
            filesystembrowser(filesystem);
        }

        ImGui::EndChild();

        ImGui::SetNextItemWidth(140);
        bool comboed=ImGui::Combo("##filesystemtypecombo", &filesystemtypeindex, filesystemtypes, IM_ARRAYSIZE(filesystemtypes));
        ImGui::SameLine();

        ImGui::PushItemWidth(-60);
        char buffer[1024]; strcpy_s(buffer, 1024, baseurl.string().c_str());
        bool entered = ImGui::InputText("###baseurlinputtext", buffer, 1024, ImGuiInputTextFlags_EnterReturnsTrue);
        baseurl = buffer;
        if(entered&&(!baseurl.empty()))
        {
            std::string filesystemtype = filesystemtypes[filesystemtypeindex];
            bool found = false;
            for (int ii = 0; ii < baseurlhistory.size(); ii++)
            {
                if (baseurlhistory[ii].first != filesystemtype) { continue; }
                if (baseurlhistory[ii].second == baseurl) { found = true; break; }
            }
            if (!found){ baseurlhistory.push_back({filesystemtype,baseurl}); }
        }
        ImGui::SameLine();

        ImGui::PushItemWidth(-1);
        static char text[256] = "";
        if (ImGui::BeginCombo("###comboinputtext", text,ImGuiComboFlags_NoPreview|ImGuiComboFlags_PopupAlignLeft))
        {
            for (auto& [type, url] : baseurlhistory)
            {
                bool selected = false;
                if(ImGui::Selectable(("["+type+"]"+url.string()).c_str(), &selected, ImGuiSelectableFlags_None))
                {
                    for (filesystemtypeindex = 0; filesystemtypeindex < IM_ARRAYSIZE(filesystemtypes); filesystemtypeindex++)
                    {
                        if (type == std::string(filesystemtypes[filesystemtypeindex])) { baseurl = url; comboed = true; break; }
                    }
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndCombo();
        }
        

        
        if(comboed||entered)
        {
            std::string filesystemtype = filesystemtypes[filesystemtypeindex];
            printf("entered baseurl[%s][%s]\n",filesystemtype.c_str(), baseurl.string().c_str());

            bool found = false;
            for(int ii = 0; ii < baseurlhistory.size(); ii++)
            {
                if(baseurlhistory[ii].first!=filesystemtype){ continue; }
                if(baseurlhistory[ii].second==baseurl){ found = true; break; }
            }

            delete filesystem;
            filesystem = nullptr;
            if(filesystemtype=="GitFilesystem")
            {
                filesystem = vertualfs::GitFilesystem::create(baseurl);
            }
            else
            if(filesystemtype=="GitApiFilesystem")
            {
                filesystem = vertualfs::GitApiFilesystem::create(baseurl);
            }
        }
        ImGui::PopItemWidth();
        ImGui::End();

        if (!windowing_frameend()) { break; }
    }

    vertualfs::shutdown();
    return EXIT_SUCCESS;
}












