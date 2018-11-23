#include "client_func.h"
/**********************
* �������ƣ�getLocalTime
* ��    �ܣ���õ�ǰ���̵�ϵͳʱ�䣬��yyyy-mm-dd hh:mm:ss�ĸ�ʽ
* ��    ��������������ɺ�s������ַ���
* ��    �أ�
* ˵    ������1900.01.01��Ϊʱ���׼
***********************/
void getLocalTime(char s[])
{
	time_t ttime;
	ttime = time(NULL);
	struct tm *stTime = localtime(&ttime);
	sprintf(s, "%d-%02d-%02d %02d:%02d:%02d\n",
		stTime->tm_year + 1900,
		stTime->tm_mon + 1,
		stTime->tm_mday,
		stTime->tm_hour,
		stTime->tm_min,
		stTime->tm_sec);
}

/**********************
* �������ƣ�getFileName
* ��    �ܣ�����������ѧ��STUNO�����̺�pid����������Ҫ����ļ���
* ��    ����
* ��    �أ�
* ˵    ����ѧ��.���̺�.pid.txt
***********************/
void getFileName(int pid, char fileName[])
{
	memset(fileName, 0, FILENAMELEN);
	char pid_str[10];
	itoa(pid, pid_str, 10);
	strcpy(fileName, STUNO);
	strcat(fileName, ".");
	strcat(fileName, pid_str);
	strcat(fileName, ".pid.txt");
}

/**********************
* �������ƣ�writeToFile
* ��    �ܣ���client�˷��͵�stuno,pid,time,�Լ�������ɵ�����д��ָ���ļ�
* ��    ����pid_tmp--�������pid��	timestr--ָ����ʽ(yyyy-mm-dd hh:mm:ss)��ʱ���ַ���
randData_bak--client�����ɲ����͵��������	fileName--Ҫд����ļ���
* ��    �أ�
* ˵    �������У�ע�⻻�з���linux�µĸ�ʽ
***********************/
void writeToFile(int pid_tmp, char timestr[], char randData_bak, char fileName[])
{
	FILE *fp = fopen(fileName, "w");
	if (fp == NULL)
	{
		perror("File open fail!\n");
		exit(-1);
	}
	fputs(STUNO, fp);
	fputc('\n\t', fp);
	char pid_str[10];
	itoa(pid_tmp, pid_str, 10);
	fputs(pid_str, fp);
	fputc('\n\t', fp);
	fputs(timestr, fp);
	fputc('\n\t', fp);
	fputs(randData_bak, fp);
}

