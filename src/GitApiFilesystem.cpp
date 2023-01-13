
#include <vertualfs/GitApiFilesystem.hpp>

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <cppcodec/base64_default_rfc4648.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Info.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <vertualfs/vertualfs.hpp>

using json_t = nlohmann::json;


//--
//-- http
//--

static std::string http_request(const std::string& authorization, const std::string& url, const std::string& accept, std::string& out_redirectUrl, bool verbose)
{
    out_redirectUrl.clear();

    std::stringstream stream;
    try
    {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;

        std::list<std::string> header;
        header.push_back("User-Agent: vertualfs");
        if (!authorization.empty()) { header.push_back("Authorization: " + authorization); }
        if (!accept.empty()) { header.push_back("Accept: " + accept); }
        request.setOpt(curlpp::options::HttpHeader(header));
        request.setOpt(curlpp::options::Url(url));
        request.setOpt(curlpp::options::Verbose(verbose));
        request.setOpt(curlpp::options::WriteStream(&stream));
        request.setOpt(curlpp::options::FollowLocation(false));
        request.perform();

        const char* redirectUrl = curlpp::Info<CURLINFO_REDIRECT_URL, const char*>::get(request);
        if (redirectUrl != nullptr) { out_redirectUrl = std::string(redirectUrl); }
    }
    catch (curlpp::LogicError& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (curlpp::RuntimeError& e)
    {
        std::cout << e.what() << std::endl;
    }

    return stream.str();
}

static json_t http_request_json(const std::string authorization, const std::string& url, std::string& out_redirectUrl, bool verbose)
{
    std::string result = http_request(authorization, url, "application/json;charset=UTF-8;qs=0.09", out_redirectUrl, verbose);
    //std::string result = http_request(authorization, url, "application/vnd.github+json", out_redirectUrl, verbose);
    if (result.empty()) { return json_t(); }
    
    
    try
    {
        return json_t::parse(result);
    }
    catch (const json_t::parse_error& e)
    {
        std::cout << "parse error: " << e.what() << std::endl;
        printf("result[%s]\n", result.c_str());
    }


    return json_t();
}

static std::string http_request_octet(const std::string& authorization, const std::string& url, std::string& out_redirectUrl, bool verbose)
{
    return http_request(authorization, url, "application/octet-stream", out_redirectUrl, verbose);
}


//--
//-- GitApiFilesystem
//--

vertualfs::GitApiFilesystem::GitApiFilesystem(const std::filesystem::path& url) : url(url)
{

}

vertualfs::GitApiFilesystem::~GitApiFilesystem()
{

}

bool vertualfs::GitApiFilesystem::cd(const std::filesystem::path& relativepath)
{
    cwd /= relativepath;
    printf("cwd[%s]\n", cwd.string().c_str());

    std::filesystem::path apiurl = vertualfs::make_preferred(url /std::filesystem::path("contents") / cwd);
    printf("apiurl[%s]\n", apiurl.string().c_str());

    std::string redirectUrl;
    cwdjson = http_request_json("", apiurl.string(), redirectUrl, true);
    printf("redirectUrl[%s]\n", redirectUrl.c_str());
    if (cwdjson.empty()) { return false; }
    //printf("[%s]\n", cwdjson.dump(4).c_str());


    return true;
}

bool vertualfs::GitApiFilesystem::listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing)
{
    //-- todo: only paths that start from root are valid
    if(!cd(path)){return false;}
    
    //printf("[%s]\n", cwdjson.dump(4).c_str());

    for (auto& element : cwdjson)
    {
        std::string name = element["name"].get<std::string>();
        std::string type = element["type"].get<std::string>();

        out_listing.push_back({name, type == "dir"});
        //printf("[%s][%s]\n", name.c_str(), type.c_str());
    }

    return true;
}

bool vertualfs::GitApiFilesystem::ls(std::vector<std::pair<std::string, bool>>& out_listing)
{
    return listing(cwd,out_listing);
}

vertualfs::GitApiFilesystem* vertualfs::GitApiFilesystem::create(const std::filesystem::path& url)
{
    //-- todo: if create returns nothing more than the constructor then just get rid of create and make the constructor public.
    return new GitApiFilesystem(url);
}



