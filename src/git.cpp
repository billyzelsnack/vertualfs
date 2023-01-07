

#include <git2.h>
#include "git.hpp"


static bool started = false;


bool git_startup()
{
	printf("git_startup\n");

	if (started) { return true; }

	git_libgit2_init();
	started = true;

	return true;
}

bool git_repo_open(git_repository** repo, const std::string& path)
{
	if (0 == git_repository_open(repo, path.c_str())) { return true; }

	return false;
}

bool git_repo_tags(git_repository* repo, const std::string& match, std::vector<std::string>& out_tags)
{
	git_strarray tag_names = { 0 };
	if (0 == git_tag_list_match(&tag_names, match.c_str(), repo))
	{
		out_tags.clear();
		for (size_t i = 0; i < tag_names.count; i++) { out_tags.push_back(tag_names.strings[i]); }
		git_strarray_dispose(&tag_names);
		return true;
	}
	return false;
}

bool git_repo_version(git_repository* repo, std::string& out_version)
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

void git_repo_close(git_repository* repo)
{
	git_repository_free(repo);
}

void git_shutdown()
{
	printf("git_shutdown\n");
	if (started) { git_libgit2_shutdown(); }
}


