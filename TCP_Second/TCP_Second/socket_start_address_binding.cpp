#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "ws2_32")
#define SERVERPORT 2000
#define BUFFER_LENGTH 512

int main(int argc, char* argv[])
{
    SOCKET listenSocket, clientSocket;
    WSADATA wsaData;
    int iResult, iSendResult;

    if (argc != 2) {
        std::cout << "서버 주소 입력 필요" << std::endl;
        return 1;
    }

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("윈속 초기화 실패");
        return 1;
    }
    //서버 연결용 소켓 생성
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cout << "socket(): 소켓 생성 실패" << std::endl;
        WSACleanup();
        return 1;
    }

    //서버로 연결 요청

    SOCKADDR_IN serverAddr, clientAddr;
    ZeroMemory(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVERPORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    iResult = bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR)
    {
        printf("소켓 바인딩 실패");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("소켓 대기모드 설정 실패");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    int addrLength = sizeof(clientAddr);
    std::cout << "client 연";
    clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLength);
    if (clientSocket == INVALID_SOCKET)
    {
        printf("클라이언트 연결 실패");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    closesocket(listenSocket);

    char buffer[BUFFER_LENGTH];

    do
    {
        ZeroMemory(buffer, sizeof(buffer));
        //데이터 수신
        iResult = recv(clientSocket, buffer, BUFFER_LENGTH, 0);
        if (iResult > 0)
        {
            printf("%s\n", buffer);
            iSendResult = send(clientSocket, buffer, iResult, 0);
            if (iSendResult == SOCKET_ERROR)
            {
                printf("send() : 데이터 전송 실패");
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
        }
        else if (iResult == 0)
            printf("연결 해제");
        else
        {
            printf("데이터 수신 실패");
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    } while (iResult > 0);
    closesocket(clientSocket);
    //윈속 해제
    WSACleanup();
    return 0;

}