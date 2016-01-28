-- 用法：premake4.exe gmake(vs2010,vs2012,...)
--[[
启动程序并加载so，设置查找路径，另外一种方法，添加-rpath编译时指定查找目录
export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH
]]
local bin_dir = "build/bin/%{cfg.buildcfg}"
local lib_dir = "build/bin/%{cfg.buildcfg}"
local obj_dir = "build/obj/%{cfg.buildcfg}/%{prj.name}"

local src_dir = nil

-- 查找并自动添加路径
function my_vpaths(files)
	local result = {}
	for k,v in pairs(files) do
		local array = {}
		result[k] = array
		for _,file in pairs(v) do
			if string.sub(file, -2, -1) == ".h" then
				table.insert(array, "engine/include/"..file)
			elseif string.sub(file, -4, -1) == ".cpp" then
				table.insert(array, "engine/src/"..file)
			end
		end
	end
	for k, v in pairs(result) do
		print(k)
		for _, file in pairs(v) do
			print(file)
		end
	end
	vpaths(result)
end

function engine_vpaths()
	my_vpaths {
	["core"] = {
		"API.h",
		"Macro.h",
		"Platform.h",
		"Algo.h",
		"Bits.h",
		"Types.h",
		"Types.cpp",
		"List.h",
		"Array.h",
		"Tree.h",
		"Tuple.h",
		"Traits.h",
		"FuncTraits.h",
		"Function.h",
		"Delegate.h",
		"Singleton.h",
		"Range.h",
		"FourCC.h",
		"Util.h",
		"Util.cpp",
		"Ref.h",
		"Ref.cpp",
		"SmartPtr.h",
		"Variant.h",
		"Variant.cpp",
		"Attribute.h",
		"Rtti.h",
		"Rtti.cpp",
		-- todo:合并?StringUtil->Util StringPiece->Range
		"StringPiece.h",
		"StringPiece.cpp",
		"StringUtil.h",
		"StringUtil.cpp",
		"Buffer.h",
		"Buffer.cpp",
		"Config.h",
		"Config.cpp",
		"File.h",
		"File.cpp",
		"Stream.h",
		"Stream.cpp",
		"Atomic.h",
		"Console.h",
		"Console.cpp",
		"LogChannel.h",
		"LogChannel.cpp",
		"Log.h",
		"Log.cpp",
	},
	["os"] = {
		"Mutex.h",
		"Mutex.cpp",
		"Condition.h",
		"Condition.cpp",
		"SyncEvent.h",
		"SyncEvent.cpp",
		"Thread.h",
		"Thread.cpp",
		"CProcess.h",
		"CProcess.cpp",
		"DynLib.h",
		"DynLib.cpp",
	},
	["math"] = {
		"CMath.h",
		"CMath.cpp",
		"Rect.h",
		"Vector2.h",
		"Vector3.h",
		"Vector4.h",
		"Matrix4.h",
		"Matrix4.cpp",
		"Quaternion.h",
		"Quaternion.cpp",
		"Plane.h",
		"Plane.cpp",
		"Ray.h",
		"Ray.cpp",
		"Frustum.h",
		"Frustum.cpp",
	},
	["net"] = {
		"AcceptChannel.h",
		"AcceptChannel.cpp",
		"Acceptor.h",
		"Acceptor.cpp",
		"Channel.h",
		"Channel.cpp",
		"HandlerMgr.h",
		"HandlerMgr.cpp",
		"IOService.h",
		"IOService.cpp",
		"IOServicePool.h",
		"IOServicePool.cpp",
		"IOOperation.h",
		"IOOperation.cpp",
		"Poller.h",
		"Poller.cpp",
		"Procotol.h",
		"Procotol.cpp",
		"Proto.h",
		"Proto.cpp",
		"NetEvent.h",
		"NetEvent.cpp",
		"NetService.h",
		"NetService.cpp",
		"Session.h",
		"Session.cpp",
		"Socket.h",
		"Socket.cpp",
		"SocketAddress.h",
		"SocketAddress.cpp",
		"SocketChannel.h",
		"SocketChannel.cpp",
		"Packet.h",
		"Packet.cpp",
		"Protocol.h",
		"Protocol.cpp",
	},
	["engine"] = {
		"Event.h",
		"Plugin.h",
		"Object.h",
		"Object.cpp",
		"Context.h",
		"Context.cpp",
		"WorkQueue.h",
		"WorkQueue.cpp",
		"Asset.h",
		"AssetCache.h",
		"AssetCache.cpp",
		"Engine.h",
		"Engine.cpp",
	},
	["graphics"] = {
		"Color.h",
		"Color.cpp",
		"PixelFormat.h",
		"PixelFormat.cpp",
		"Graphics.h",
		"Graphics.cpp",
		"GraphicsDefs.h",
		"GraphicsDesc.h",
		"GraphicsDesc.cpp",
		"VertexLayout.h",
		"VertexLayout.cpp",
	},
	["render"] = {
		"Animatable.h",
		"Animatable.cpp",
		"Animation.h",
		"Animation.cpp",
		"Animator.h",
		"Animator.cpp",
		"Playable.h",
		"Playable.cpp",
		"PlayController.h",
		"PlayController.cpp",
		"Renderer.h",
		"RenderPath.h",
		"RenderPath.cpp",
		"RenderQueue.h",
		"RenderQueue.cpp",
		"View.h",
		"View.cpp",
		"Scene.h",
		"Scene.cpp",
		"Entity.h",
		"Entity.cpp",
		"Component.h",
		"Component.cpp",
		"Transform.h",
		"Transform.cpp",
		"Drawable.h",
		"Drawable.cpp",
		"Canvas.h",
		"Canvas.cpp",
		"Camera.h",
		"Camera.cpp",
		"Light.h",
		"Light.cpp",
		"Model.h",
		"Skeleton.h",
		"Skeleton.cpp",
		"Model.h",
		"Model.cpp",
		"ModelRender.h",
		"ModelRender.cpp",
		"Material.h",
		"Material.cpp",
		"Shader.h",
		"Shader.cpp",
	},
	["ui"] = {
		"Application.h",
		"Application.cpp",
		"Window.h",
		"Window.cpp",
		"UIView.h",
		"UIWidget.h"
		--"UIElement.h",
		--"UIElement.cpp",
	},
	["audio"] = {
	},
}
end

