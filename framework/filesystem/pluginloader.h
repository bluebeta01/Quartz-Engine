#pragma once
#include "c_plugin.h"
#include "framework/pch.h"

namespace filesystem
{
	Plugin* loadPlugin(std::string pluginName);
}