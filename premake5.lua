-- 用法：premake4.exe gmake(vs2010,vs2012,...)
--[[
启动程序并加载so，设置查找路径，另外一种方法，添加-rpath编译时指定查找目录
export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH
]]
local bin_dir = "build/bin/%{cfg.buildcfg}"
local lib_dir = "build/bin/%{cfg.buildcfg}"
local obj_dir = "build/obj/%{cfg.buildcfg}/%{prj.name}"
local lib_kind = iif(_ACTION =="gmake","StaticLib", "SharedLib")

local src_dir = nil

-- 查找并自动添加路径
function read_module(file_path)
	local file_module = nil
	
	print(file_path)
	local file = io.open(file_path, "r")
	local line = file:read("*line")
	
	if line ~= nil then
		-- 支持两种方式//!或者//module
		if string.startswith(line, "//!") then
			file_module = string.sub(line, 4);
		else
			local pos = nil
			pos = string.find(line, "module")
			if pos ~= nil then
				file_module = string.sub(line, pos + 6)
			end
		end
	end
	
	file:close()
	
	-- trim
	if file_module ~= nil then
		file_module = string.gsub(file_module, "^%s*(.-)%s*$", "%1")
	end
	return file_module
end

function module_vpaths(dirs, base_module)	
	-- 遍历所有目录文件
	local src_files = {}
	for _,pattern in pairs(dirs) do
		local tmp = os.matchfiles(pattern)
		for _, file in pairs(tmp) do
			--print(file)
			table.insert(src_files, file);
		end
	end
	
	-- 读取module,相同名字的.h和.cpp需要在同一个module下
	local file_modules = {}
	for _, file in pairs(src_files) do
		local basename = path.getbasename(file);
		local fmodule = read_module(file);
		--print("set module",file, fmodule)
		if fmodule ~= nil then
			file_modules[basename] = fmodule;
		end
	end
	
	-- 生成vpaths
	local file_vpaths = {}
	
	for _, file in pairs(src_files) do
		local basename = path.getbasename(file);
		local fmodule = file_modules[basename];
		if fmodule == nil then
			fmodule = base_module
		end
		
		--
		local vpath = fmodule;
		-- if string.endswith(file, ".h") then
		-- 	vpath = fmodule .. "/include";
		-- else
		-- 	vpath = fmodule .. "/src";
		-- end
		
		if file_vpaths[vpath] == nil then
			file_vpaths[vpath] = {}
		end
		
		table.insert(file_vpaths[vpath], file)
	end
	
	-- 写入摸
	
	vpaths(file_vpaths);
	
end

workspace "Cute"
	location("solution")
	--toolset "v140"
	--basedir("solution")
	startproject "game"
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
group "core"
project("engine")
	defines {"CUTE_BUILD_DLL"}
	kind (lib_kind)
	-- add files
	files({"engine/**.*"})
	removefiles({"engine/src/conversion/**.*"})
	removefiles({"engine/src/pcre/**.*"})
	removefiles({"engine/backup/**.*"})
	module_vpaths({"engine/**.*"}, "Core")
	
-- 服务器
group "server"	
	project("world")
		kind ("ConsoleApp")
		dependson {"engine"}
		files {"server/world/**.*"}
		module_vpaths({"server/world/**.*"}, "src")
		--vpaths ({["src"]="server/world/**.*"})

	project("game")
		kind ("ConsoleApp")
		dependson {"engine"}
		files {"server/game/**.*"}
		module_vpaths({"server/game/**.*"}, "src")
		--vpaths ({["src"]="server/game/**.*"})
		
	-- 机器人测试工具
	project("robot")
		kind ("ConsoleApp")
		dependson {"engine"}
		files {"server/robot/**.*"}
		vpaths ({["src"]="server/robot/**.*"})

group "tools"
	-- 表格自动生成并校验工具
	project("tabgen")
		kind ("ConsoleApp")
		dependson {"engine"}
		files {"tools/tabgen/**.*"}

	-- execel转csv工具
	project("csvgen")
		language("C#")
		kind("ConsoleApp")
		--libdirs { "tools/csvgen/EPPlus"}
		links {"Excel", "System", "System.Data", "System.XML"}
		files {"tools/csvgen/src/**.*"}
		vpaths({["src"] = "tools/csvgen/src/**.*"});
	
--[[
group "sql"
	project("plugin_mysql")
		
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
		sdk_dir = "D:/Program Files/Vulkan/1.0.11.0/"
		dependson { "engine" }
		kind("SharedLib")
		defines { "CU_VULKAN_BUILD" }
		--includedirs { src_dir}
		includedirs { sdk_dir.."Include" }
		libdirs { sdk_dir.."Bin" }
		links { "vulkan-1" }
		files { src_dir .. "**.*" }
		vpaths { ["src"] = {src_dir.. "**.*"} }
		
	project("plugin_d3d11")
		src_dir = "plugins/render_d3d11/"
		dependson { "engine" }
		kind("SharedLib")
		defines { "CU_D3D11_BUILD" }
		--includedirs { src_dir}
		--includedirs { "D:/Program Files (x86)/Microsoft DirectX SDK (August 2009)/Include" }
		libdirs	{"D:\Program Files (x86)\Microsoft DirectX SDK (August 2009)\Lib\x64"}
		files { src_dir .. "**.*" }
		vpaths { ["src"] = {src_dir.. "**.*"} }
		
	project("plugin_d3d12")
		src_dir = "plugins/render_d3d12/"
		dependson { "engine" }
		kind("SharedLib")
		defines { "CU_D3D12_BUILD" }
		--includedirs { src_dir }
		includedirs { "D:/Program Files (x86)/Windows Kits/10/Include/10.0.10069.0/um"}
		libdirs { "D:/Program Files (x86)/Windows Kits/10/Lib/10.0.10069.0/um/x64"}
		files { src_dir .. "**.*" }
		vpaths { ["src"] = {src_dir.. "**.*"} }

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
]]