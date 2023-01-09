

#include <git2.h>
#include "git.hpp"


static bool started = false;


void git_shutdown()
{
	if (!started) { return; }

	printf("git_shutdown\n");
	git_libgit2_shutdown();
}

bool git_startup()
{
	if (started) { return true; }

	printf("git_startup\n");

	if (started) { return true; }

	git_libgit2_init();
	started = true;

	return true;
}

bool git_repo_clone(const std::string& url, const std::string& path, struct git_repository*& out_repo)
{
	out_repo = nullptr;
	if(0==git_clone(&out_repo, url.c_str(), path.c_str(), NULL)){ return true; }

	return false;
}

bool git_repo_open(const std::string& path, struct git_repository*& out_repo )
{
	out_repo = nullptr;
	if(0==git_repository_open(&out_repo, path.c_str())) { return true; }

	return false;
}

bool git_repo_tags(struct git_repository* repo, const std::string& match, std::vector<std::string>& out_tags)
{
	git_strarray tag_names = { 0 };
	if(0==git_tag_list_match(&tag_names, match.c_str(), repo))
	{
		out_tags.clear();
		for (size_t i = 0; i < tag_names.count; i++) { out_tags.push_back(tag_names.strings[i]); }
		git_strarray_dispose(&tag_names);
		return true;
	}
	return false;
}

bool git_repo_version(struct git_repository* repo, std::string& out_version)
{
	git_describe_options describe_options;
	memset(&describe_options, 0, sizeof(describe_options));
	if (0 != git_describe_options_init(&describe_options, GIT_DESCRIBE_OPTIONS_VERSION)) { return false; }
	describe_options.describe_strategy = GIT_DESCRIBE_TAGS;

	git_describe_format_options format_options;
	memset(&format_options, 0, sizeof(format_options));
	if (0 != git_describe_format_options_init(&format_options, GIT_DESCRIBE_FORMAT_OPTIONS_VERSION)) { return false; }

	git_describe_result* describe_result;
	if (0 == git_describe_workdir(&describe_result, repo, &describe_options))
	{
		git_buf buf = { 0 };
		if (0 == git_describe_format(&buf, describe_result, &format_options))
		{
			if (buf.ptr != nullptr) { out_version = std::string(buf.ptr); return true; }
		}
	}

	return false;
}

void git_repo_close(struct git_repository*& repo)
{
	git_repository_free(repo);
	repo = nullptr;
}

bool git_repo_tree(struct git_repository* repo, const std::string& path, struct git_tree*& out_tree)
{
	/*
	out_tree = nullptr;

	struct git_commit* head = nullptr;
	struct git_reference* headref = nullptr;
	git_repository_head(&headref, repo);
	git_reference_peel((git_object**)&head, headref, GIT_OBJ_COMMIT);

	struct git_tree* treeroot = nullptr;
	git_commit_tree(&treeroot, head);

	out_tree = treeroot;
	if (!path.empty())
	{
		const git_tree_entry* entry = git_tree_entry_byname(treeroot, path.c_str());
		git_object_t type = git_tree_entry_type(entry);
		if (type == GIT_OBJ_TREE)
		{
			git_object* obj = nullptr;
			git_tree_entry_to_object(&obj, repo, entry);
			git_tree_lookup(&out_tree, repo, git_object_id(obj));
		}
		else
		{
			out_tree = nullptr;
		}
	}

	git_tree_free(tree);
	git_commit_free(head);
	git_reference_free(headref);

	return true;
	*/
	return false;
}



bool git_repo_listing(struct git_repository* repo, const std::string& path, std::vector<std::pair<std::string,bool>>& out_listing)
{
	/*
	out_listing.clear();

	struct git_tree* tree = nullptr;
	if(!git_repo_tree(repo, path, tree))
	{
		return false;
	}

	git_repository* tree_repo = git_tree_owner(tree);

	
	
	for(unsigned int ii = 0; ii < git_tree_entrycount(tree); ii++) 
	{
		const git_tree_entry* entry = git_tree_entry_byindex(tree, ii);

		git_object_t type = git_tree_entry_type(entry);
		if (type == GIT_OBJ_TREE)
		{
			printf("path %s\n", git_tree_entry_name(entry));
		}
		else
		if (type == GIT_OBJ_BLOB)
		{
			printf("file %s\n", git_tree_entry_name(entry));
		}
	}

	git_tree_free(tree);
	git_commit_free(head);
	git_reference_free(headref);

	return true;
	*/

	return false;
}

























