#pragma once
#include "Platform.h"

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
class RenderTarget;
class ShaderStage;
class DescriptorSet;
class Pipeline;
class CommandBuffer;
class CommandQueue;
class Device;
class Graphics;

typedef GpuBuffer VertexBuffer;
typedef GpuBuffer IndexBuffer;
typedef GpuBuffer UniformBuffer;

// render
class Playable;
class Animatable;

class View;
struct RenderCommand;
struct RenderTargetInfo;
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

// ÄÚ²¿ÀàID
enum ObjectID
{
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
};

CU_NS_END
