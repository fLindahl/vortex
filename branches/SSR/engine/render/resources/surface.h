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

		///Get surface name
		Util::String GetName();
		Util::String GetPath() { return this->filePath; }

		///Get surface material
		std::shared_ptr<Material> GetMaterial();

		///Appends a modelnode to this surface. Only use if you know what you're doing!
		void AppendModelNode(ModelNode* node) { this->modelNodes.Append(node); }
		///Remove given modelnode from surface
		bool RemoveModelNode(ModelNode* node);
		
		///Get texture list
		Util::Array<std::shared_ptr<TextureResource>>& TextureList();

		MaterialParameter* GetParameterByName(const Util::String& name);

		Util::Array<MaterialParameter*>& ParameterList() { return this->parameters; }

		Util::Array<ModelNode*>& GetModelNodes() { return this->modelNodes; }

	private:
		friend class Material;
        friend class ResourceServer;

		/// name of the surface
		Util::String name;
		Util::String filePath;

		/// surface material
		std::shared_ptr<Material> material;

		
		/// loaded textures
		std::map<TextureType, std::shared_ptr<TextureResource>> texturesByType;
		Util::Array<std::shared_ptr<TextureResource>> textures;

		std::map<Util::String, MaterialParameter*> parametersByName;
		Util::Array<MaterialParameter*> parameters;

		//TODO: Implement surface instances
		// all instances of this surface
		//Util::Array<std::shared_ptr<SurfaceInstance>> surfaceInstances;

		///Contains all modelinstances that point to this surface and the primitivegroup that uses it.
		Util::Array<ModelNode*> modelNodes;

        void AddParameter(const Util::String &name, const Util::Variable &variable);
    };
}