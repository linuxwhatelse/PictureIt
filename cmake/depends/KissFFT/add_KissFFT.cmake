set(KISSFFT_SRC_DIR ${CMAKE_BINARY_DIR}/3rdParty/KissFFT-src)
set(KISSFFT_BUILD_DIR ${CMAKE_BINARY_DIR}/3rdParty/KissFFT-build)

# Download and unpack KissFFT at configure time
configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt.in
               ${KISSFFT_BUILD_DIR}/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                WORKING_DIRECTORY ${KISSFFT_BUILD_DIR})

execute_process(COMMAND ${CMAKE_COMMAND} --build .
                WORKING_DIRECTORY ${KISSFFT_BUILD_DIR})


add_subdirectory(${KISSFFT_SRC_DIR}
                 ${KISSFFT_BUILD_DIR})


mark_as_advanced(FORCE ${KISSFFT_SRC_DIR} ${KISSFFT_BUILD_DIR})
