#pragma once
#include "Platform.h"
#include "SmartPtr.h"

CU_NS_BEGIN

// core
class Object;
class Stream;
class Log;

class DynLib;
class SpinLock;
class Mutex;
class Thread;
class Process;
class Condition;

class XMLNode;
class XMLFile;

// system
class Context;

// math
template<typename T>
class TVector2;
template<typename T>
class TVector3;
template<typename T>
class TVector4;

typedef TVector2<float>	Vector2;
typedef TVector3<float> Vector3;
typedef TVector4<float> Vector4;

class Matrix3;
class Matrix4;
class Matrix3x4;
class Quaternion;

class Ray;
class Plane;
class AABox;
class Sphere;
class Frustum;

// net
class Socket;
class SocketAddress;

class IOService;
class Channel;
class AcceptChannel;
class SocketChannel;
class IProtocol;
class Session;
class NetService;
class HandlerMgr;

// graphics
class Color;
class GpuBuffer;
class Texture;
class VertexLayout;
class VertexArray;
class RenderTarget;
class FrameBuffer;
class SwapChain;
class ShaderStage;
class ShaderProgram;
class DescriptorSet;
class Pipeline;
class CommandBuffer;
class CommandQueue;
class Device;
class Graphics;

// render
class Playable;
class Animatable;

class View;
class RenderPath;
class Renderer;
class Engine;
class SceneManager;

class Entity;
class Component;
class Drawable;
class Transform;
class Camera;
class Light;
class Canvas;

class Geometry;
class Model;
class ModelRender;

class Skeleton;
class Animation;
class Animator;

class Shader;
class Material;

class Octant;
class Octree;
class OctreeQuery;

// ui
class Window;
class Application;

// some used SharedPtr
typedef SharedPtr<GpuBuffer>		GpuBufferPtr;
typedef SharedPtr<Texture>			TexturePtr;
typedef SharedPtr<VertexLayout>		VertexLayoutPtr;
typedef SharedPtr<VertexArray>		VertexArrayPtr;
typedef SharedPtr<RenderTarget>		RenderTargetPtr;
typedef SharedPtr<FrameBuffer>		FrameBufferPtr;
typedef SharedPtr<SwapChain>		SwapChainPtr;
typedef SharedPtr<ShaderStage>		ShaderStagePtr;
typedef SharedPtr<ShaderProgram>	ShaderProgramPtr;
typedef SharedPtr<DescriptorSet>	DescriptorSetPtr;
typedef SharedPtr<Pipeline>			PipelinePtr;
typedef SharedPtr<CommandBuffer>	CommandBufferPtr;
typedef SharedPtr<CommandQueue>		CommandQueuePtr;
typedef SharedPtr<Device>			DevicePtr;

typedef SharedPtr<Window>			WindowPtr;

// ÄÚ²¿ÀàID
enum ObjectID
{
	OBJ_ID_OBJECT,
	OBJ_ID_WINDOW,
	//
	OBJ_ID_GRAPHICS,
	OBJ_ID_GPU_RESOURCE,
	OBJ_ID_GPU_BUFFER,
	OBJ_ID_TEXTURE,
	OBJ_ID_INPUT_LAYOUT,
	OBJ_ID_RENDER_TARGET,
	OBJ_ID_FRAMEBUFFER,
	OBJ_ID_SWAPCHAIN,
	OBJ_ID_SHADER_STAGE,
	OBJ_ID_SHADER_PROTRAM,
	OBJ_ID_DESCRIPTOR_SET,
	OBJ_ID_PIPELINE,
	OBJ_ID_RENDER_PASS,
	OBJ_ID_COMMAND_BUFFER,
	OBJ_ID_COMMAND_QUEUE,
	OBJ_ID_FENCE,
	OBJ_ID_DEVICE,

	// 
	OBJ_ID_COMPONENT,
	OBJ_ID_CAMERA,
	OBJ_ID_LIGHT,
	OBJ_ID_ANIMATOR,

	// plugin
	OBJ_ID_PLUGIN_START = 1000,
	OBJ_ID_PLUGIN_D3D9  = OBJ_ID_PLUGIN_START,
	OBJ_ID_PLUGIN_D3D11 = OBJ_ID_PLUGIN_D3D9  + 100,
	OBJ_ID_PLUGIN_D3D12 = OBJ_ID_PLUGIN_D3D11 + 100,
	OBJ_ID_PLUGIN_OGL	= OBJ_ID_PLUGIN_D3D12 + 100,
	OBJ_ID_PLUGIN_VULKAN = OBJ_ID_PLUGIN_OGL + 100,

	OBJ_ID_CUSTOM = 10000,
};

CU_NS_END
