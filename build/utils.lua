utils =
{

MakeModel = function()
	local model = {
		GetName = function(self)
			return c_getModelName(self.uid)
		end,

		GetMaterial = function(self)
			material = utils.MakeMaterial()
			material.uid = c_getModelMaterial(self.uid)
			return material
		end,

		SetMaterial = function(self, material)
			c_setModelMaterial(self.uid, material.uid)
		end
	}
	return model
end,

MakeMaterial = function()
	local material = {
		GetName = function(self)
			return c_getMaterialName(self.uid)
		end,

		SetTextureSlot = function(self, slotName)
			c_setMaterialTextureSlot(self.uid, slotName)
		end,

		GetTextureSlot = function(self, slotName)
			texture = utils.MakeTexture()
			texture.uid = c_getMaterialTextureSlot(self.uid, slotName)
			return texture
		end
	}
	return material
end,

MakeTexture = function()
	local texture = {
		GetName = function(self)
			return c_getTextureName(self.uid)
		end
	}
	return texture
end,

MakeComponent = function()
	local component = {
		GetEnabled = function(self)
			return c_getComponentEnabled(self.uid, self.type)
		end,

		GetModel = function(self)
			uid = c_getRenderComponentModel(self.uid)
			model = utils.MakeModel()
			model.uid = uid
			return model
		end,

		SetModel = function(model)
			c_setRenderComponentModel(uid, model.uid)
			return
		end,

		GetMaterial = function(self)
			uid = c_getRenderComponentMaterial(self.uid)
			materail = utils.MakeMaterial()
			material.uid = uid
			return material
		end,

		SetMaterial = function(self, material)
			c_setRenderComponentMaterial(self.uid, material.uid)
		end

	}

	return component
end,

MakeEntity = function()
	local entity = {

		GetName = function(self)
			return c_getEntityName(self.uid)
		end,

		SetName = function(self, name)
			c_setEntityName(self.uid, name)
		end,

		GetEditorId = function(self)
			return c_getEntityEditorId(self.uid)
		end,

		GetTransform = function(self)
			return c_getEntityTransform(self.uid)
		end,

		SetTransform = function(self, transform)
			c_setEntityTransform(self.uid, transform)
		end,

		GetChildren = function(self)
			local uids = c_getEntityChildren(self.uid)
			local entities = {}
			for i=1, #uids, 1 do
				entities[i] = utils.MakeEntity()
				entities[i].uid = uids[i]
			end
			return entities
		end,

		GetChildByName = function(self, name)
			local entities = self.GetChildren()
			for i=1, #entities, 1 do
				if entities[i].GetName() == name then
					return entities[i]
				end
			end
		end,

		GetChildByUid = function(self, uid)
			local entities = self.GetChildren()
			for i=1, #entities, 1 do
				if entities[i].uid == uid then
					return entities[i]
				end
			end
		end,

		GetChildByEditorId = function(self, id)
			local entities = self.GetChildren()
			for i=1, #entities, 1 do
				if entities[i].GetEditorId() == id then
					return entities[i]
				end
			end
		end,

		AddChild = function(self, entity)
			c_addEntityChild(entity.uid, entity.uid)
		end,

		RemoveChild = function(self, entity)
			c_removeEntityChild(entity.uid, entity.uid)
		end,

		GetWorldPosition = function(self)
			return c_getEntityWorldPosition(self.uid)
		end,

		SetWorldPosition = function(self, x, y, z)
			c_setWorldPosition(self.uid, {x,y,z})
		end,

		GetComponent = function(self, type)
			local component = utils.MakeComponent()
			local value = c_getEntityComponent(self.uid, type)
			component.type = value.type
			component.uid = value.uid
			return component
		end,

		AddComponent = function(self, type)
			local value = c_addEntityComponent(self.uid, type)
			component = utils.MakeComponent()
			component.uid = value.uid
			component.type = value.type
			return component
		end,

		RemoveComponent = function(self, type)
			c_removeEntityComponent(self.uid, type)
		end,

		GetScript = function(self, name)
			return c_getEntityScript(self.uid, name)
		end,

		AddScript = function(self, name)
			c_addEntityScript(self.uid, name)
		end,

		RemoveScript = function(self, name)
			c_removeEntityScript(self.uid, name)
		end


	}

	return entity
end


}