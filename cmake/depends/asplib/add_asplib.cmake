set(ASPLIB_SRC_DIR ${CMAKE_BINARY_DIR}/3rdParty/asplib-src)
set(ASPLIB_BUILD_DIR ${CMAKE_BINARY_DIR}/3rdParty/asplib-build)

# Download and unpack asplib at configure time
configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt.in
               ${ASPLIB_BUILD_DIR}/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                WORKING_DIRECTORY ${ASPLIB_BUILD_DIR})

execute_process(COMMAND ${CMAKE_COMMAND} --build .
                WORKING_DIRECTORY ${ASPLIB_BUILD_DIR})

add_subdirectory(${ASPLIB_SRC_DIR}
                 ${ASPLIB_BUILD_DIR})


mark_as_advanced(FORCE ${ASPLIB_SRC_DIR} ${ASPLIB_BUILD_DIR})
