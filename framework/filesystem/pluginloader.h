#pragma once
#include "c_plugin.h"
#include "pch.h"

namespace filesystem
{
	Plugin* loadPlugin(std::string pluginName);
}