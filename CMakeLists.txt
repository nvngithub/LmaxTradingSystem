cmake_minimum_required(VERSION 2.6)
 
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(Boost COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set (CMAKE_CXX_STANDARD 17)
set (OPENSSL_ROOT_DIR /usr/bin/openssl)
find_package(OpenSSL REQUIRED)

#set (SOURCE_FILES main.cpp)
#add_executable(runTests ${SOURCE_FILES})

add_executable(runTests 
			src/globals/globals.cpp 
			src/sock_comm/sock_comm.cpp 
			src/logger/logger.cpp 
			src/client_handler/client_handler.cpp
			src/models/request/request_base.cpp
			src/models/request/market_data_request/market_data_request.cpp
			src/models/request/authenticate/autheticate_request.cpp
			src/models/request/heart_beat_request/heart_beat_request.cpp
			src/models/request/single_order_request/single_order_request.cpp
			src/models/response/heart_beat_response/heart_beat_response.cpp
			src/models/response/login_response/login_response.cpp
			src/models/response/market_data_response/market_data_response.cpp
			src/models/response/service_test_request/service_test_request.cpp
			src/fix/fix_converter.cpp
			src/service/service.cpp
			src/scalpers/simple_scalper/simple_scalper.cpp
			src/main.cpp)

target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runTests ${Boost_FILESYSTEM_LIBRARY})
target_link_libraries(runTests OpenSSL::SSL)
