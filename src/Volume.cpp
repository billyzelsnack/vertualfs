
#include "vertualfs/Volume.hpp"
#include "vertualfs/Filesystem.hpp"
#include "vertualfs/Repository.hpp"





vertualfs::Volume* vertualfs::Volume::create(const std::filesystem::path& path)
{
	vertualfs::Filesystem* filesystem = vertualfs::Filesystem::create(path.string());
	if(filesystem == nullptr){ return nullptr; }

	Volume* volume = new Volume();
	volume->availableFilesystems.push_back(filesystem);
	
	return volume;
}







