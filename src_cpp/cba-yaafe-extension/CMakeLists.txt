file(GLOB_RECURSE cba-yaafe-extension_SOURCES *.cpp)

set(cba-yaafe-extension_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src_cpp/ ${EIGEN_INCLUDE_DIR})
set(cba-yaafe-extension_LIBS )


include_directories(${cba-yaafe-extension_INCLUDE_DIRS})
#link_directories(${CMAKE_BINARY_DIR}/src_cpp/yaafe-core)
add_library (cba-yaafe-extension MODULE ${cba-yaafe-extension_SOURCES})
if (NOT APPLE)
   set_target_properties(cba-yaafe-extension PROPERTIES VERSION ${YAAFE_LIBVERSION} SOVERSION ${YAAFE_API_VERSION})
endif (NOT APPLE)
target_link_libraries(cba-yaafe-extension yaafe-core ${cba-yaafe-extension_LIBS})

install(TARGETS cba-yaafe-extension DESTINATION ${CMAKE_INSTALL_YAAFE_EXTENSIONS})

