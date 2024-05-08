#pragma comment(lib, "ws2_32") // ���� ���̺귯�� ��ũ
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    // ���� �ʱ�ȭ
    WSADATA wsa;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa); // ���� ���̺귯�� �ʱ�ȭ
    if (result != 0) {
        cout << "���� �ʱ�ȭ ����" << "\n";
        return 1;
    }

    cout << "���� �ʱ�ȭ ����" << "\n";
    SOCKET dataSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (dataSocket == INVALID_SOCKET) {
        cout << "���� ���� ����" << "\n";
        WSACleanup();
        return 1;
    }
    cout << "���� ���� ����" << "\n";
    cout << "������ �ݽ��ϴ�." << "\n";
    closesocket(dataSocket);
    cout << "������ �����մϴ�." << "\n";

    // ���� ����
    WSACleanup();
    return 0;
}