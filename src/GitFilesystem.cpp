
#include "vertualfs/GitFilesystem.hpp"
#include "vertualfs/vertualfs.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <git2.h>




static bool started = false;



//-- after an open need to check if the remote has changed (do a pull for instance)



static std::filesystem::path CreateLocalPath(const std::filesystem::path& url)
{
	std::filesystem::path unipath(url);
	unipath.replace_extension(); //-- unify to never having the .git extension
	std::filesystem::path remoteurl = unipath;

	std::string protocol = unipath.root_name().string();
	std::string domain = unipath.root_directory().string();
	std::regex url_regex("(https?)://([^/]+)(.*)");
	std::smatch url_match;
	std::string wtf(remoteurl.string());
	if (std::regex_match(wtf, url_match, url_regex))
	{
		protocol = url_match[1];
		domain = url_match[2];
		std::string modpath = remoteurl.string();
		modpath.erase(0, protocol.length() + 2 + domain.length() + 1);
		unipath = std::filesystem::path(modpath);
	}
	else
	{
		domain = protocol;
	}
	domain.erase(std::remove(domain.begin(), domain.end(), ':'), domain.end());

	std::filesystem::path openpath = "vertualfs";
	if (!domain.empty()) { openpath += "/" + domain; }
	openpath /= "/" / unipath.relative_path();

	return openpath;
}





vertualfs::GitFilesystem::GitFilesystem(const std::filesystem::path& baseurl, git_repository* repository, git_commit* commit) : fsbaseurl(baseurl), repository(repository), commit(commit)
{
	fscwd = vertualfs::make_preferred("/");
}

vertualfs::GitFilesystem::~GitFilesystem()
{
	git_commit_free(commit);
	git_repository_free(repository);
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

std::filesystem::path vertualfs::GitFilesystem::baseurl() const
{
	return fsbaseurl;
}

std::filesystem::path vertualfs::GitFilesystem::cwd() const
{
	return fscwd;
}

bool vertualfs::GitFilesystem::listing(const std::filesystem::path& path, std::vector<std::pair<std::string, bool>>& out_listing)
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

bool vertualfs::GitFilesystem::ls(std::vector<std::pair<std::string, bool>>& out_listing)
{
	if (!listing(fscwd, out_listing)) { return false; }
	return true;
}

bool vertualfs::GitFilesystem::cd(const std::filesystem::path& path)
{
	std::filesystem::path pathmp(path);
	printf("cd [%s][%s]\n", fscwd.string().c_str(), pathmp.string().c_str());
	if (path.has_root_directory())
	{
		fscwd = pathmp;
		return true;
	}
	printf("cd [%s][%s]\n", fscwd.string().c_str(), pathmp.string().c_str());
	pathmp = pathmp.relative_path();
	printf("cd(relative_path)[%s][%s]\n", fscwd.string().c_str(), pathmp.string().c_str());
	printf("fscwd:\n");
	vertualfs::print_path(fscwd);
	printf("path:\n");
	vertualfs::print_path(pathmp);
	fscwd = fscwd/path;
	printf("newfscwd:\n");
	vertualfs::print_path(fscwd);

	return true;
}

bool vertualfs::GitFilesystem::lookupurl(const std::filesystem::path& path, std::filesystem::path& out_url) const
{
	std::filesystem::path remoteurl;
	if (!lookup_remote_url("origin", remoteurl)) { return false; }

	out_url=CreateLocalPath(remoteurl)/fscwd/path;
	//printf("out_url[%s]\n", out_url.string().c_str());
	return true;
}

vertualfs::GitFilesystem* vertualfs::GitFilesystem::create(const std::filesystem::path& baseurl)
{
	if (!started) { return nullptr; }

	std::filesystem::path openpath = CreateLocalPath(baseurl);

	std::filesystem::path unipath(baseurl);
	unipath.replace_extension(); //-- unify to never having the .git extension
	std::filesystem::path remoteurl = unipath;

	//printf("[%s] [%s]\n", remoteurl.c_str(), openpath.c_str());
	//return nullptr;

	git_repository* repo = nullptr;
	printf("attempt git_repository_open [%s]\n", openpath.string().c_str());
	if (0 != git_repository_open(&repo, openpath.string().c_str()))
	{
		printf("attempt git_repository_clone [%s] to [%s]\n", remoteurl.string().c_str(), openpath.string().c_str());
		if (0 != git_clone(&repo, remoteurl.string().c_str(), openpath.string().c_str(), nullptr))
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

	return new vertualfs::GitFilesystem(baseurl, repo, commit);
}

bool vertualfs::GitFilesystem::lookup_remote_url(const std::string& name, std::filesystem::path& out_url) const
{
	out_url.clear();

	git_remote* remote = nullptr;
	if(0!=git_remote_lookup(&remote, repository, name.c_str())){return false;}
	out_url=std::filesystem::path(git_remote_url(remote));
	git_remote_free(remote);

	return true;
}




void vertualfs::GitFilesystem_Shutdown()
{
	if (!started) { return; }

	git_libgit2_shutdown();
	started = false;

	return;
}

bool vertualfs::GitFilesystem_Startup()
{
	if (started) { return true; }

	if (0 == git_libgit2_init()) { return false; }
	started = true;

	return true;
}





