#pragma once
#include "framework/pch.h"

namespace filesystem
{
	std::string findFilePathByName(std::string filenameFromPluginRoot);
	void registerPlugin(std::string pluginName);
}