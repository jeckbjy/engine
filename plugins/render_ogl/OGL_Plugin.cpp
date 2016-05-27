#include "OGL_Plugin.h"
#include "OGL_API.h"
//#include "OGL_Device.h"
//#include "OGL_Mapping.h"
//#include "Root.h"

CU_NS_BEGIN

DEF_PLUGIN(CU_OGL_API, OGL_Plugin)

//OGLDevice* g_ogl_render = NULL;

void OGL_Plugin::install()
{
	//g_ogl_render = new OGLDevice();
	//g_ogl_render->init();
	//Root::Instance().addDevice(g_ogl_render);
}

void OGL_Plugin::uninstall()
{
	//Root::Instance().delDevice(g_ogl_render);
	//if (g_ogl_render != NULL)
	//{
	//	delete g_ogl_render;
	//	g_ogl_render = NULL;
	//}
}

CU_NS_END