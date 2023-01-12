
#include "vertualfs/Volume.hpp"

#include "vertualfs/Filesystem.hpp"
#include "vertualfs/Repository.hpp"





vertualfs::Volume* vertualfs::Volume::create(const std::filesystem::path& hubpath)
{
	vertualfs::Repository* repository = vertualfs::Repository_Create(hubpath.string());
	if(repository == nullptr) { return nullptr; }

	vertualfs::Filesystem* filesystem = vertualfs::Filesystem::create(repository);
	if(filesystem == nullptr)
	{
		delete repository;
		return nullptr;
	}

	Volume* volume = new Volume();
	volume->availableRepositories.push_back(repository);
	volume->filesystem = filesystem;
	
	return volume;
}







