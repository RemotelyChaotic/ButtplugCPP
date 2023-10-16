project(ButtplugCppClient)

if(MSVC)
  # Static runtime
  set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../lib/protobuf/cmake/")
include(protobuf-generate)

include(ExternalProject)
ExternalProject_Get_Property(protobuf install_dir)

add_library(bp-proto OBJECT "${CMAKE_CURRENT_LIST_DIR}/../lib/buttplug-rs-ffi/protobuf_schemas/buttplug_rs_ffi.proto")
add_dependencies(bp-proto protobuf-install)
set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(bp-proto PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")
target_include_directories(bp-proto PRIVATE ${install_dir}/include)
target_link_libraries(bp-proto PUBLIC libprotobuf)

add_library(${PROJECT_NAME} STATIC
    "ButtplugFFI.cpp"
    "ButtplugClient.cpp"
    "ButtplugProto.cpp"
    "ButtplugDevice.cpp"
)
add_dependencies(${PROJECT_NAME} protobuf-install)

protobuf_generate(
    LANGUAGE cpp
    PROTOC_EXE "${CMAKE_BINARY_DIR}/protobuf/protoc${CMAKE_EXECUTABLE_SUFFIX}"
    TARGET bp-proto
    IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/../lib/buttplug-rs-ffi/protobuf_schemas/"
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE dylib)
target_link_libraries(${PROJECT_NAME} PRIVATE bp-proto)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/..)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${install_dir}/include)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /WX)
  #disable loss of precision warning because google doesn't care about such mundane things
  target_compile_options(${PROJECT_NAME} PRIVATE /wd4267)
endif()

if(UNIX AND NOT APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE dl)
endif()
