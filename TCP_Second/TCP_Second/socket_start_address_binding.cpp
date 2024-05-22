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
        std::cout << "���� �ּ� �Է� �ʿ�" << std::endl;
        return 1;
    }

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("���� �ʱ�ȭ ����");
        return 1;
    }
    //���� ����� ���� ����
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cout << "socket(): ���� ���� ����" << std::endl;
        WSACleanup();
        return 1;
    }

    //������ ���� ��û

    SOCKADDR_IN serverAddr, clientAddr;
    ZeroMemory(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVERPORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    iResult = bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR)
    {
        printf("���� ���ε� ����");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("���� ����� ���� ����");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    int addrLength = sizeof(clientAddr);
    std::cout << "client ��";
    clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLength);
    if (clientSocket == INVALID_SOCKET)
    {
        printf("Ŭ���̾�Ʈ ���� ����");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    closesocket(listenSocket);

    char buffer[BUFFER_LENGTH];

    do
    {
        ZeroMemory(buffer, sizeof(buffer));
        //������ ����
        iResult = recv(clientSocket, buffer, BUFFER_LENGTH, 0);
        if (iResult > 0)
        {
            printf("%s\n", buffer);
            iSendResult = send(clientSocket, buffer, iResult, 0);
            if (iSendResult == SOCKET_ERROR)
            {
                printf("send() : ������ ���� ����");
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
        }
        else if (iResult == 0)
            printf("���� ����");
        else
        {
            printf("������ ���� ����");
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    } while (iResult > 0);
    closesocket(clientSocket);
    //���� ����
    WSACleanup();
    return 0;

}