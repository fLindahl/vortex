#pragma once
#include <memory>
#include "config.h"
#include "shaderobject.h"
#include "textureresource.h"
#include "foundation/util/array.h"
#include <map>
#include "foundation/util/variable.h"
//#include "foundation/util/string.h"
#include "render/resources/surface.h"

namespace Render
{

struct MaterialParameter
{
	Util::String name;
	Util::Variable var;
};

class Material : public Core::RefCounted
{
	__DeclareClass(Material);
public:
	Material();
	~Material();

	// Used for sorting by shaderobject.
	//bool operator<(const Material& rhs) const;

    void SetName(const Util::String& name);
	Util::String GetName();

	///returns shader from specified pass
	Ptr<ShaderObject> GetShader(const Util::String& pass);

    Util::Array<Ptr<TextureResource>>& TextureList();

	MaterialParameter* GetParameterByName(const Util::String& name);
	void AddParameter(const Util::String& name, const Util::Variable& variable);
	
    Util::Array<Ptr<Surface>>& SurfaceList();

	void SetFramePass(const Util::String& pass, const Util::String& shader);
	//std::string GetFramePasses();
	
	Util::Array<MaterialParameter*>& ParameterList() { return this->parameters; }

	//TEMPORARY
	Util::Array<ModelInstance*>& getModelInstances() { return this->modelInstances; }

	static TextureType TextureTypeFromString(const std::string& s);

private:
	friend class ResourceServer;

	///TEMPORARY
	Util::Array<ModelInstance*> modelInstances;

	/// name of the material
	Util::String name;

	/// What framepasses are we rendering this material in and with what shader?
	std::map<Util::String, Ptr<ShaderObject>> framepasses;

	/// loaded textures
	Util::Array<Ptr<TextureResource>> textures;
	/// This is adjacent with textures array. e.g. textures[1]'s type == TextureParamTypes[1].
	Util::Array<TextureType> TextureParamTypes;
	
	std::map<Util::String, MaterialParameter*> parametersByName;
	Util::Array<MaterialParameter*> parameters;

	/// all surfaces that currently use this material
	Util::Array<Ptr<Surface>> surfaces;
};

}