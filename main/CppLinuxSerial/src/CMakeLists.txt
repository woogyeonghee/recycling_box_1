

file(GLOB_RECURSE CppLinuxSerial_SRC
        "*.cpp")

file(GLOB_RECURSE CppLinuxSerial_HEADERS
        "${CMAKE_SOURCE_DIR}/include/*.hpp")

add_library(CppLinuxSerial ${CppLinuxSerial_SRC} ${CppLinuxSerial_HEADERS})

target_include_directories(CppLinuxSerial PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
                                                "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>")                                                  

# On Linux, "sudo make install" will typically copy the library
# into the folder /usr/local/bin
install(TARGETS CppLinuxSerial DESTINATION lib)

# On Linux, "sudo make install" will typically copy the
# folder into /usr/local/include
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/CppLinuxSerial DESTINATION include)
