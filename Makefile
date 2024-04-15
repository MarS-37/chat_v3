SRC_DIR = src
BINDIR = bin
C_SRC = \
	$(SRC_DIR)/chat_client.cpp \
	$(SRC_DIR)/project_lib.cpp \
	$(SRC_DIR)/config_file.cpp \
	$(SRC_DIR)/logger.cpp \
	$(SRC_DIR)/client.cpp
S_SRC = \
	$(SRC_DIR)/private_message.cpp \
	$(SRC_DIR)/broadcast_message.cpp \
	$(SRC_DIR)/chat_user.cpp \
	$(SRC_DIR)/config_file.cpp \
	$(SRC_DIR)/chat_server.cpp \
	$(SRC_DIR)/SHA256.cpp \
	$(SRC_DIR)/project_lib.cpp \
	$(SRC_DIR)/mysql.cpp \
	$(SRC_DIR)/logger.cpp \
	$(SRC_DIR)/server.cpp

C_TARGET = $(BINDIR)/chat
S_TARGET = $(BINDIR)/chat_server
PREFIX = /usr/local/bin
CONFIG_DIR = /etc
CLIENT_CONFIG_FILE = client.cfg
SERVER_CONFIG_FILE = server.cfg
INCLUDES = /usr/include/mysql
LIB = -lmysqlclient
STD = c++20

chat: $(C_SRC) $(S_SRC) create_bindir build_client build_server

create_bindir:
	mkdir -p $(BINDIR)

build_client:
	g++ --std=$(STD) -o $(C_TARGET) $(C_SRC) 

build_server:
	g++ --std=$(STD) -o $(S_TARGET) $(S_SRC) -I $(INCLUDES) $(LIB)

clean:
	rm -rf *.o $(C_TARGET) $(S_TARGET)

install:
	install $(C_TARGET) $(PREFIX)
	install $(CLIENT_CONFIG_FILE) $(CONFIG_DIR)
	install $(S_TARGET) $(PREFIX)
	install $(SERVER_CONFIG_FILE) $(CONFIG_DIR)

uninstall:
	rm -rf $(PREFIX)/$(C_TARGET)
	rm -rf $(CONFIG_DIR)/$(CLIENT_CONFIG_FILE)
	rm -rf $(PREFIX)/$(S_TARGET)
	rm -rf $(CONFIG_DIR)/$(SERVER_CONFIG_FILE)
