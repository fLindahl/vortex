#pragma once
#include "material.h"

namespace Render
{
	class Surface
	{
	public:
		Surface();
		~Surface();

		//Load surface from file
		void SetupSurface(const Util::String& file);

		//Get surface name
		Util::String GetName();

		//Get surface material
		std::shared_ptr<Material> GetMaterial();
		
		//Get texture list
		Util::Array<std::shared_ptr<TextureResource>>& TextureList();

		MaterialParameter* GetParameterByName(const Util::String& name);

		Util::Array<ModelInstance*>& getModelInstances() { return this->modelInstances; }

	private:
		friend class Material;
        friend class ResourceServer;

		// name of the surface
		Util::String name;

		// surface material
		std::shared_ptr<Material> material;

		// loaded textures
		Util::Array<std::shared_ptr<TextureResource>> textures;

		std::map<Util::String, MaterialParameter*> parametersByName;
		Util::Array<MaterialParameter*> parameters;

		//TODO: Implement surface instances
		// all instances of this surface
		//Util::Array<std::shared_ptr<SurfaceInstance>> surfaceInstances;

		Util::Array<ModelInstance*> modelInstances;

        void AddParameter(const Util::String &name, const Util::Variable &variable);
    };
}