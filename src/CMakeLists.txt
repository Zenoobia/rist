cmake_minimum_required(VERSION 2.8.12)
project(hose)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)
# CMake policies: don't treat imported targets with :: as files
if(POLICY CMP0028)
    cmake_policy(SET CMP0028 NEW)
endif()
# Enable MACOSX_RPATH by default
if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif()
# Quoted variables should not be dereferenced
if(POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()



find_package(Magnum REQUIRED 
	GlfwApplication
	DebugTools
	Primitives
	SceneGraph
	Shapes
    Shaders
)

find_package(Corrade REQUIRED Utility)
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configure.h.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/configure.h)

#corrade_add_resource(_RESOURCES resources.conf)

set(EX_NAME MyApplication)
add_executable(${EX_NAME} MyApplication.cpp)

target_link_libraries(${EX_NAME}
    Magnum::Application
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::SceneGraph
    Magnum::Shaders
    Magnum::Shapes
    Corrade::Utility
    )

target_include_directories(${EX_NAME} PRIVATE
	"P:/sys/include"
	"/usr/include"
	"/usr/local/include"
	${CMAKE_CURRENT_BINARY_DIR})

#install(TARGETS ${EX_NAME} DESTINATION ${MAGNUM_BINARY_INSTALL_DIR})
#install(FILES README.md DESTINATION ${MAGNUM_DATA_INSTALL_DIR}/examples RENAME README-shadows.md)

add_custom_target(
	COMMAND ${EX_NAME}
	COMMAND ${EX_NAME}
    COMMAND ${MAGNUM_BINARY_INSTALL_DIR})
