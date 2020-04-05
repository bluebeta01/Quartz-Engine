#include "pch.h"
#include "pngloader.h"
#include <lodepng.h>
#include "filesystem/filesystem.h"

namespace asset
{
	void loadTextureFromDisk(JobLoadTextureData* job)
	{
		std::string path = "textures/";
		path.append(job->filename);
		path.append(".png");

		path = filesystem::findFilePathByName(path);

		unsigned int errorCode = lodepng::decode(job->textureData->rawPixelData, job->textureData->width, job->textureData->height, path.c_str());
		if (errorCode)
		{
			std::string error = "Failed to open the file ";
			error.append(path);
			LOGERROR(error.c_str());
			return;
		}


		job->textureData->loaded = true;
	}
}