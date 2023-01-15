

#include "vertualfs/OpenaiFilesystem.hpp"
#include "http.hpp"
#include "vertualfs/vertualfs.hpp"

#include <regex>

#include <nlohmann/json.hpp>

using json_t = nlohmann::json;



vertualfs::OpenaiFilesystem::OpenaiFilesystem(const std::string& authorization, const std::filesystem::path& baseurl) : authorization(authorization), fsbaseurl(baseurl)
{

}

vertualfs::OpenaiFilesystem::~OpenaiFilesystem()
{

}

std::filesystem::path vertualfs::OpenaiFilesystem::baseurl() const
{
    return fsbaseurl;
}

std::filesystem::path vertualfs::OpenaiFilesystem::cwd() const
{
    return fscwd;
}

//-- https://stackoverflow.com/questions/5878775/how-to-find-and-replace-string
static void ReplaceStringInPlace(std::string& subject, const std::string& search,
    const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

bool vertualfs::OpenaiFilesystem::cd(const std::filesystem::path& path)
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


    json_t jsonpostfields;
    //jsonpostfields["model"] = "text-davinci-003";
    jsonpostfields["prompt"] = "forget everything. pretend you are a disk drive; the current directory has a list of 10 cute cat breed named jpg files. show a listing of files only from the current directory as json; END";
    jsonpostfields["max_tokens"] = 2000;
    jsonpostfields["stop"] = "END";
    jsonpostfields["seed"] = 1;
    //jsonpostfields["temperature"] = 0.5;
    //"max_tokens" : 150,
    //"top_p" : 1,
    //"frequency_penalty" : 0.0,
    //"presence_penalty" : 0.6,
    //"stop" : [" Human:", " AI:"]

    std::filesystem::path apiurl = vertualfs::make_preferred(fsbaseurl);
    printf("apiurl[%s]\n", apiurl.string().c_str());

    std::string redirectUrl;
    fscwdjson = http_request_json(authorization, apiurl.string(), jsonpostfields, redirectUrl, false);
    printf("redirectUrl[%s]\n", redirectUrl.c_str());
    if (fscwdjson.empty()) { return false; }

    try
    {
        printf("%s\n", fscwdjson.dump(4).c_str());
        printf("------------\n");
        std::string text=fscwdjson["choices"][0]["text"].dump();
        printf("AAA%s\n", text.c_str());
        text.erase(0, 1); text.erase(text.size() - 1, 1);
        ReplaceStringInPlace(text, "\\\"", "\"");
        ReplaceStringInPlace(text, "\\n", "\n");
        printf("BBB%s\n", text.c_str());
        json_t json = json_t::parse(text);
        printf("------------\n");
        printf("%s\n", json.dump(4).c_str());

        fscwdjson = json;
    }
    catch (...)
    {
        printf("parse error\n");
        return false;
    }

    


    return true;
}

bool vertualfs::OpenaiFilesystem::listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing)
{
    //-- todo: only paths that start from root are valid
    if (path != fscwd)
    {
        printf("listingcd[%s][%s]\n", fscwd.string().c_str(), path.string().c_str());
        if (!cd(path)) { return false; }
    }

    for (auto& element : fscwdjson)
    {
        try
        {
            out_listing.push_back({ element["name"], false});
            //printf("[%s][%s]\n", name.c_str(), type.c_str());
        }
        catch (...) {}
    }

       
    return true;
}

bool vertualfs::OpenaiFilesystem::ls(std::vector<std::pair<std::string, bool>>& out_listing)
{
    return listing(fscwd,out_listing);
}

bool vertualfs::OpenaiFilesystem::lookupurl(const std::filesystem::path& path, std::filesystem::path& out_url) const
{
    printf("lookupurl[%s][%s]\n", fscwd.string().c_str(), path.string().c_str());

    return false;
}

vertualfs::OpenaiFilesystem* vertualfs::OpenaiFilesystem::create(const std::string& secret_key, const std::filesystem::path& baseurl)
{   
    vertualfs::OpenaiFilesystem* filesystem=new OpenaiFilesystem("Bearer "+secret_key, baseurl);
    if(filesystem == nullptr){ return nullptr; }

    if(!filesystem->cd("/"))
    {
        delete filesystem;
        return nullptr;
    }
  
    return filesystem;
}




