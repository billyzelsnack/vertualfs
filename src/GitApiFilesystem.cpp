
#include <vertualfs/GitApiFilesystem.hpp>

#include <cstdlib>
#include <cstring>
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

using json_t = nlohmann::json;


//--
//-- http
//--

std::string static http_request(const std::string& authorization, const std::string& url, const std::string& accept, std::string& out_redirectUrl, bool verbose)
{
    out_redirectUrl.clear();

    std::stringstream stream;
    try
    {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;

        std::list<std::string> header;
        header.push_back("accept: " + accept);
        header.push_back("Authorization: " + authorization);
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

json_t static http_request_json(const std::string authorization, const std::string& url, std::string& out_redirectUrl, bool verbose)
{
    std::string result = http_request(authorization, url, "application/json;charset=UTF-8;qs=0.09", out_redirectUrl, verbose);
    if (result.empty()) { return json_t(); }
    return json_t::parse(result);
}

std::string static http_request_octet(const std::string& authorization, const std::string& url, std::string& out_redirectUrl, bool verbose)
{
    return http_request(authorization, url, "application/octet-stream", out_redirectUrl, verbose);
}


//--
//-- GitApiFilesystem
//--




bool vertualfs::GitApiFilesystem::cd(const std::filesystem::path& relativepath)
{
    return false;
}

bool vertualfs::GitApiFilesystem::listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing)
{
    return false;
}

bool vertualfs::GitApiFilesystem::ls(std::vector<std::pair<std::string, bool>>& out_listing)
{
    return false;
}




