/**********************
* �������ƣ�communicate
* ��    �ܣ���server�˵�ͨ������
* ��    ����conn--�����׽���	pid--���̺�
isfork--�Ƿ���fork�ӽ��̵ķ�ʽ��������
socket_id--�ڼ�������
* ��    �أ�
* ˵    �������ݽ�����ɺ󣬽�����д��ָ���ļ�
***********************/
void communicate(int conn, int pid, int isfork, int socket_id)
{
	char readBuf[BUFSIZE];
	char writeBuf[BUFSIZE];
	memset(writeBuf, 0, BUFSIZE);
	memset(readBuf, 0, BUFSIZE);

	if (read(conn, readBuf, 5) < 0)//����S���͵�StuNo,����β��
	{
		perror("[read-1]");
		exit(-1);
	}
	readBuf[5] = '\0';
	if (strcmp(readBuf, "StuNo") != 0)//���յ�����Ϣ������StuNo
	{
		close(conn);//�ж�����
		printf("Connection_%d closed!", pid);
		exit(0);
	}

	//?????????????????????????????????????��ô��ȷ��ת�����򣬲������֮���ٸ�
	int stuno = htonl(atoi(STUNO));//��STUNO������ַ���ת��int�ͺ�ת��������
	if (write(conn, &stuno, 4) < 0)//�����������ѧ��,int
	{
		perror("[write-1]");
		exit(-1);
	}

	/*---------------------------------------------------------------------*/
	memset(readBuf, 0, BUFSIZE);
	if (read(conn, readBuf, 3) < 0)//����S���͵�pid������β��
	{
		perror("[read-2]");
		exit(-1);
	}
	readBuf[3] = '\0';
	if (strcmp(readBuf, "pid") != 0)//���յ�����Ϣ������"pid"
	{
		close(conn);//�ж�����
		printf("Connection_%d closed!", pid);
		exit(0);
	}

	//?????????????????????????????????????��ô��ȷ��ת�����򣬲������֮���ٸ�
	int pid_tmp;
	if (isfork == 1)
		pid_tmp = htonl(pid);
	else
		pid_tmp = htonl(pid << 16 + socket_id);

	if (write(conn, &pid_tmp, 4) < 0)
	{
		perror("[write-2]");
		exit(-1);
	}
	/*=-------------------------------------------------------------*/
	char timestr[TIMESTRLEN];
	memset(timestr, 0, TIMESTRLEN);
	memset(readBuf, 0, BUFSIZE);
	if (read(conn, readBuf, 5) < 0)//����S���͵�TIME����β��
	{
		perror("[read-3]");
		exit(-1);
	}
	if (strcmp(readBuf, "TIME") != 0)//���յ�����Ϣ������"TIME"
	{
		close(conn);//�ж�����
		printf("Connection_%d closed!", pid);
		exit(0);
	}
	getLocalTime(timestr);
	if (write(conn, timestr, 19) < 0)
	{
		perror("[write-3]");
		exit(-1);
	}

	/*-----------------------------------------------------------------*/
	memset(readBuf, 0, BUFSIZE);
	if (read(conn, readBuf, 9) < 0)//����S���͵�"str*****"����β��
	{
		perror("[read-4]");
		exit(-1);
	}
	char tmp[10];
	strncpy(tmp, readBuf + 3, 5);//��ȡ����λ�����
	int randNumber = atoi(tmp);
	if (strncmp(readBuf, "str", 3) != 0 | randNumber<32768)//���յ�����Ϣ������"str*****"
	{
		close(conn);//�ж�����
		printf("Connection_%d closed!", pid);
		exit(0);
	}

	//�����������
	//���ݣ����ں���д���ļ�
	//���Ż���ʹ�ö�̬����ռ䷽ʽ
	char randData_bak[BUFSIZE];
	for (int i = 0; i < randNumber; i++)
	{
		writeBuf[i] = rand() % 256;
		randData_bak[i] = writeBuf[i];
	}
	writeBuf[randNumber] = '\0';
	randData_bak[randNumber] = '\0';

	//Ϊ��ֹ����������ѭ��write
	int nwritten, total = 0;
	while (total != randNumber)
	{
		nwritten = write(conn, writeBuf, randNumber - total);
		if (nwritten == 0)  //������������˷�����д��Ļ����п�����sendbuffer����
		{
			printf("[sendbuffer may be full]\n");
			break;
		}
		if (nwritten == -1) //д��������ܶԷ��عر�������
		{
			perror("[write error]");
			exit(-1);
		}
		total += nwritten;
		strncpy(writeBuf, writeBuf + nwritten, randNumber - total);
		writeBuf[randNumber - total] = '\0';
	}

	/*-------------------------------------------------------------------------*/
	memset(readBuf, 0, BUFSIZE);
	if (read(conn, readBuf, 3) < 0)//����S���͵�end������β��
	{
		perror("[read-5]");
		exit(-1);
	}
	readBuf[3] = '\0';
	if (strcmp(readBuf, "end") != 0)//���յ�����Ϣ������"end"
	{
		close(conn);//�ж�����
		printf("Connection_%d closed!", pid);
		exit(0);
	}
	//��������Ϣд���ļ�
	char fileName[FILENAMELEN];
	getFileName(pid, fileName);
	writeToFile(pid, timestr, randData_bak, fileName);
}