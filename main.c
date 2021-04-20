#include <stdio.h>
//#include <iostream>
#include <WinSock2.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <process.h>
//#include <time.h>
//#include <chrono>
//#include <iomanip>
#include <string.h>
//#include <sstream>

#define    BUFLEN    2000

struct sockaddr_in fsin;
SOCKET sockets[2048], ssock;
HANDLE hThread[2048] = { NULL };
unsigned int threadID;
int number;

struct request //data from the request
{
	IN_ADDR ip;
	int port;
    char *url;
    char *method;
	char *header;
};

char *message(char *header)
{
	sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: text/html;charset=utf-8\r\n\r\n<!DOCTYPE html>\n<html><head>\n<meta charset=\"gb2312\">\n<title>HelloWorld</title>\n<body>\n<h1>我的第一个标题</h1>\n<p>我的第一个段落。</p>\n</body>\n</html>\n");
	return header;
}

int deleteClient(int nn)
{
	int i, i2;
	for (i = 0; sockets[i] != '\0'; i++) {
		;
	}
	for (i2 = 0; hThread[i2] != NULL; i2++) {
		;
	}
	int a;

	sockets[nn] = sockets[nn + 1];
	hThread[nn] = hThread[nn + 1];
	for (a = 1; a <= i - nn; a++) {
		sockets[nn + a] = sockets[nn + a + 1];
	}
	for (a = 1; a <= i2 - a; a++) {
		hThread[nn + a] = hThread[nn + a + 1];
	}
	sockets[nn + a] = '\0';
	hThread[nn + a] = NULL;

	number--;

	return 0;
}

unsigned int __stdcall response(PVOID PM){
	char body[2048];
	char header[2048];
	int n = number;
    struct request requestx;

    sockets[n] = ssock;
	//unsigned int ThreadID[100];
	HANDLE haTHREAD = hThread[n];
	//ThreadID[n] = threadID;
	SOCKET sock = ssock;
	requestx.ip = fsin.sin_addr ;
	requestx.port = fsin.sin_port;

	while(recv(sock, body, sizeof(body), 0) > 0){
		printf("%s", body);
		send(sock, message(header), 2048, 0);
		printf("%s", message(header));
	}

	sock = NULL;

	deleteClient(n);
	if (CloseHandle(haTHREAD) == TRUE) {
		printf("geschafft,die Thread zu schliessen.\n");
	}
	*body = NULL;
	(void)closesocket(sock);
	return 0;
}

int main(){
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
	{
		return 1;
	}

    int port=80;
    SOCKET Listen_Sock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN serverAddr;
    ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);           
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(Listen_Sock, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));
	listen(Listen_Sock, SOMAXCONN);

    number = -1;
	int len = sizeof(SOCKADDR);
	while (1){
		ssock = accept(Listen_Sock, (struct sockaddr *)&fsin, &len);
		number++;
		hThread[number] = (HANDLE)_beginthreadex(NULL, 0, response, NULL, 0, &threadID);
	}

    closesocket(Listen_Sock);
	for (int i = 0; i <= sizeof(sockets); i++) {
		closesocket(sockets[i]);
	}
	closesocket(ssock);

	CloseHandle(hThread);
	_getch();
	WSACleanup();
    return 0;
}