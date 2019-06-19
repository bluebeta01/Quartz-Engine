#include "framework/pch.h"
#include "filesystem.h"
#include "c_plugin.h"
#include "pluginloader.h"

namespace filesystem
{
	std::string pluginRoot = "plugins/";
	std::vector<Plugin*> pluginVector;

	void registerPlugin(std::string pluginName)
	{
		Plugin* plugin = loadPlugin(pluginName);
		if (plugin)
			pluginVector.push_back(plugin);
	}

	std::string findFilePathByName(std::string filenameFromPluginRoot)
	{
		for (int i = 0; i < pluginVector.size(); i++)
		{
			std::string path = pluginRoot;
			path.append(pluginVector[i]->uniqueName);
			path.append("/");
			path.append(filenameFromPluginRoot);

			FILE* file;
			fopen_s(&file, path.c_str(), "r");
			if (file)
			{
				std::string path = pluginRoot;
				path.append(pluginVector[i]->uniqueName);
				path.append("/");
				path.append(filenameFromPluginRoot);
				return path;
			}
		}
		std::string error = "Failed to find the file ";
		error.append(filenameFromPluginRoot);
		LOGERROR(error.c_str());
		return "";
	}
}