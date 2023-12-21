add_library(dylib INTERFACE)
target_include_directories(dylib INTERFACE "dylib/")


if (WIN32)
elseif(ANDROID)
  add_library(libprotobuf STATIC IMPORTED)
  set_target_properties(libprotobuf PROPERTIES IMPORTED_GLOBAL TRUE)
  add_library(libprotoc STATIC IMPORTED)
  set_target_properties(libprotoc PROPERTIES IMPORTED_GLOBAL TRUE)

  set_target_properties(libprotobuf PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/protobuff_android/${ANDROID_ABI}/libprotobuf.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/protobuff_android/include"
    IMPORTED_GLOBAL TRUE
  )

  set(CUSTOM_ANDROID_EXTRA_LIBS
      ${CUSTOM_ANDROID_EXTRA_LIBS}
      "${CMAKE_CURRENT_LIST_DIR}/protobuff_android/${ANDROID_ABI}/libprotobuf.a"
  CACHE INTERNAL "")

  set_target_properties(libprotoc PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/protobuff_android/${ANDROID_ABI}/libprotoc.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/protobuff_android/include"
    IMPORTED_GLOBAL TRUE
  )

  set(CUSTOM_ANDROID_EXTRA_LIBS
      ${CUSTOM_ANDROID_EXTRA_LIBS}
      "${CMAKE_CURRENT_LIST_DIR}/protobuff_android/${ANDROID_ABI}/libprotoc.a"
  CACHE INTERNAL "")
endif()

