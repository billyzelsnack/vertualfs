
#ifndef VERTUALFS_HTTP_HPP
#define VERTUALFS_HTTP_HPP


#include <nlohmann/json.hpp>

using json_t = nlohmann::json;


namespace vertualfs
{
    std::string http_request(const std::string& authorization, const std::string& url, const std::string& accept, const json_t& jsonpostfields, std::string& out_redirectUrl, bool verbose);
    json_t http_request_json(const std::string authorization, const std::string& url, const json_t& jsonpostfields, std::string& out_redirectUrl, bool verbose);
    std::string http_request_octet(const std::string& authorization, const std::string& url, std::string& out_redirectUrl, bool verbose);
}




#endif