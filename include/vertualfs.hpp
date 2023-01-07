

#ifndef VERTUALFS_HPP
#define VERTUALFS_HPP



namespace vertualfs
{
	class FileSystem;
}

class vertualfs::FileSystem
{

private:

	FileSystem(struct git_repository* repo);
	~FileSystem();

public:

	bool list(std::vector<std::pair<std::string, bool>>& out_files);

	static FileSystem* create(const std::string& localfullpath);

private:

	git_repository* repo = nullptr;

};



bool vertualfs_startup();
void vertualfs_shutdown();




#endif