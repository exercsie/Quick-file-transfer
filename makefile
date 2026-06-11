CLIENT_SRC = \
src/Client/Main/MenuClient.cpp \
src/Client/Main/MainClient.cpp \
src/Client/Start-Up/StartUpClient.cpp \
src/Shared/File-send-and-receive/ReceiveFile.cpp \
src/Shared/File-send-and-receive/SendFile.cpp

SERVER_SRC = \
src/Server/Main/MainServer.cpp \
src/Server/Main/MenuServer.cpp \
src/Server/Start-Up/StartUpServer.cpp \
src/Shared/File-send-and-receive/SendFile.cpp \
src/Shared/File-send-and-receive/ReceiveFile.cpp

client Client:
	g++ $(CLIENT_SRC) -o Client -O3


server Server:
	g++ $(SERVER_SRC) -o Server -O3

clean:
	$(RM) victim server
