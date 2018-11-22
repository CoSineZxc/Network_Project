#pragma once
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <fcntl.h>

extern const int TRUE;
extern const int FALSE;
extern char *SERVER_DEFAULTIP;

struct SocketPara
{
	char *ip;       //ip
	int port;       //�˿ں�
	int connectNum; //������
	int isBlock;    //�Ƿ�Ϊ������ʽ
	int isFork;     //�Ƿ�Ϊ���ѽ��̷�ʽ
};

int strcasecmp(const char *s1, const char *s2);
int IsIPAvailable(char *ip);
int GetSocketPara(int argc, char *argv[], struct SocketPara *obj, const int isServer);
int SetSocketNonblock(int fd);