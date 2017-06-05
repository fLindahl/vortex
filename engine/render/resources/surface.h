#pragma once
#include "config.h"
#include "material.h"

namespace Render
{
	struct MaterialParameter;
	
	//TODO: This should also denote location in shader, but isn't currently. We assume everything is in this order in the material list.
	enum TextureType
	{
		AlbedoMap = 0,
		NormalMap = 1,
		SpecularMap = 2,
		RoughnessMap = 3
	};

	class Surface : public Core::RefCounted
	{
	__DeclareClass(Surface);
	public:
		Surface();
		~Surface();

		//Load surface from file
		void SetupSurface(const Util::String& file);

		///Get surface name
		Util::String GetName();
		Util::String GetPath() { return this->filePath; }

		///Get surface material
		Ptr<Material> GetMaterial();

		///Appends a modelnode to this surface. Only use if you know what you're doing!
		void AppendModelNode(ModelNode* node) { this->modelNodes.Append(node); }
		///Remove given modelnode from surface
		bool RemoveModelNode(ModelNode* node);
		
		///Get texture list
		Util::Array<Ptr<TextureResource>>& TextureList();

		Render::MaterialParameter* GetParameterByName(const Util::String& name);

		Util::Array<Render::MaterialParameter*>& ParameterList() { return this->parameters; }

		Util::Array<ModelNode*>& GetModelNodes() { return this->modelNodes; }

	private:
		friend class Material;
        friend class ResourceServer;

		/// name of the surface
		Util::String name;
		Util::String filePath;

		/// surface material
		Ptr<Material> material;

		
		/// loaded textures
		std::map<TextureType, Ptr<TextureResource>> texturesByType;
		Util::Array<Ptr<TextureResource>> textures;

		std::map<Util::String, MaterialParameter*> parametersByName;
		Util::Array<MaterialParameter*> parameters;

		//TODO: Implement surface instances
		// all instances of this surface
		//Util::Array<Ptr<SurfaceInstance>> surfaceInstances;

		///Contains all modelinstances that point to this surface and the primitivegroup that uses it.
		Util::Array<ModelNode*> modelNodes;

        void AddParameter(const Util::String &name, const Util::Variable &variable);
    };
}