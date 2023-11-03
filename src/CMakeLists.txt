project(ButtplugCppClient)

if(MSVC)
  # Static runtime
  set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif()

if (NOT ANDROID)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../lib/protobuf/cmake/")
  include(protobuf-generate)

  include(ExternalProject)
  ExternalProject_Get_Property(protobuf install_dir)
endif()

add_library(bp-proto OBJECT "${CMAKE_CURRENT_LIST_DIR}/../lib/buttplug-rs-ffi/protobuf_schemas/buttplug_rs_ffi.proto")
if (NOT ANDROID)
  add_dependencies(bp-proto protobuf-install)
endif()
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
if (NOT ANDROID)
  add_dependencies(${PROJECT_NAME} protobuf-install)
endif()

if (ANDROID)
  file(COPY "${CMAKE_CURRENT_LIST_DIR}/../lib/buttplug-rs-ffi_android/buttplug_rs_ffi.pb.cc"
       DESTINATION ${PROTO_BINARY_DIR})
  file(COPY "${CMAKE_CURRENT_LIST_DIR}/../lib/buttplug-rs-ffi_android/buttplug_rs_ffi.pb.h"
       DESTINATION ${PROTO_BINARY_DIR})
else()
  set(PROTOC_EXE "${CMAKE_BINARY_DIR}/protobuf/protoc${CMAKE_EXECUTABLE_SUFFIX}")
  protobuf_generate(
      LANGUAGE cpp
      PROTOC_EXE ${PROTOC_EXE}
      TARGET bp-proto
      IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/../lib/buttplug-rs-ffi/protobuf_schemas/"
      PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
)
endif()

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
