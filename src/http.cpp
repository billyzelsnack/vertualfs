

#include "http.hpp"

#include <cppcodec/base64_default_rfc4648.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Info.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>


using json_t = nlohmann::json;




std::string vertualfs::http_request(const std::string& authorization, const std::string& url, const std::string& accept, std::string& out_redirectUrl, bool verbose)
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

json_t vertualfs::http_request_json(const std::string authorization, const std::string& url, std::string& out_redirectUrl, bool verbose)
{
    std::string result = vertualfs::http_request(authorization, url, "application/json;charset=UTF-8;qs=0.09", out_redirectUrl, verbose);
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

std::string vertualfs::http_request_octet(const std::string& authorization, const std::string& url, std::string& out_redirectUrl, bool verbose)
{
    return vertualfs::http_request(authorization, url, "application/octet-stream", out_redirectUrl, verbose);
}

