
#ifndef VERTUALFSTOOL_CLI_HPP
#define VERTUALFSTOOL_CLI_HPP


namespace vertualfstool
{

	bool commandscli(int argc, const char** argv, std::string& out_result);
	bool commandscli(const char* argv0, const std::string& line, std::string& out_result);

}



#endif
