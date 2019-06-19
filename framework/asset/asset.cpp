#include "framework/pch.h"
#include "asset.h"
#include "jobs.h"
#include "loader.h"
#include "framework/screen/screen.h"
#include "pngloader.h"
#include "materialloader.h"

namespace asset
{
	std::vector<Job*> justSubmittedJobs;
	std::vector<Job*> jobVector;

	//This will be used to make sure that the asset thread and the quartz thread don't attempt to access the jobVector at the same time.
	//Calling std::lock_guard will only return once the calling thread has access to the muxtex variable.
	//std::lock_guard will only guard the mutex until the end of the scope it was called in.

	std::mutex jobVectorMutex;

	void submitUnsubmittedJobs()
	{
		std::lock_guard<std::mutex> guard(jobVectorMutex);

		for (int i = 0; i < justSubmittedJobs.size(); i++)
		{
			jobVector.push_back(justSubmittedJobs[i]);
		}

		justSubmittedJobs.clear();
	}

	void checkJobs()
	{
		while (!screen::terminated)
		{
			submitUnsubmittedJobs();

			if (jobVector.size() == 0)
				continue;
			

			Job* job = jobVector.back();

			JobLoadModelData* jobLoadModel = dynamic_cast<JobLoadModelData*>(job);
			if (jobLoadModel)
			{
				loadModelFromDisk(jobLoadModel);

				jobVector.pop_back();
				delete jobLoadModel;
				continue;
				
			}

			JobLoadTextureData* jobLoadTextureData = dynamic_cast<JobLoadTextureData*>(job);
			if (jobLoadTextureData)
			{
				loadTextureFromDisk(jobLoadTextureData);
				
				jobVector.pop_back();
				delete jobLoadTextureData;
				continue;
			}

			JobLoadMaterialData* jobLoadMaterialData = dynamic_cast<JobLoadMaterialData*>(job);
			if (jobLoadMaterialData)
			{
				loadMaterialFromDisk(jobLoadMaterialData);

				jobVector.pop_back();
				delete jobLoadMaterialData;
				continue;
			}


			LOGERROR("Unknown job type submitted to asset jobs!");
		}
	}

	std::thread assetInit()
	{
		std::thread assetThread(checkJobs);
		return assetThread;
	}

	ModelData* assetLoadModelData(std::string filename)
	{
		JobLoadModelData* job = new JobLoadModelData();
		ModelData* modelData = new ModelData();
		job->modelData = modelData;
		job->filename = filename;
		std::lock_guard<std::mutex> guard(jobVectorMutex);
		justSubmittedJobs.push_back(job);
		return modelData;
	}

	TextureData* assetLoadTextureData(std::string filename)
	{
		JobLoadTextureData* job = new JobLoadTextureData();
		TextureData* textureData = new TextureData();
		job->filename = filename;
		job->textureData = textureData;
		std::lock_guard<std::mutex> guard(jobVectorMutex);
		justSubmittedJobs.push_back(job);
		return textureData;
	}

	MaterialData* assetLoadMaterialData(std::string filename)
	{
		JobLoadMaterialData* job = new JobLoadMaterialData();
		MaterialData* materialData = new MaterialData();
		job->filename = filename;
		job->materialData = materialData;
		std::lock_guard<std::mutex> guard(jobVectorMutex);
		justSubmittedJobs.push_back(job);
		return materialData;
	}

	std::string loadText(std::string path)
	{
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::string err = "Could not open text file ";
			err.append(path);
			LOGERROR(err.c_str());
			return "";
		}

		std::string line;
		std::string content;
		while (std::getline(file, line))
		{
			content.append(line);
			content.append("\n");
		}
		content.append("\0");
		file.close();
		return content;
	}
}