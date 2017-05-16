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
		defines 		{ "CUTE_BUILD_CORE" }
		kind 			( lib_kind )
		
		-- add files
		files			{ "engine/**.*" }
		removefiles		{ "engine/src/conversion/**.*" }
		removefiles		{ "engine/src/pcre/**.*" }
		removefiles		{ "engine/backup/**.*" }
		module_vpaths	( { "engine/**.*" }, "Core"	)

-- 测试项目
group "samples"
	project("demo")
		src_dir			= "samples/demo/"
		
		kind			( "ConsoleApp" )
		
		files			{ src_dir .. "**.*" }
	
-- 服务器
group "server"	
	project("world")
		kind 			( "ConsoleApp" )
		dependson 		{ "engine" }
		files 			{ "server/world/**.*" }
		module_vpaths(  { "server/world/**.*"}, "src" )
		--vpaths ({["src"]="server/world/**.*"})

	project("game")
		kind 			( "ConsoleApp" )
		dependson 		{ "engine" }
		files 			{ "server/game/**.*" }
		module_vpaths( {"server/game/**.*"}, "src" )
		--vpaths ({["src"]="server/game/**.*"})
		
	-- 机器人测试工具
	project("robot")
		kind 		( "ConsoleApp" )
		dependson 	{ "engine" }
		files 		{ "server/robot/**.*" }
		vpaths 		{ ["src"] = "server/robot/**.*" }
		
	-- 一些测试代码
	project("test")
		kind 		( "ConsoleApp" )
		dependson 	{ "engine" }
		files 		{ "server/test/**.*" }
		vpaths 		{ ["src"]="server/test/**.*" }

-- 工具
group "tools"
	-- 表格自动生成并校验工具
	project("tabgen")
		kind 		( "ConsoleApp" )
		dependson 	{ "engine" }
		files 		{ "tools/tabgen/**.*" }

	-- execel转csv工具
	project("csvgen")
		language("C#")
		kind	("ConsoleApp")
		--libdirs { "tools/csvgen/EPPlus"}
		links 	{"Excel", "System", "System.Data", "System.XML"}
		files 	{"tools/csvgen/src/**.*"}
		vpaths	{ ["src"] = "tools/csvgen/src/**.*" }

-- 渲染API		
group "plugin_render"
	project("plugin_ogl")
		src_dir 	= "plugins/RenderOGL/"
		glew_dir 	= src_dir .."/glew/"
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_OGL"}
		defines		{ "GLEW_BUILD", "GLEW_NO_GLU" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }
		
		-- glew on windows
		if os.is("windows") then
			includedirs { "./depends/glew/include"}
			--includedirs { src_dir.."glew/include" }
			--files 		{ glew_dir .. "**.*" }
			--vpaths 		{ ["glew"] = glew_dir }
		end
				
	project("plugin_vulkan")
		src_dir 	= "plugins/RenderValkan/"
		sdk_dir 	= "D:/Program Files/Vulkan/1.0.11.0/"
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_VALKAN" }
		
		includedirs { sdk_dir.."Include" }
		libdirs 	{ sdk_dir.."Bin" }
		links 		{ "vulkan-1" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = {src_dir.. "**.*"} }
		
	project("plugin_d3d11")
		src_dir 	= "plugins/RenderD3D11/"
		sdk_dir 	= "D:/Program Files (x86)/Microsoft DirectX SDK (August 2009)/"
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_D3D11" }
		
		includedirs { sdk_dir .. "Include" 	}
		libdirs		{ sdk_dir .. "/Lib/x64"	}
		
		files 		{ src_dir .. "**.*" 	}
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }
		
	project("plugin_d3d12")
		src_dir = "plugins/RenderD3D12/"
		sdk_dir = "D:/Program Files (x86)/Windows Kits/10/"
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_D3D12" }
		
		-- dependson
		includedirs { sdk_dir .. "Include/10.0.10069.0/um/"}
		libdirs		{ sdk_dir .. "Lib/10.0.10069.0/um/x64" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = {src_dir.. "**.*"} }
		
-- 数据库
group "plugin_db"
	project("plugin_mysql")
		src_dir 	= "plugins/DBMySQL/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_MYSQL" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }
		
	project("plugin_postgre")
		-- DBPostgreSQL
		src_dir 	= "plugins/DBPostgreSQL/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_PTSQL" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }
		
	project("plugin_sqlite")
		src_dir 	= "plugins/DBSQLite/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_SQLITE" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }
		
-- 脚本
group "plugin_script"
	project ("plugin_lua")
		src_dir 	= "plugins/ScriptLua/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_LUA" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }
		
	project ("plugin_python")
		src_dir 	= "plugins/ScriptPython/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_PYTHON" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }
	
	project ("plugin_v8")
		src_dir 	= "plugins/ScriptV8/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_V8" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }
		
-- 脚本绑定
--group "plugin_binding"
	
-- 文件加载
group "plugin_importer"
	project ("plugin_assimp")
		src_dir 	= "plugins/ImporterAssimp/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_ASSIMP" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }

	project ("plugin_fbx")
		src_dir 	= "plugins/ImporterFBX/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_FBX" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }
		
	project ("plugin_font")
		src_dir 	= "plugins/ImporterFont/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_FONT" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }	

	project ("plugin_freeimage")
		src_dir 	= "plugins/ImporterFreeImage/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_FREEIMAGE" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }	

-- 音频
group "plugin_audio"		
	project ("plugin_fmod")
		src_dir 	= "plugins/AudioFMOD/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_FMOD" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }	
		
	project ("plugin_openal")
		src_dir 	= "plugins/AudioOpenAL/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_OPENAL" }
		
		files 		{ src_dir .. "**.*" }
		vpaths		{ ["src/coder"] = { src_dir .. "*coder.*"} }
		vpaths 		{ ["src"]       = { src_dir .. "**.*" } }	
		
	
-- 物理引擎
group "plugin_physics"
	project ("plugin_ode")
		src_dir 	= "plugins/PhysicsODE/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_ODE" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }			
		

		
	project ("plugin_physx")
		src_dir 	= "plugins/PhysicsPhysX/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_PHYSX" }
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }			

-- 输入输出
group "plugin_input"
	project ("plugin_ois")
		src_dir 	= "plugins/InputOIS/"
		sdk_dir		= ""
		
		dependson 	{ "engine" }
		kind		( "SharedLib" )
		defines 	{ "CUTE_BUILD_OIS" }
		includedirs	{ "./depends/OIS/include" }
		libdirs		{ "./depends/OIS/lib"}
		
		files 		{ src_dir .. "**.*" }
		vpaths 		{ ["src"] = { src_dir.. "**.*" } }
