
#include "vertualfs/Filesystem.hpp"

#include <filesystem>
#include <regex>
#include <string>

#include <git2.h>

#include "vertualfs/Repository.hpp"


vertualfs::GitFilesystem::GitFilesystem(vertualfs::GitRepository* repository) : repository(repository)
{
}

vertualfs::GitFilesystem::~GitFilesystem()
{
}

bool vertualfs::GitFilesystem::repo_version(std::string& out_version) const
{
	git_describe_options describe_options;
	memset(&describe_options, 0, sizeof(describe_options));
	if (0 != git_describe_options_init(&describe_options, GIT_DESCRIBE_OPTIONS_VERSION)) { return false; }
	describe_options.describe_strategy = GIT_DESCRIBE_TAGS;

	git_describe_format_options format_options;
	memset(&format_options, 0, sizeof(format_options));
	if (0 != git_describe_format_options_init(&format_options, GIT_DESCRIBE_FORMAT_OPTIONS_VERSION)) { return false; }

	git_describe_result* describe_result;
	if (0 == git_describe_workdir(&describe_result, repository->repo, &describe_options))
	{
		git_buf buf = { 0 };
		if (0 == git_describe_format(&buf, describe_result, &format_options))
		{
			if (buf.ptr != nullptr) { out_version = std::string(buf.ptr); return true; }
		}
	}

	return false;
}


bool vertualfs::GitFilesystem::listing(const std::string& path, std::vector<std::pair<std::string, bool>>& out_listing)
{	
	out_listing.clear();

	for (unsigned int ii = 0; ii < git_tree_entrycount(repository->tree); ii++)
	{
		const git_tree_entry* entry = git_tree_entry_byindex(repository->tree, ii);

		git_object_t type = git_tree_entry_type(entry);
		const char* name = git_tree_entry_name(entry);
		if (type == GIT_OBJ_TREE)
		{
			printf("path %s\n", name);
			out_listing.push_back({ name, true });
		}
		else
		if (type == GIT_OBJ_BLOB)
		{
			printf("file %s\n", name);
			out_listing.push_back({ name, false });
		}
	}

	return false;
}

vertualfs::GitFilesystem* vertualfs::GitFilesystem::create(vertualfs::GitRepository* repository)
{
	//LOG(INFO) << "vertualfs::GitFilesystem::create";

	if (repository == nullptr) { return nullptr; }

	return new vertualfs::GitFilesystem(repository);
}



bool vertualfs::GitFilesystem::lookup_remote_url(const std::string& name, std::string& out_url) const
{
	out_url.clear();

	git_remote* remote = nullptr;
	if(0!=git_remote_lookup(&remote, repository->repo, name.c_str())){return false;}
	out_url=std::string(git_remote_url(remote));
	git_remote_free(remote);

	return true;
}










