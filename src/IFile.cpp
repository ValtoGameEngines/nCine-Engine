#include "common_macros.h"
#include "IFile.h"
#include "StandardFile.h"

#ifdef __ANDROID__
	#include <cstring>
	#include "AssetFile.h"
#endif

namespace ncine {

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

IFile::IFile(const char *filename)
    : type_(FileType::BASE), filename_(filename), fileDescriptor_(-1), filePointer_(nullptr),
      shouldCloseOnDestruction_(true), shouldExitOnFailToOpen_(true), fileSize_(0)
{
	ASSERT(filename);
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

bool IFile::isOpened() const
{
	if (fileDescriptor_ >= 0 || filePointer_ != nullptr)
		return true;
	else
		return false;
}

nctl::UniquePtr<IFile> IFile::createFileHandle(const char *filename)
{
	ASSERT(filename);
#ifdef __ANDROID__
	if (strncmp(filename, static_cast<const char *>("asset::"), 7) == 0)
	{
		// Skip leading path separator character
		const char *assetFilename = (filename[7] == '/') ? filename + 8 : filename + 7;
		return nctl::makeUnique<AssetFile>(assetFilename);
	}
	else
#endif
		return nctl::makeUnique<StandardFile>(filename);
}

}
