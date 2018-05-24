PROJECT(LightViewer)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH} )

set (CMAKE_CXX_STANDARD 11)


# find OpenCL
find_package(OpenCL REQUIRED)
include_directories(SYSTEM ${OpenCL_INCLUDE_DIRS})

# find OpenGL
find_package(OpenGL REQUIRED)
include_directories(SYSTEM ${OpenGL_INCLUDE_DIRS})

find_package(QGLVIEWER REQUIRED)
include_directories(${QGLVIEWER_INCLUDE_DIR})

 find_package(Qt5 COMPONENTS Widgets OpenGL Xml REQUIRED)
#find_package(Qt5Widgets REQUIRED)
#find_package(Qt5OpenGL REQUIRED)
include_directories(${Qt5Widgets_INCLUDES_DIRS}
${Qt5OpenGL_INCLUDES_DIR}
${Qt5Xml_INCLUDES_DIR})

# required by both versions
set(CMAKE_AUTOMOC TRUE)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# find Boost
find_package(Boost 1.48 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})


set(EXAMPLES
  mainLight
)


foreach(EXAMPLE ${EXAMPLES})
add_executable(${EXAMPLE} ${EXAMPLE}.cpp lightviewer)
target_link_libraries(${EXAMPLE} ${OpenCL_LIBRARIES} ${QGLVIEWER_LIBRARIES} ${OpenGL_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES} ${Qt5Xml_LIBRARIES})
endforeach()