workspace "engine"
	location("solution")
	--toolset "v140"
	--basedir("solution")
	--startproject "engine"
	startproject "unit"
	configurations {"Debug", "Release"}
	language "c++"
	
	-- 全局配置
	includedirs {"engine/include"}
	targetdir(bin_dir)
	libdirs(lib_dir)
	objdir(obj_dir)
	debugdir(bin_dir)
	-- 关闭警告
	filter {"action:vs*"}
		defines{ "_CRT_SECURE_NO_WARNINGS" }
	filter {}
	-- 调试信息
	filter "configurations:Debug"
		defines { "DEBUG" }
		flags 	{ "Symbols" }
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
	filter {}
	-- 特殊规则
	filter {"kind:StaticLib"}
		targetdir(lib_dir)
	filter {"action:vs*", "kind:SharedLib"}
		implibdir(lib_dir)
	filter {"action:gmake"}
		buildoptions {"-std=c++11", "-fpermissive", "-g -ggdb -Wall"}
		links { "pthread", "dl"}
	filter {"action:gmake","kind:SharedLib"}
		buildoptions {"-fPIC"}
	filter {}
-- 主工程
project("engine")
	defines {"CU_BUILD_DLL"}
	kind (iif(_ACTION =="gmake","StaticLib", "SharedLib"))
	files {"engine/include/**.*", "engine/src/**.*"}
	engine_vpaths()

