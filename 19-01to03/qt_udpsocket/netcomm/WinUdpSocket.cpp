#include "WinUdpSocket.h"
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "cameradatareceiver.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 1500

WinUdpSocket::WinUdpSocket()
{
}


WinUdpSocket::~WinUdpSocket()
{
    if( inited ) {
        // cleanup
        closesocket(clientSocket);
    }
    WSACleanup();
}

int WinUdpSocket::init()
{
    recvedData.resize(DEFAULT_BUFLEN);
    WSADATA wsaData;
    struct addrinfo hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == clientSocket) {
        printf("Failed Create Socket:%0X\n", WSAGetLastError());
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    inited = true;
    return 0;
}

int WinUdpSocket::getByteRecved() const
{
    return byteRecved;
}

void WinUdpSocket::clearByteRecved()
{
    byteRecved = 0;
}


int WinUdpSocket::getPacketCount() const
{
    return packetCount;
}

void WinUdpSocket::clearPacketCount()
{
    packetCount = 0;
}


bool WinUdpSocket::bind(const char *host, int port)
{
    if( !inited ) {
        init();
    }
    SOCKADDR_IN sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = inet_addr(host);
    //InetPton(AF_INET, (PCWSTR)("127.0.0.1"), &sin.sin_addr.s_addr);

    if (::bind(clientSocket, (SOCKADDR *)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !");
        return false;
    }
    return true;
}

void WinUdpSocket::startRead()
{
    qInfo()<< ("Windows Udp Socket Start Recving.");
    int iResult;
//    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    int addrLen = sizeof(fromaddr);
//    char *s = new char[20];
    do {
//        iResult = recvfrom(clientSocket, recvbuf, recvbuflen, 0, &fromaddr, &addrLen);
        // 这里将 recvdData 指向的内容指针传递进去，后续使用时，尽量不要再使用该指针
        iResult = recvfrom(clientSocket, recvedData.data(), recvbuflen, 0, &fromaddr, &addrLen);
        //addr_in = (struct sockaddr_in *)fromaddr
        //iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            packetCount += 1;
            byteRecved += iResult;
            cdr->rawDataRecved(recvedData);
//            cdr->recvCharData(recvbuf);
//            inet_ntop(AF_INET, &((sockaddr_in *)(&fromaddr))->sin_addr, s, 20);
        }
        else if (iResult == 0) {
            printf("Connection closed\n");
        }
        else {
            printf("Recv failed with error: %d\n", WSAGetLastError());
        }
    } while (iResult > 0 && m_working);
    qInfo()<< ("Windows Udp Socket Stop Recving.");
    closesocket(clientSocket);
    WSACleanup();
}

void WinUdpSocket::setReceiver(CameraDataReceiver *cdr)
{
    this->cdr = cdr;
}


void WinUdpSocket::setWorking(bool working)
{
    m_working = working;
}
