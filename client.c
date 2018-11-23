#include "client_func.h"
#define MAXCONN 1200
int main(int argc, char *argv[])
{
	struct SocketPara socketPara;
	GetSocketPara(argc, argv, &socketPara, 0);//�������в������д�����ɸ�ֵ
	if (socketPara.isFork == 1)//��fork����ʽ��������
	{
		int i = 0;
		pid_t pid = 0;
		for (i = 0; i<socketPara.connectNum; i++)
		{
			pid = fork();
			if (pid == -1)//fork�ӽ���ʧ��
			{
				perror("[Fork error]\n");
				return -1;
			}
			else if (pid == 0)//�ӽ���
			{
				int client_sockfd;
				int conn;
				if ((client_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
				{
					perror("[Create socket error]\n");
					return -1;
				}
				//���÷�������ʽ
				if (socketPara.isBlock == 0)
					SetSocketNonblock(client_sockfd);

				//������IP���˿ںŰ�
				struct sockaddr_in remote_addr;//�ͻ��������ַ�ṹ��
				memset(&remote_addr, 0, sizeof(remote_addr));
				remote_addr.sin_family = AF_INET;//ͨ��Э��Ϊ������
				remote_addr.sin_port = htons(socketPara.port);//�������˿ں�
				remote_addr.sin_addr.s_addr = inet_addr(socketPara.ip);//������IP��ַ

				if (socketPara.isBlock == 0)//��������ʽ
				{

				}
				else //������ʽ
				{
					if ((conn = connect(client_sockfd, (struct sockaddr*)&remote_addr, sizeof(remote_addr))) < 0)
					{
						perror("connect error");
						return -1;
					}
					printf("[connect-%d success]\n", i);
					communicate(conn, getpid(), 1, i);
					exit(0);
				}	
			}
			else
			{
				while (1)
					sleep(2);
			}
		}
	}
	else //����������ɴ������첽
	{
		int client_sockfd[MAXCONN], conn[MAXCONN];

	}
}