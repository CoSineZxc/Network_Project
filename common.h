

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

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
