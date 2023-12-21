project(ButtplugCppClient)

if (NOT ANDROID)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/3rdparty/protobuf/cmake/")
  include(protobuf-generate)

  find_package(absl CONFIG REQUIRED
               HINTS ${CMAKE_BINARY_DIR}/protobuf/install/lib/cmake/absl)
  find_package(utf8_range CONFIG REQUIRED
               HINTS ${CMAKE_BINARY_DIR}/protobuf/install/lib/cmake/utf8_range)
  find_package(protobuf CONFIG REQUIRED
               HINTS ${CMAKE_BINARY_DIR}/protobuf/install/cmake)

  message(STATUS "Using Protocol Buffers ${protobuf_VERSION}")
endif()

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

add_library(${PROJECT_NAME} STATIC
    "${CMAKE_CURRENT_LIST_DIR}/../lib/buttplug-rs-ffi/protobuf_schemas/buttplug_rs_ffi.proto"
    "ButtplugFFI.cpp"
    "ButtplugClient.cpp"
    "ButtplugProto.cpp"
    "ButtplugDevice.cpp"
)
if (NOT ANDROID)
  target_link_libraries(${PROJECT_NAME} PRIVATE protobuf::libprotobuf)
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE libprotobuf)
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
      TARGET ${PROJECT_NAME}
      IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/../lib/buttplug-rs-ffi/protobuf_schemas/"
      PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE dylib)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)
target_include_directories(${PROJECT_NAME} PRIVATE
  "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>"
  ${CMAKE_BINARY_DIR}/protobuf/install/include
  ${CMAKE_CURRENT_BINARY_DIR}
)
if (NOT ANDROID)
  target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_BINARY_DIR}/protobuf/install/include
  )
else()
  target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdparty/protobuff_android/include
  )
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /WX)
  #disable some warnings because google doesn't care about such mundane things
  target_compile_options(${PROJECT_NAME} PRIVATE /wd4141)
  target_compile_options(${PROJECT_NAME} PRIVATE /wd4267)
  target_compile_options(${PROJECT_NAME} PRIVATE /wd4244)
  target_compile_options(${PROJECT_NAME} PRIVATE /wd4715)
endif()

if(UNIX AND NOT APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE dl)
endif()
