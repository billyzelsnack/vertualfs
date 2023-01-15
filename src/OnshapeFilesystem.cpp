

#include "vertualfs/OnshapeFilesystem.hpp"
#include "http.hpp"
#include <vertualfs/vertualfs.hpp>


using json_t = nlohmann::json;



vertualfs::OnshapeFilesystem::OnshapeFilesystem(const std::filesystem::path& baseurl) : fsbaseurl(baseurl)
{

}

vertualfs::OnshapeFilesystem::~OnshapeFilesystem()
{

}

std::filesystem::path vertualfs::OnshapeFilesystem::baseurl() const
{
    return fsbaseurl;
}

std::filesystem::path vertualfs::OnshapeFilesystem::cwd() const
{
    return fscwd;
}

bool vertualfs::OnshapeFilesystem::cd(const std::filesystem::path& path)
{
    std::filesystem::path pathmp(path);
    printf("cd [%s][%s]\n", fscwd.string().c_str(), pathmp.string().c_str());
    if (path.has_root_directory())
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
    fscwdjson = http_request_json("", apiurl.string(), redirectUrl, false);
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

bool vertualfs::OnshapeFilesystem::listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing)
{
    //-- todo: only paths that start from root are valid
    if (path != fscwd)
    {
        printf("listingcd[%s][%s]\n", fscwd.string().c_str(), path.string().c_str());
        if (!cd(path)) { return false; }
    }

    return true;
}

bool vertualfs::OnshapeFilesystem::ls(std::vector<std::pair<std::string, bool>>& out_listing)
{
    return listing(fscwd, out_listing);
}

bool vertualfs::OnshapeFilesystem::lookupurl(const std::filesystem::path& path, std::filesystem::path& out_url) const
{
    printf("lookupurl[%s][%s]\n", fscwd.string().c_str(), path.string().c_str());

    return false;
}

vertualfs::OnshapeFilesystem* vertualfs::OnshapeFilesystem::create(const std::filesystem::path& baseurl)
{
    vertualfs::OnshapeFilesystem* filesystem = new OnshapeFilesystem(baseurl);
    if (filesystem == nullptr) { return nullptr; }

    if (!filesystem->cd("/"))
    {
        delete filesystem;
        return nullptr;
    }

    return filesystem;
}





