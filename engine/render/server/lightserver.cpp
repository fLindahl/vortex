#include "config.h"
#include "lightserver.h"
#include "renderdevice.h"

namespace Render
{

LightServer::LightServer()
{

}

void LightServer::AddPointLight(const PointLight& pLight)
{
	this->pointLights.Append(pLight);
	RenderDevice::Instance()->UpdateLightBuffer();
}

}