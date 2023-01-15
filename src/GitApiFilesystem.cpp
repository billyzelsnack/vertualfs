

#include "vertualfs/GitApiFilesystem.hpp"
#include "http.hpp"

#include <nlohmann/json.hpp>
#include <vertualfs/vertualfs.hpp>

using json_t = nlohmann::json;



vertualfs::GitApiFilesystem::GitApiFilesystem(const std::filesystem::path& baseurl) : fsbaseurl(baseurl)
{

}

vertualfs::GitApiFilesystem::~GitApiFilesystem()
{

}

std::filesystem::path vertualfs::GitApiFilesystem::baseurl() const
{
    return fsbaseurl;
}

std::filesystem::path vertualfs::GitApiFilesystem::cwd() const
{
    return fscwd;
}

bool vertualfs::GitApiFilesystem::cd(const std::filesystem::path& path)
{
    std::filesystem::path pathmp(path);
    printf("cd [%s][%s]\n", fscwd.string().c_str(), pathmp.string().c_str());
    if(path.has_root_directory())
    {
        fscwd = pathmp;
    }
    else
    {
        fscwd = fscwd / pathmp.relative_path();
    }

    std::filesystem::path apiurl = vertualfs::make_preferred(fsbaseurl / std::filesystem::path("contents") / fscwd.relative_path());
    printf("apiurl[%s]\n", apiurl.string().c_str());

    std::string redirectUrl;
    fscwdjson = http_request_json("", apiurl.string(), json_t(), redirectUrl, false);
    printf("redirectUrl[%s]\n", redirectUrl.c_str());
    if (fscwdjson.empty()) { return false; }



    /*
    fscwd=fscwd/path;
    //printf("cwd[%s]\n", fscwd.string().c_str());

    std::filesystem::path apiurl = baseurl / std::filesystem::path("contents") / fscwd;
    printf("apiurl[%s]\n", apiurl.string().c_str());

    std::string redirectUrl;
    fscwdjson = http_request_json("", apiurl.string(), redirectUrl, false);
    printf("redirectUrl[%s]\n", redirectUrl.c_str());
    if (fscwdjson.empty()) { return false; }
    //printf("[%s]\n", cwdjson.dump(4).c_str());
    */

    return true;
}

bool vertualfs::GitApiFilesystem::listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing)
{
    //-- todo: only paths that start from root are valid
    if (path != fscwd)
    {
        printf("listingcd[%s][%s]\n", fscwd.string().c_str(), path.string().c_str());
        if (!cd(path)) { return false; }
    }
    
    //printf("[%s]\n", fscwdjson.dump(4).c_str());
    
    for (auto& element : fscwdjson)
    {
        try
        {
            std::string name = element["name"].get<std::string>();
            std::string type = element["type"].get<std::string>();

            out_listing.push_back({ name, type == "dir" });
            //printf("[%s][%s]\n", name.c_str(), type.c_str());
        }
        catch (...) {}
    }
    
    return true;
}

bool vertualfs::GitApiFilesystem::ls(std::vector<std::pair<std::string, bool>>& out_listing)
{
    return listing(fscwd,out_listing);
}

bool vertualfs::GitApiFilesystem::lookupurl(const std::filesystem::path& path, std::filesystem::path& out_url) const
{
    printf("lookupurl[%s][%s]\n", fscwd.string().c_str(), path.string().c_str());

    return false;
}

vertualfs::GitApiFilesystem* vertualfs::GitApiFilesystem::create(const std::filesystem::path& baseurl)
{   
    vertualfs::GitApiFilesystem* filesystem=new GitApiFilesystem(baseurl);
    if(filesystem == nullptr){ return nullptr; }

    if(!filesystem->cd(""))
    {
        delete filesystem;
        return nullptr;
    }
    
    return filesystem;
}



