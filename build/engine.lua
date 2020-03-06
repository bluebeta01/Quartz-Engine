require("utils")

game = 
{

	CreateEntity = function(name)
		entity = utils.MakeEntity()
		entity.uid = c_createEntity(name)
		entity.SetName = name
		return entity
	end,

	GetEntityByName = function(name)
		local uid = c_getEntityByName(name)
		local entity = utils.MakeEntity()
		entity.uid = uid
		return entity
	end,

	GetEntityByUid = function(id)
		local entity = utils.MakeEntity()
		entity.uid = c_getEntityByUid(id)
		return entity
	end,

	GetEntityByEditorId = function(id)
		local uid = c_getEntityByEditorId(id)
		local entity = utils.MakeEntity()
		entity.uid = uid
		return entity
	end,

	GetModelByName = function(name)
		model = utils.MakeModel()
		model.uid = c_getModelByName(name)
		return model
	end,

	GetMaterialByName = function(name)
		material = utils.MakeMaterial()
		material.uid = c_getMaterialByName(name)
		return material
	end,

	GetTextureByName = function(name)
		texture = utils.MakeTexture()
		texture.uid = c_getTextureByName(name)
		return texture
	end


}