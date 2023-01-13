
#include "vertualfs/Filesystem.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <git2.h>


static bool started = false;



//-- after an open need to check if the remote has changed



vertualfs::Filesystem::Filesystem(git_repository* repository, git_commit* commit) : repository(repository), commit(commit)
{

}

vertualfs::Filesystem::~Filesystem()
{
	git_commit_free(commit);
	git_repository_free(repository);
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
	if (0 == git_describe_workdir(&describe_result, repository, &describe_options))
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
	git_commit_tree(&root, commit);
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
				git_tree_entry_to_object(&object, repository, entry);
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

vertualfs::Filesystem* vertualfs::Filesystem::create(const std::string& path)
{
	if (!started) { return nullptr; }

	std::string openpath = vertualfs::Filesystem_CreateLocalPath(path);

	std::filesystem::path stdpath(path);
	stdpath.replace_extension(); //-- unify to never having the .git extension
	std::string remoteurl = stdpath.string();

	//printf("[%s] [%s]\n", remoteurl.c_str(), openpath.c_str());
	//return nullptr;

	git_repository* repo = nullptr;
	printf("attempt git_repository_open [%s]\n", openpath.c_str());
	if (0 != git_repository_open(&repo, openpath.c_str()))
	{
		printf("attempt git_repository_clone [%s] to [%s]\n", remoteurl.c_str(), openpath.c_str());
		if (0 != git_clone(&repo, remoteurl.c_str(), openpath.c_str(), nullptr))
		{
			const git_error* error = git_error_last();
			printf("Error: %s\n", error->message);
			return nullptr;
		}
	}

	git_reference* commitref = nullptr;
	git_repository_head(&commitref, repo);
	git_commit* commit = nullptr;
	git_reference_peel((git_object**)&commit, commitref, GIT_OBJ_COMMIT);
	git_reference_free(commitref); //-- todo: can i delete this immediately
	if (commit == nullptr)
	{
		git_repository_free(repo);
		return nullptr;
	}

	return new vertualfs::Filesystem(repo, commit);
}

bool vertualfs::Filesystem::lookup_remote_url(const std::string& name, std::string& out_url) const
{
	out_url.clear();

	git_remote* remote = nullptr;
	if(0!=git_remote_lookup(&remote, repository, name.c_str())){return false;}
	out_url=std::string(git_remote_url(remote));
	git_remote_free(remote);

	return true;
}



std::string vertualfs::Filesystem_CreateLocalPath(const std::string& url)
{
	std::filesystem::path stdpath(url);
	stdpath.replace_extension(); //-- unify to never having the .git extension
	std::string remoteurl = stdpath.string();

	std::string protocol = stdpath.root_name().string();
	std::string domain = stdpath.root_directory().string();
	std::regex url_regex("(https?)://([^/]+)(.*)");
	std::smatch url_match;
	if (std::regex_match(remoteurl, url_match, url_regex))
	{
		protocol = url_match[1];
		domain = url_match[2];
		std::string modpath = remoteurl;
		modpath.erase(0, protocol.length() + 2 + domain.length() + 1);
		stdpath = std::filesystem::path(modpath);
	}
	else
	{
		domain = protocol;
		//domain.erase(std::remove(domain.begin(), domain.end(), ':'), domain.end());
	}
	domain.erase(std::remove(domain.begin(), domain.end(), ':'), domain.end());
	//std::cout << "Protocol: " << protocol << std::endl;
	//std::cout << "Domain: " << domain << std::endl;
	//std::cout << "Path: " << stdpath.relative_path() << std::endl;
	//std::cout << "Filename: " << stdpath.filename() << std::endl;
	//std::cout << "Filename stem: " << stdpath.stem() << std::endl;
	//std::cout << "Filename extension: " << stdpath.extension() << std::endl;

	std::string openpath = "vertualfs";
	if (!domain.empty()) { openpath += "/" + domain; }
	openpath += "/" + stdpath.relative_path().string();

	return openpath;
}





void vertualfs::Filesystem_Shutdown()
{
	if (!started) { return; }

	git_libgit2_shutdown();
	started = false;

	return;
}

bool vertualfs::Filesystem_Startup()
{
	if (started) { return true; }

	if (0 == git_libgit2_init()) { return false; }
	started = true;

	return true;
}





