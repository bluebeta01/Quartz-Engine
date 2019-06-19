#include "framework/pch.h"
#include "loader.h"
#include "framework/filesystem/filesystem.h"

namespace asset
{
	void loadModelFromDisk(JobLoadModelData* job)
	{
		std::string path = "models/";
		path.append(job->filename);
		path.append(".qmf");

		path = filesystem::findFilePathByName(path);

		FILE* file;
		fopen_s(&file, path.c_str(), "rb");

		if (!file)
		{
			std::string error = "Failed to open the file ";
			error.append(path);
			LOGERROR(error.c_str());
			return;
		}
		

		char headerText[16];
		fread_s(headerText, sizeof(headerText), 1, 16, file);


		int verison;
		fread_s(&verison, sizeof(verison), 1, sizeof(verison), file);


		int modelNameLength;
		fread_s(&modelNameLength, sizeof(modelNameLength), 1, sizeof(modelNameLength), file);


		char* modelName = new char[modelNameLength];
		fread_s(modelName, sizeof(char) * modelNameLength, 1, modelNameLength, file);


		int materialNameLength;
		fread_s(&materialNameLength, sizeof(materialNameLength), 1, sizeof(materialNameLength), file);


		char* materialName = new char[materialNameLength];
		fread_s(materialName, sizeof(char) * materialNameLength, 1, materialNameLength, file);


		int numberOfVerts;
		fread_s(&numberOfVerts, sizeof(numberOfVerts), 1, sizeof(numberOfVerts), file);


		int floatsPerVert;
		fread_s(&floatsPerVert, sizeof(floatsPerVert), 1, sizeof(floatsPerVert), file);


		float* vertexData = new float[floatsPerVert * numberOfVerts];
		fread_s(vertexData, sizeof(float) * floatsPerVert * numberOfVerts, 1, sizeof(float) * floatsPerVert * numberOfVerts, file);


		fclose(file);

		job->modelData->data = new float[numberOfVerts * floatsPerVert];
		memcpy_s(job->modelData->data, sizeof(float) * numberOfVerts * floatsPerVert, vertexData, sizeof(float) * numberOfVerts * floatsPerVert);
		std::string n = modelName;
		job->modelData->name = n;
		job->modelData->vertCount = numberOfVerts;
		job->modelData->floatsPerVert = floatsPerVert;
		job->modelData->materialName = materialName;
		job->modelData->loaded = true;

		delete vertexData;
		delete materialName;
		delete modelName;
	}
}