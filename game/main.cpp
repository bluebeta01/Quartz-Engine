#include "framework/framework.h"
#include <thread>
#include <iostream>
#include <lua.hpp>
#include <lualib.h>

lua_State* state;
World* world;
Renderer* renderer;

int c_createEntity(lua_State* state)
{
	const char* name = lua_tostring(state, 1);
	Entity* e = world->createEntity(name);
	lua_pushinteger(state, e->m_uid);
	return 1;
}

int c_addEntityComponent(lua_State* state)
{
	int uid = lua_tointeger(state, 1);
	RenderComponent* rc = new RenderComponent();
	Entity* e = world->m_entityMap[uid];
	e->addComponent(rc);

	lua_newtable(state);
	return 1;
}

int c_setRenderComponentModel(lua_State* state)
{
	int uid = lua_tointeger(state, 1);
	Entity* e = world->m_entityMap[uid];
	RenderComponent* rc = (RenderComponent*)e->getComponent(Component::COMPONENT_TYPE_RENDER_COMPONENT);
	rc->setModel(renderer->getModel("Cube"));
	return 0;
}

int main()
{
	std::thread assetThread = asset::assetInit();
	filesystem::registerPlugin("game");
	screen::initializeScreen();

	world = new World();
	renderer = new Renderer(world);
	renderer->initialize(screen::windowHandle, 1280, 720);

	state = luaL_newstate();
	luaL_openlibs(state);

	int result = luaL_loadfile(state, "main.lua");
	if (result != LUA_OK)
	{
		const char* message = lua_tostring(state, -1);
		puts(message);
		lua_pop(state, 1);
	}

	//lua_pushstring(state, "c_createEntity");
	lua_pushcfunction(state, c_createEntity);
	lua_setglobal(state, "c_createEntity");
	lua_pushcfunction(state, c_addEntityComponent);
	lua_setglobal(state, "c_addEntityComponent");
	lua_pushcfunction(state, c_setRenderComponentModel);
	lua_setglobal(state, "c_setRenderComponentModel");
	
	lua_newtable(state);
	lua_newtable(state);
	lua_getglobal(state, "_G");
	lua_setfield(state, -2, "__index");
	lua_setmetatable(state, -2);
	lua_setfield(state, LUA_REGISTRYINDEX, "main");
	lua_getfield(state, LUA_REGISTRYINDEX, "main");
	lua_setupvalue(state, 1, 1);
	result = lua_pcall(state, 0, LUA_MULTRET, 0);
	if (result != LUA_OK)
	{
		const char* message = lua_tostring(state, -1);
		puts(message);
		lua_pop(state, 1);
	}



	



	lua_getfield(state, LUA_REGISTRYINDEX, "main");
	lua_getfield(state, -1, "main");
	lua_call(state, 0, 0);
	lua_pop(state, 1);


	while (!screen::terminated)
	{
		screen::updateScreen();
		renderer->setCamera(Camera(glm::vec3(0, 0, -10), glm::vec3(0, 90, 0), glm::radians(40.0f), 0.1f, 1000.0f, (float)renderer->m_width / (float)renderer->m_height));
		renderer->render();
		renderer->present();
	}


	assetThread.join();
}