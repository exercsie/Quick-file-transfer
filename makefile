CLIENT_SRC = \
src/Client/Main/MenuClient.cpp \
src/Client/Main/MainClient.cpp \
src/Client/Start-Up/StartUpClient.cpp \
src/Shared/File-send-and-receive/ReceiveFile.cpp \
src/Shared/File-send-and-receive/SendFile.cpp \
src/Shared/Helpers/Helper.cpp

SERVER_SRC = \
src/Server/Main/MainServer.cpp \
src/Server/Main/MenuServer.cpp \
src/Server/Start-Up/StartUpServer.cpp \
src/Shared/File-send-and-receive/SendFile.cpp \
src/Shared/File-send-and-receive/ReceiveFile.cpp \
src/Shared/Helpers/Helper.cpp

client Client:
	g++ $(CLIENT_SRC) -o Client -std=c++23


server Server:
	g++ $(SERVER_SRC) -o Server -std=c++23

clean:
	$(RM) victim server
