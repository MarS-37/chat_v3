project(chat)
cmake_minimum_required(VERSION 3.0)

include_directories(/usr/include/mysql)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

add_executable(chat 
	${PROJECT_SOURCE_DIR}/chat_client.cpp 
	${PROJECT_SOURCE_DIR}/project_lib.cpp 
	${PROJECT_SOURCE_DIR}/config_file.cpp
	${PROJECT_SOURCE_DIR}/logger.cpp
	${PROJECT_SOURCE_DIR}/client.cpp)
set_property(TARGET chat PROPERTY CXX_STANDARD 20)
add_executable(chat_server 
	${PROJECT_SOURCE_DIR}/chat_server.cpp 
	${PROJECT_SOURCE_DIR}/private_message.cpp 
	${PROJECT_SOURCE_DIR}/broadcast_message.cpp 
	${PROJECT_SOURCE_DIR}/chat_user.cpp 
	${PROJECT_SOURCE_DIR}/config_file.cpp 
	${PROJECT_SOURCE_DIR}/SHA256.cpp 
	${PROJECT_SOURCE_DIR}/project_lib.cpp 
	${PROJECT_SOURCE_DIR}/mysql.cpp 
	${PROJECT_SOURCE_DIR}/logger.cpp
	${PROJECT_SOURCE_DIR}/server.cpp)
set_property(TARGET chat_server PROPERTY CXX_STANDARD 20)
target_link_libraries(chat_server mysqlclient)


