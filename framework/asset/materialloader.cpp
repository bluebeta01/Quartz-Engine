#include "pch.h"
#include "materialloader.h"
#include "filesystem/filesystem.h"

namespace asset
{
	void loadMaterialFromDisk(JobLoadMaterialData* job)
	{
		std::string path = "materials/";
		path.append(job->filename);
		path.append(".qmat");

		path = filesystem::findFilePathByName(path);

		FILE* file;
		fopen_s(&file, path.c_str(), "r");
		if (!file)
		{
			std::string error = "Unable to open file ";
			error.append(path);
			LOGERROR(error.c_str());
			return;
		}

		std::string fileData;
		while (!feof(file))
		{
			char c;
			fread_s(&c, sizeof(char), 1, sizeof(char), file);
			fileData += c;
		}
		fileData.pop_back();
		
		fclose(file);

		nlohmann::json json;
		try
		{
			json = nlohmann::json::parse(fileData.c_str());
		}
		catch (const std::exception&)
		{
			std::string error = "Unable to parse material file ";
			error.append(path);
			LOGERROR(error.c_str());
			return;
		}
		nlohmann::json::string_t diffuseName = json["diffuse"];
		nlohmann::json::string_t ormName = json["orm"];
		nlohmann::json::string_t normalName = json["normal"];

		job->materialData->diffuseTextureName = diffuseName.c_str();
		job->materialData->ormTextureName = ormName.c_str();
		job->materialData->normalTextureName = normalName.c_str();

		job->materialData->loaded = true;
	}
}