group "render"
	project("plugin_ogl")
		src_dir = "plugins/render_ogl/"
		dependson {"engine"}
		kind("SharedLib")
		defines {"CU_OGL_BUILD", "GLEW_BUILD", "GLEW_NO_GLU"}
		--includedirs {src_dir}
		if os.is("windows") then
			includedirs {src_dir.."glew/include"}
			files { src_dir .. "**.*"}
			vpaths{
				["glew"] = {src_dir.."glew/**.*"},
				["src"]  = {src_dir.."**.*"}
			}
		else
			--不用递归
			files { src_dir .. "/src/*.*"}
			vpaths { ["src"] = {src_dir.. "**.*"} }
		end
				
	project("plugin_vulkan")
		src_dir = "plugins/render_vulkan/"
		dependson { "engine" }
		kind("SharedLib")
		defines { "CU_VULKAN_BUILD" }
		--includedirs { src_dir}
		files { src_dir .. "**.*" }
		vpaths { ["src"] = {src_dir.. "**.*"} }
		
	project("plugin_d3d11")
		src_dir = "plugins/render_d3d11/"
		dependson { "engine" }
		kind("SharedLib")
		defines { "CU_D3D11_BUILD" }
		--includedirs { src_dir}
		includedirs { "D:/Program Files (x86)/Microsoft DirectX SDK (August 2009)/Include" }
		libdirs	{"D:\Program Files (x86)\Microsoft DirectX SDK (August 2009)\Lib\x64"}
		files { src_dir .. "**.*" }
		vpaths { ["src"] = {src_dir.. "**.*"} }
		
	project("plugin_d3d12")
		src_dir = "plugins/render_d3d12/"
		dependson { "engine" }
		kind("SharedLib")
		defines { "CU_D3D12_BUILD" }
		--includedirs { src_dir }
		includedirs { "D:/Program Files (x86)/Windows Kits/10/Include/10.0.10069.0/um", "D:/Program Files (x86)/Windows Kits/10/Include/10.0.10069.0/shared"}
		libdirs { "D:\Program Files (x86)\Windows Kits\10\Lib\10.0.10069.0\um\x64"}
		files { src_dir .. "**.*" }
		vpaths { ["src"] = {src_dir.. "**.*"} }
--[[
group "importer"
	project("plugin_fbx")
		src_dir = "plugins/fbx/"
		dependson { "engine" }
		kind("SharedLib")
		defines { "CU_FBX_BUILD" }
		includedirs { src_dir .. "include" }
		includedirs { "dependencies/fbx/include" }
		libdirs {"dependencies/fbx/lib/x86/%{cfg.buildcfg}"}
		links { "libfbxsdk-md.lib" }
		files { src_dir .. "**.*" }
		vpaths{
			["src"] = { src_dir .. "**.*" }
		}
		
	project("plugin_assimp")
		src_dir = "plugins/assimp/"
		dependson { "engine" }
		kind("SharedLib")
		defines { "CU_ASSIMP_BUILD" }
		includedirs { src_dir .. "include" }
		includedirs { "dependencies/assimp/include" }
		libdirs {"dependencies/assimp/lib"}
		links { "assimp.lib", "zlibd"}
		files { src_dir .. "**.*" }
		vpaths{
			["src"] = { src_dir .. "**.*" }
		}
		
	project("plugin_freeimage")
		src_dir = "plugins/freeimage/"
		dependson { "engine" }
		kind("SharedLib")
		defines { "CU_FREEIMAGE_BUILD" }
		includedirs { src_dir .. "include" }
		includedirs { "dependencies/freeimage/include" }
		libdirs {"dependencies/freeimage/lib/x86/%{cfg.buildcfg}"}
		links { "FreeImage.lib" }
		files { src_dir .. "**.*" }
		vpaths{
			["src"] = { src_dir .. "**.*" }
		}
	]]
group "test"
	project "test_engine"
		src_dir = "sample/test_engine/"
		--debugdir( bin_dir )
		dependson {"engine"}
		kind "WindowedApp"
		links { "engine" }
		--links {"Engine", "assimp", "zlibd" }
		--includedirs { "dependencies/assimp/include" }
		--includedirs { "sample/glm" }
		--libdirs{ "dependencies/assimp/lib" }
		files { src_dir.."**.h", src_dir.."**.cpp" }
	project "test_cpp"
		src_dir = "sample/test_cpp/"
		kind "ConsoleApp"
		includedirs {"."}
		files { src_dir.."**.h", src_dir.."**.cpp" }
	project "tnet_server"
		src_dir = "sample/tnet_server/"
		dependson {"engine"}
		kind "ConsoleApp"
		includedirs {"."}
		files { src_dir.."**.h", src_dir.."**.cpp" }
	project "tnet_client"
		src_dir = "sample/tnet_client/"
		dependson {"engine"}
		kind "ConsoleApp"
		includedirs {"."}
		files { src_dir.."**.h", src_dir.."**.cpp" }
