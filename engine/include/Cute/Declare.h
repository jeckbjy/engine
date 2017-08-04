#pragma once
#include "Foundation.h"

CUTE_NS_BEGIN

enum ObjectID
{
	OID_ANY = 0,
	OID_OBJECT = 1,
	OID_WINDOW,

	// graphics
	OID_GRAPHICS,
	OID_GPU_RESOURCE,
	OID_GPU_BUFFER,
	OID_TEXTURE,
	OID_INPUT_LAYOUT,
	OID_RENDER_TARGET,
	OID_FRAMEBUFFER,
	OID_SWAPCHAIN,
	OID_SHADER_STAGE,
	OID_SHADER_PROTRAM,
	OID_DESCRIPTOR_SET,
	OID_PIPELINE,
	OID_RENDER_PASS,
	OID_COMMAND_BUFFER,
	OID_COMMAND_QUEUE,
	OID_FENCE,
	OID_DEVICE,

	//
    OID_SCENE,
	OID_ENTITY,
	OID_COMPONENT,
	OID_TRANSFORM,
	OID_CAMERA,
	OID_LIGHT,
	OID_ANIMATOR,

	// plugin
	OID_PLUGIN_START	= 1000,
	OID_PLUGIN_D3D9		= OID_PLUGIN_START,
	OID_PLUGIN_D3D11	= OID_PLUGIN_D3D9 + 100,
	OID_PLUGIN_D3D12	= OID_PLUGIN_D3D11 + 100,
	OID_PLUGIN_OGL		= OID_PLUGIN_D3D12 + 100,
	OID_PLUGIN_VULKAN	= OID_PLUGIN_OGL + 100,

	OID_CUSTOM = 10000,
};

// core
class Stream;

// application
class Window;
class Application;

// Math
class Matrix3;
class Matrix4;
class Quaternion;
class Plane;
class AABox;
class Sphere;
class Ray;

// Graphics
class IBuffer;
class ITexture;
class ISampler;
class ISwapChain;
class IFrameBuffer;
class IVertexLayout;
class IDescriptorSet;
class IProgram;
class IPipeline;
class IDevice;

typedef IBuffer VertexBuffer;
typedef IBuffer IndexBuffer;
typedef IBuffer UniformBuffer;

// Render
class Shader;
class Material;
class Mesh;

// animation
class AnimationCurve;
class AnimationClip;

// scene,entity,component
class Scene;
class SceneNode;
class Entity;
class Component;
class Transform;
class Camera;
class Light;
class Renderable;
//class MeshRender;
//class SkinnedRender;

CUTE_NS_END
