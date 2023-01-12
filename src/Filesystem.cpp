
#include "vertualfs/Filesystem.hpp"
#include "vertualfs/Repository.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <git2.h>




vertualfs::Filesystem::Filesystem(vertualfs::Repository* repository) : repository(repository)
{
}

vertualfs::Filesystem::~Filesystem()
{
}

bool vertualfs::Filesystem::repo_version(std::string& out_version) const
{
	git_describe_options describe_options;
	memset(&describe_options, 0, sizeof(describe_options));
	if (0 != git_describe_options_init(&describe_options, GIT_DESCRIBE_OPTIONS_VERSION)) { return false; }
	describe_options.describe_strategy = GIT_DESCRIBE_TAGS;

	git_describe_format_options format_options;
	memset(&format_options, 0, sizeof(format_options));
	if (0 != git_describe_format_options_init(&format_options, GIT_DESCRIBE_FORMAT_OPTIONS_VERSION)) { return false; }

	git_describe_result* describe_result;
	if (0 == git_describe_workdir(&describe_result, repository->repository, &describe_options))
	{
		git_buf buf = { 0 };
		if (0 == git_describe_format(&buf, describe_result, &format_options))
		{
			if (buf.ptr != nullptr) { out_version = std::string(buf.ptr); return true; }
		}
	}

	return false;
}


bool vertualfs::Filesystem::listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing)
{	
	out_listing.clear();

	std::filesystem::path lscwd;// = "/";

	git_tree* root = nullptr;
	git_commit_tree(&root, repository->commit);
	if (root == nullptr) { return false; }
	std::vector<git_tree*> crumbs;
	crumbs.push_back(root);
	git_tree* trav = root;
	for (const auto& subpath : path)
	{
		size_t numentries = git_tree_entrycount(trav);
		for (unsigned int ii = 0; ii < numentries; ii++)
		{
			const git_tree_entry* entry = git_tree_entry_byindex(trav, ii);

			git_object_t type = git_tree_entry_type(entry);
			const char* name = git_tree_entry_name(entry);
			if ((type == GIT_OBJ_TREE) && (subpath == name))
			{
				git_object* object=nullptr;
				git_tree_entry_to_object(&object, repository->repository, entry);
				git_object_peel((git_object**)&trav, object, GIT_OBJECT_TREE);
				git_object_free(object);
				crumbs.push_back(trav);
				lscwd.append(name);
				continue;
			}
		}
	}
	
	
	size_t numentries = git_tree_entrycount(trav);
	for (unsigned int ii = 0; ii < numentries; ii++)
	{
		const git_tree_entry* entry = git_tree_entry_byindex(trav, ii);

		git_object_t type = git_tree_entry_type(entry);
		const char* name = git_tree_entry_name(entry);
		if (type == GIT_OBJ_TREE)
		{
			out_listing.push_back({ name, true });
		}
		else
		if (type == GIT_OBJ_BLOB)
		{ 
			out_listing.push_back({ name, false });
		}
	}

	for (auto crumb : crumbs) { git_tree_free(crumb); }

	return true;
}

bool vertualfs::Filesystem::ls(std::vector<std::pair<std::string, bool>>& out_listing)
{
	if (!listing(cwd.string(), out_listing)) { return false; }
	return true;
}

bool vertualfs::Filesystem::cd(const std::filesystem::path& relativepath)                          
{
	std::filesystem::path newcwd = cwd.append(relativepath.string());
	cwd = newcwd;


	return true;
}

vertualfs::Filesystem* vertualfs::Filesystem::create(vertualfs::Repository* repository)
{
	if (repository == nullptr) { return nullptr; }

	return new vertualfs::Filesystem(repository);
}

bool vertualfs::Filesystem::lookup_remote_url(const std::string& name, std::string& out_url) const
{
	out_url.clear();

	git_remote* remote = nullptr;
	if(0!=git_remote_lookup(&remote, repository->repository, name.c_str())){return false;}
	out_url=std::string(git_remote_url(remote));
	git_remote_free(remote);

	return true;
}










