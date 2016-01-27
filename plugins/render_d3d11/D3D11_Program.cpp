#include "D3D11_Program.h"

CU_NS_BEGIN

static D3D11Program* g_program = NULL;

D3D11Program::D3D11Program()
: _vs(NULL)
, _ps(NULL)
, _gs(NULL)
, _ds(NULL)
, _hs(NULL)
, _cs(NULL)
{

}

D3D11Program::~D3D11Program()
{
	D3D11_RELEASE(_vs);
	D3D11_RELEASE(_ps);
	D3D11_RELEASE(_gs);
	D3D11_RELEASE(_ds);
	D3D11_RELEASE(_hs);
	D3D11_RELEASE(_cs);
}

bool D3D11Program::attach(ShaderType type, const String& data, const String& name, const String& entry, ShaderProfile profile, bool binary)
{
	return false;
}

bool D3D11Program::link()
{
	return true;
}

void D3D11Program::bind(ID3D11DeviceContextN* context)
{
	if (g_program == this)
		return;
	g_program = this;
	if (_vs != NULL)
		context->VSSetShader(_vs, NULL, 0);
	if (_ps != NULL)
		context->PSSetShader(_ps, NULL, 0);
	if (_gs != NULL)
		context->GSSetShader(_gs, NULL, 0);
	if (_ds != NULL)
		context->DSSetShader(_ds, NULL, 0);
	if (_hs != NULL)
		context->HSSetShader(_hs, NULL, 0);
	if (_cs != NULL)
		context->CSSetShader(_cs, NULL, 0);
}

void D3D11Program::unbind(ID3D11DeviceContextN* context)
{
	if (g_program == this)
		g_program = NULL;
	if (_vs != NULL)
		context->VSSetShader(NULL, NULL, 0);
	if (_ps != NULL)
		context->PSSetShader(NULL, NULL, 0);
	if (_gs != NULL)
		context->GSSetShader(NULL, NULL, 0);
	if (_ds != NULL)
		context->DSSetShader(NULL, NULL, 0);
	if (_hs != NULL)
		context->HSSetShader(NULL, NULL, 0);
	if (_cs != NULL)
		context->CSSetShader(NULL, NULL, 0);
}

CU_NS_END