cmake_minimum_required(VERSION 2.8.12)
project(hose)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)
if(UNIX)
	#set(MAGNUM_BINARY_INSTALL_DIR "${MAGNUM_BINARY_INSTALL_DIR}/${CMAKE_BUILD_TYPE}/")
#message(${MAGNUM_BINARY_INSTALL_DIR})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/Debug")

endif(UNIX)

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

#find_package(Sol2 REQUIRED)
link_directories("p:/sys/lib")
find_package(Lua REQUIRED)

find_package(Magnum REQUIRED 
	GlfwApplication
	DebugTools
	Primitives
	SceneGraph
	Shapes
    Shaders
	MeshTools
)

find_package(Corrade REQUIRED Utility TestSuite)
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configure.h.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/configure.h)

corrade_add_resource(RESOURCES data/resources.conf)

enable_testing()
corrade_add_test(MyTest main.cpp example/StringCast.cpp
	LIBRARIES 
		Corrade::Utility
		${LUA_LIBRARIES}
	)

target_include_directories(MyTest PRIVATE
	"P:/sys/include"
	"/usr/include"
	"/usr/local/include"
	"../../external/json/src"
	"../build/include"
    "../include/metas"
	${LUA_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR})

	
set(EX_NAME MyApplication)
add_executable(${EX_NAME}
	${RESOURCES}
    example/StringCast.cpp
    MyApplication.cpp
	MyApplication.h
	Types.hpp
)


target_link_libraries(${EX_NAME}
    Magnum::Application
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::SceneGraph
    Magnum::Shaders
    Magnum::Shapes
	Magnum::DebugTools
    Corrade::Utility
	${LUA_LIBRARIES}
    )

target_include_directories(${EX_NAME} PRIVATE
	"P:/sys/include"
	"/usr/include"
	"/usr/local/include"
	"../../external/json/src"
	"../build/include"
    "../include/metas"
	${LUA_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR})

#"../../external/MetaStuff/include"
#install(TARGETS ${EX_NAME} DESTINATION ${MAGNUM_BINARY_INSTALL_DIR})
#install(FILES README.md DESTINATION ${MAGNUM_DATA_INSTALL_DIR}/examples RENAME README-shadows.md)

add_custom_target(
	COMMAND ${EX_NAME}
	COMMAND ${EX_NAME}
    COMMAND "${MAGNUM_BINARY_INSTALL_DIR}/Debug")
