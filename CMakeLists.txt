cmake_minimum_required(VERSION 2.6)

project(foilanalyzer)

set(CMAKE_CXX_STANDARD 11)
set(GCC_MINIMUM_VERSION "4.8")
set(CLANG_MINIMUM_VERSION "3.4")

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

if(UNIX)
   INCLUDE_DIRECTORIES(
      ${PROJECT_SOURCE_DIR}/build/include
   )

elseif(WIN32)
   INCLUDE_DIRECTORIES(
      ${PROJECT_SOURCE_DIR}/build/include/win64
   )
endif()

FILE(GLOB SRCS src/*.cpp)
FILE(GLOB HDRS src/*.h)

ADD_EXECUTABLE(flighty
   ${SRCS}
   ${HDRS}
)

if(UNIX)
   TARGET_LINK_LIBRARIES(flighty
      ${wxWidgets_LIBRARIES}
   )
elseif(WIN32)
   TARGET_LINK_LIBRARIES(flighty
      ${wxWidgets_LIBRARIES}
   )
endif()
