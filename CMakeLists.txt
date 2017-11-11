cmake_minimum_required(VERSION 3.2.3)
project(projet_neuron)

set(CMAKE_CXX_STANDARD 11)

enable_testing()
add_subdirectory(gtest)
set(SOURCE_FILES
	main.cpp
	Network.cpp
	Network.hpp
	Neuron.cpp
	Neuron.hpp)


include_directories(${gtest_SOURCE_DIR}/include${gtest_SOURCE_FILES})

add_executable(Neuron ${SOURCE_FILES})
add_executable(neuron_unittest neuron.cpp Network.cpp neuron_unittest.cpp)

target_link_libraries(neuron_unittest gtest gtest_main)
add_test(neuron_unittest neuron_unittest)

###### Doxygen ######

find_package(Doxygen)
if(DOXYGEN_FOUND)

    IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
        message(FATAL_ERROR "Doxygen configuration file is needed to build the documentation:
         - ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in not found.
         - Generate one in the project folder with doxygen -g Doxyfile.in")
    ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
ELSE(DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
ENDIF(DOXYGEN_FOUND)
