cmake_minimum_required(VERSION 3.16)
#project(ButtplugCppClient)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Android doesn't work reliably yet, so it's disabled for now
# we need to wait for buttplug.io to update and get Android out of
# beta
if (ANDROID) #AND ANDROID_ABI STREQUAL "x86")
  set(HAS_BUTTPLUG_CPP FALSE CACHE INTERNAL "")
else()
  add_subdirectory("lib/")
  add_subdirectory("src/")
  if (BUILD_EXAMPLES)
    add_subdirectory("example/")
  endif()
  set(HAS_BUTTPLUG_CPP TRUE CACHE INTERNAL "")
endif()
