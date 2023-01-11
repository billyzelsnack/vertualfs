
#include "vertualfs/Filesystem.hpp"
#include "vertualfs/Repository.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include <git2.h>
#include <nlohmann/json.hpp>




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


bool vertualfs::GitFilesystem::listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing)
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
				git_tree_entry_to_object(&object, repository->repo, entry);
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
			if(std::string(name)=="vertualfs.json")
			{
				std::string remoteurl;
				if (lookup_remote_url("origin", remoteurl))
				{
					std::string localpath = GitRepository_CreateLocalPath(remoteurl);
					//printf("[%s]\n", localpath.c_str());

					std::filesystem::path jsonpath = std::filesystem::path(localpath) / lscwd / std::string(name);
					jsonpath.make_preferred();
					//printf("[%s]\n", jsonpath.string().c_str());
					std::ifstream file(jsonpath);
					nlohmann::json json;
					file >> json;
					//std::cout << json.dump(4) << std::endl;
					auto& jsonrepos=json.at("repositories");
					for (auto& jsonrepo : jsonrepos)
					{
						std::string url=jsonrepo["url"];
						std::string localurl = GitRepository_CreateLocalPath(url);
						//printf("[%s]\n", localurl.c_str());
						out_listing.push_back({ localurl, true });
					}
				}

				//out_listing.push_back({ name, true });
			}
			else
			{
				out_listing.push_back({ name, false });
			}
		}
	}

	for (auto crumb : crumbs) { git_tree_free(crumb); }

	return true;
}

bool vertualfs::GitFilesystem::ls(std::vector<std::pair<std::string, bool>>& out_listing)
{
	if (!listing(cwd.string(), out_listing)) { return false; }
	return true;
}

bool vertualfs::GitFilesystem::cd(const std::filesystem::path& relativepath)                          
{
	std::filesystem::path newcwd = cwd.append(relativepath.string());
	cwd = newcwd;


	return true;
}


vertualfs::GitFilesystem* vertualfs::GitFilesystem::create(vertualfs::GitRepository* repository)
{
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










