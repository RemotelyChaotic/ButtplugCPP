add_library(dylib INTERFACE)
target_include_directories(dylib INTERFACE "dylib/")


#build protobuff externally
include(ExternalProject)

set(libPTB protobuf)
ExternalProject_Add(${libPTB}
  PREFIX ${CMAKE_BINARY_DIR}/${libPTB}
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/${libPTB}
  BINARY_DIR ${CMAKE_BINARY_DIR}/${libPTB}
  INSTALL_DIR ${CMAKE_BINARY_DIR}/${libPTB}
  CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/${libPTB}"
    "-DCMAKE_INSTALL_INCLUDEDIR:PATH=${CMAKE_BINARY_DIR}/${libPTB}/include"
    "-Dprotobuf_BUILD_TESTS=OFF"
    "-Dprotobuf_BUILD_EXAMPLES:BOOL=OFF"
    "-Dprotobuf_INSTALL=ON"
    "-Dprotobuf_WITH_ZLIB=Off"
  BUILD_BYPRODUCTS
    ${CMAKE_BINARY_DIR}/${libPTB}/protoc${CMAKE_EXECUTABLE_SUFFIX}
    ${CMAKE_BINARY_DIR}/${libPTB}/libprotobufd${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${CMAKE_BINARY_DIR}/${libPTB}/libprotobuf${CMAKE_STATIC_LIBRARY_SUFFIX}
  INSTALL_BYPRODUCTS
    ${CMAKE_BINARY_DIR}/${libPTB}/include
  STEP_TARGETS   install
)

# Acquire Installation Directory of
ExternalProject_Get_Property(${libPTB} install_dir)
include_directories(${install_dir}/include)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/${libPTB})

add_library(libprotobuf STATIC IMPORTED)
set_target_properties(libprotobuf PROPERTIES IMPORTED_GLOBAL TRUE)

if (${KDE_DEBUG})
  set_target_properties (libprotobuf PROPERTIES IMPORTED_LOCATION
    ${install_dir}/libprotobufd${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
  set_target_properties (libprotobuf PROPERTIES IMPORTED_LOCATION
    ${install_dir}/libprotobuf${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()
