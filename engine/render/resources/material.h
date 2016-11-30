#pragma once
#include <memory>
#include "shaderobject.h"
#include "textureresource.h"
#include "foundation/util/array.h"
#include <map>
#include "foundation/util/variable.h"
//#include "foundation/util/string.h"

namespace Render
{
class Surface;

struct MaterialParameter
{
	Util::String name;
	Util::Variable var;
};

class Material
{
public:
	Material();
	~Material();

	// Used for sorting by shaderobject.
	//bool operator<(const Material& rhs) const;

    void SetName(const Util::String& name);
	Util::String GetName();

	///returns shader from specified pass
	std::shared_ptr<ShaderObject> GetShader(const std::string& pass);

    Util::Array<std::shared_ptr<TextureResource>>& TextureList();

	MaterialParameter* GetParameterByName(const Util::String& name);
	void AddParameter(const Util::String& name, const Util::Variable& variable);
	
    Util::Array<std::shared_ptr<Surface>>& SurfaceList();

	void SetFramePass(const Util::String& pass, const Util::String& shader);
	//std::string GetFramePasses();
	
	Util::Array<MaterialParameter*>& ParameterList() { return this->parameters; }

	//TEMPORARY
	Util::Array<ModelInstance*>& getModelInstances() { return this->modelInstances; }

private:
	friend class ResourceServer;

	///TEMPORARY
	Util::Array<ModelInstance*> modelInstances;

	/// name of the material
	Util::String name;

	/// What framepasses are we rendering this material in and with what shader?
	std::map<Util::String, std::shared_ptr<ShaderObject>> framepasses;

	/// loaded textures
	Util::Array<std::shared_ptr<TextureResource>> textures;
	
	std::map<Util::String, MaterialParameter*> parametersByName;
	Util::Array<MaterialParameter*> parameters;

	/// all surfaces that currently use this material
	Util::Array<std::shared_ptr<Surface>> surfaces;
};

}