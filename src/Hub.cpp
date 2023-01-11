
#include "vertualfs/Hub.hpp"

#include "vertualfs/Filesystem.hpp"
#include "vertualfs/Repository.hpp"





vertualfs::Hub* vertualfs::Hub::create(const std::filesystem::path& hubpath)
{
	vertualfs::GitRepository* repository = vertualfs::GitRepository_Create(hubpath.string());
	if(repository == nullptr) { return nullptr; }

	vertualfs::GitFilesystem* filesystem = vertualfs::GitFilesystem::create(repository);
	if(filesystem == nullptr)
	{
		delete repository;
		return nullptr;
	}

	Hub* hub = new Hub();
	hub->availableRepositories.push_back(repository);
	hub->filesystem = filesystem;
	
	return hub;
}







