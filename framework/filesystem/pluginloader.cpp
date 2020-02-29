#include "pch.h"
#include "pluginloader.h"

namespace filesystem
{
	extern std::string pluginRoot;

	Plugin* loadPlugin(std::string pluginName)
	{
		std::string path = pluginRoot;
		path.append(pluginName);
		path.append("/");
		path.append(pluginName);
		path.append(".qpf");

		FILE* pluginFile;
		fopen_s(&pluginFile, path.c_str(), "r");
		if (!pluginFile)
		{
			std::string error = "Failed to load plugin ";
			error.append(path);
			LOGERROR(error.c_str());
			return NULL;
		}

		std::string qpfSource;
		while (!feof(pluginFile))
		{
			char c;
			fread_s(&c, 1, 1, 1, pluginFile);
			qpfSource += c;
		}
		qpfSource.pop_back();
		fclose(pluginFile);

		nlohmann::json json;
		try
		{
			json = nlohmann::json::parse(qpfSource);
		}
		catch (const std::exception&)
		{
			std::string error = "Failed to parse plugin file ";
			error.append(path);
			LOGERROR(error.c_str());
			return NULL;
		}
		
		nlohmann::json::string_t friendlyName = json["friendlyName"];
		nlohmann::json::string_t author = json["author"];
		nlohmann::json::string_t version = json["version"];
		nlohmann::json::string_t desc = json["desc"];

		Plugin* plugin = new Plugin();
		plugin->uniqueName = pluginName;
		plugin->friendlyName = friendlyName;
		plugin->author = author;
		plugin->version = version;
		plugin->desc = desc;

		return plugin;
	}
}