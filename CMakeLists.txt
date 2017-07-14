cmake_minimum_required(VERSION 2.8.0)

project(cute)

#engine
file(GLOB_RECURSE engine_src ./engine/*.cpp ./engine/*.h)
add_library(engine ${engine_src})

#plugins
###Render
file(GLOB_RECURSE valkan_src ./plugins/RenderVulkan/*.* )
add_library(plugin_vulkan ${valkan_src})

#samples

#servers