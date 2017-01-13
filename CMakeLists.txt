cmake_minimum_required(VERSION 2.8)

project(PcapToSqlite)

add_subdirectory(LibPcap)
if (NOT TARGET gtest) # googletest library may have been added by other subdirectory
    add_subdirectory(googletest)
endif() 

set(CPPOBSERVER ${CMAKE_CURRENT_SOURCE_DIR}/cppobserver)  
set(CPP_LIBPCAP ${CMAKE_CURRENT_SOURCE_DIR}/LibPcap)  
set(CXXOPTS ${CMAKE_CURRENT_SOURCE_DIR}/cxxopts)  
set(CPP_SQLITE ${CMAKE_CURRENT_SOURCE_DIR}/sqlite_modern_cpp)  

set(GOOGLETEST ${CMAKE_CURRENT_SOURCE_DIR}/googletest/)  
include_directories(${GOOGLETEST}/googlemock/include ${GOOGLETEST}/googletest/include)
link_directories(${GOOGLETEST}/build/googlemock ${GOOGLETEST}/build/googlemock/gtest)
include_directories(${CPPOBSERVER})
include_directories(${CPP_LIBPCAP})
include_directories(${CXXOPTS}/include)
include_directories(${CPP_SQLITE}/hdr)
if(NOT CMAKE_BUILD_TYPE)
    message( SEND_ERROR "please set CMAKE_BUILD_TYPE to Debug or Release" )
endif(NOT CMAKE_BUILD_TYPE)

# set build type: cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-4.9   ; to see all options of AddressSanitizer, set the environment variable ASAN_OPTIONS=help=1
#for DEBUG: -p will generate gmon.out which can be read by gprof; to visualize gprof output use tool such as gprof2dot (https://github.com/jrfonseca/gprof2dot)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -p -g -std=c++1y -Wall -Wextra -Wno-unused-parameter -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -g -std=c++1y -Wall -Wextra -Wno-unused-parameter")

set(SRC packetdb.cpp packetdb_sqlite.cpp)

add_executable(runtests tests.cpp tests_packetdb.cpp ${SRC})
set(SAMPLE_PCAP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/)
set_target_properties(runtests PROPERTIES COMPILE_FLAGS -D'SAMPLE_PCAP_DIR="${SAMPLE_PCAP_DIR}"')
target_link_libraries(runtests gmock)
target_link_libraries(runtests gtest)
target_link_libraries(runtests pthread)
target_link_libraries(runtests cpppcap)
target_link_libraries(runtests pcap)
target_link_libraries(runtests sqlite3)


add_executable(pcap2sqlite pcap2sqlite.cpp ${SRC})
add_dependencies(pcap2sqlite runtests) # test binaries should be ok before building the actual app
target_link_libraries(pcap2sqlite cpppcap pcap sqlite3)
