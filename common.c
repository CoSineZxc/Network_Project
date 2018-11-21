
#include "common.h"
const int TRUE = 1;
const int FALSE = 0;
char *SERVER_DEFAULTIP = "0.0.0.0";

// struct SocketPara
// {
//     char *ip;       //ip
//     int port;       //�˿ں�
//     int connectNum; //������
//     int isBlock;    //�Ƿ�Ϊ������ʽ
//     int isFork;     //�Ƿ�Ϊ���ѽ��̷�ʽ
// };

/**********************
 * �������ƣ�strcasecmp                             
 * ��    �ܣ��ж������ַ����ǲ�����ȣ����漰��Сд                  
 * ��    ����const char* s1,const char* s2 - �����ַ���
 * ��    �أ�int - �������������ǲ������ TRUE/FALSE         
 * ˵    ���������ִ�Сд                               
***********************/
int strcasecmp(const char *s1, const char *s2)
{
    int c1 = 0, c2 = 0;
    do
    {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
    } while (c1 == c2 && c1 != 0);

    return (c1 == c2) ? TRUE : FALSE;
}

/**********************
 * �������ƣ�IsIPAvailable                       
 * ��    �ܣ��ж�IP�Ƿ���ϱ�׼
 * ��    ����const char* const ip
 * ��    �أ�TRUE/FALSE - ip���Ϲ淶/�����Ϲ淶
 * ˵    ����
***********************/
int IsIPAvailable(char *ip)
{
    int i = 0, num = 0;

    //ipȫ��Ϊ������'.'
    for (i = 0; ip[i] != 0; i++)
        if (ip[i] < '0' || ip[i] > '9')
        {
            if (ip[i] == '.')
                num++;
            else
                return FALSE;
        }

    if (num != 3)
        return FALSE;

    //ÿһ��Ҫ�����0-255
    char tmpIP[20] = {0};
    strcpy(tmpIP, ip);
    char *tmp = strtok(tmpIP, ".");
    for (i = 0; i < 4; i++)
    {
        //��ֹ 192.17.2. �������
        if (tmp == NULL)
            return FALSE;

        int tmpNum = atoi(tmp);
        if (tmpNum < 0 || tmpNum > 255)
            return FALSE;

        tmp = strtok(NULL, ".");
    }

    return TRUE;
}

/**********************
 * �������ƣ�GetSocketPara                  
 * ��    �ܣ��ͻ��ˡ�����˵������в�������
 * ��    ����int argc, char *argv[]     - �����в���
 *           struct SocketPara *obj,   - socket����
 *           int isServer=TRUE         - �жϵ�ǰ�Ƿ�Ϊ�ͻ���
 * ��    �أ�
 * ˵    ����
***********************/
int GetSocketPara(int argc, char *argv[], struct SocketPara *obj, const int isServer)
{
    /*��ʼ��*/
    {
        obj->ip = NULL;
        obj->connectNum = 100;
        obj->isFork = FALSE;
        obj->isBlock = FALSE;
        obj->port = -1;
    }

    /*�����в�������*/
    int i = 1;
    int isNonBlockExist = FALSE;
    for (; i < argc; i++)
    {
        //ip
        if (strcasecmp(argv[i], "--ip") == TRUE)
        {
            i++;
            if (i == argc)
            {
                printf("IP error\n");
                exit(-1);
            }

            //�ж�ip�Ƿ���Ϲ淶
            if (IsIPAvailable(argv[i]) == FALSE)
            {
                printf("IP error\n");
                exit(-1);
            };

            obj->ip = argv[i];
        }

        //port
        if (strcasecmp(argv[i], "--port") == TRUE)
        {
            i++;
            if (i == argc)
            {
                printf("port error\n");
                exit(EXIT_FAILURE);
            }

            //�ж�port�Ƿ���Ϲ淶
            int tmpport = atoi(argv[i]);
            if (tmpport < 0)
            {
                printf("port error\n");
                exit(EXIT_FAILURE);
            }

            obj->port = tmpport;
        }

        //block
        if (strcasecmp(argv[i], "--block") == TRUE)
        {
            //���block �� nonblockͬʱ����,block��Ч
            if (isNonBlockExist == FALSE)
                obj->isBlock = TRUE;
        }

        //nonblock
        if (strcasecmp(argv[i], "--nonblock") == TRUE)
        {
            //���block �� nonblockͬʱ����,block��Ч
            obj->isBlock = FALSE;
            isNonBlockExist = TRUE;
        }

        //fork
        if (strcasecmp(argv[i], "--fork") == TRUE)
        {
            obj->isFork = TRUE;
        }

        //nofork
        if (strcasecmp(argv[i], "--nofork") == TRUE)
        {
            obj->isFork = FALSE;
        }

        //num
        if (strcasecmp(argv[i], "--num") == TRUE)
        {
            i++;
            if (i == argc)
            {
                printf("num error\n");
                exit(EXIT_FAILURE);
            }

            //�ж�port�Ƿ���Ϲ淶
            int tmpnum = atoi(argv[i]);
            if (tmpnum < 0)
            {
                printf("num error\n");
                exit(EXIT_FAILURE);
            }

            obj->connectNum = tmpnum;
        }
    }

    /*ȱʡ���*/
    if (obj->ip == NULL)
    {
        if (isServer == TRUE)
            obj->ip = SERVER_DEFAULTIP;
        else
        {
            printf("lack client ip\n");
            exit(EXIT_FAILURE);
        }
    }

    if (obj->port == -1)
    {
        printf("lack port\n");
        exit(EXIT_FAILURE);
    }
}

