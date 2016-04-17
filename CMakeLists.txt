cmake_minimum_required(VERSION 3.0)
project(PictureIt VERSION 0.1)

include(UseMultiArch.cmake)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${PROJECT_SOURCE_DIR})

# Enable pic
if(NOT WIN32)
    set(CMAKE_POSITION_INDEPENDENT_CODE 1)
endif()

# Find and use C++11
find_package(CXX11 REQUIRED)
# Find and include OpenGL
find_package(OpenGL REQUIRED)
# Find and include asplib
find_package(asplib REQUIRED SpectrumVisProcessor FFT)

# add definitions and include directories
add_definitions(${OpenGL_DEFINITIONS})
include_directories(${OpenGL_INCLUDE_DIRS})

set(SOURCES src/pictureit.cpp
            src/spectrum.cpp
            src/utils.cpp
            src/effects/effects.cpp
            src/effects/crossfade.cpp
            src/effects/slide.cpp)

set(HEADERS src/pictureit.h
            src/spectrum.h
            src/utils.h
            src/effects/effects.h
            src/effects/crossfade.h
            src/effects/slide.h)

# add library and configure it's include directories
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(PI::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE
                           PRIVATE src deps
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deps>
                           $<INSTALL_INTERFACE:include>
                           $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

if(WIN32)
  target_include_directories(${PROJECT_NAME} INTERFACE
                             $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deps/win32>)
endif()

# set target properties
target_link_libraries(${PROJECT_NAME} PRIVATE asplib::SpectrumVisProcessor)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)


# Install all header files
foreach (HEADER ${HEADERS})
    get_filename_component(HEADERDIR ${HEADER} DIRECTORY)
    string(REGEX REPLACE "src" "" HEADERDIR ${HEADERDIR})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}/${HEADERDIR})
endforeach()

if(NOT WIN32)
message(STATUS "CMAKE_INSTALL_LIBDIR_NOARCH=${CMAKE_INSTALL_LIBDIR_NOARCH}")
  # Pkgconfig
  include(PkgConfigHandler.cmake)
  configure_pc_file(${PROJECT_NAME} ${PROJECT_NAME}.pc.in
                                    ${PROJECT_NAME}.pc
                                    ${CMAKE_INSTALL_PREFIX}
                                    ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
                                    ${CMAKE_INSTALL_PREFIX}/include)

  install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
          DESTINATION ${CMAKE_INSTALL_LIBDIR_NOARCH}/pkgconfig)
endif()

# Generate and install a CMake config, version and target file
include(CMakePackageConfigHelpers)
set(CMAKE_INSTALL_CMAKECONFIGDIR
    ${CMAKE_INSTALL_LIBDIR_NOARCH}/cmake/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})

configure_package_config_file(${PROJECT_NAME}Config.cmake.in ${PROJECT_NAME}Config.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})
                              #PATH_VARS INCLUDE_INSTALL_DIR)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})

install(EXPORT ${PROJECT_NAME} FILE ${PROJECT_NAME}-targets.cmake NAMESPACE PI::
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR})

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

# Print project configuration
message(STATUS)
message(STATUS "${CMAKE_PROJECT_NAME} - Configuration:")
message(STATUS "CMAKE_BUILD_TYPE:     ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS)

