#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#define TRUE 0
#define FALSE -1
#define DEFAULT_DEV "/dev/ttyUSB0"
#define DEFAULT_BAUND B115200
#define DEFAULT_DATABIT CS8
#define DEFAULT_PAR ~PARENB
#define DEFAULT_INP ~INPCK
#define DEFAULT_STOPBIT ~CSTOPB
#define DEFAULT_INTERVAL 11

#define STR_NUM 9

#define DEBUG
//#define FROM_ARGV

//����ģʽ�¶�ȡ�����ȡ��������
#ifdef DEBUG 
#define READ_BUFFER_SIZE 32
#endif

//���û�ж����FROM_ARGV����pstrΪָ�����飬�����Ԫ����ָ�������Ϊ��Ҫ���͵�����
#ifndef FROM_ARGV
static const char *pstr[] = {0, "AT", "AT", "AT+CMGF=1", "AT+CMGS=", "\"", "13474669578", "\"", ";\r", (char *)26};
#else  //����pstr������������Ϊargv�����򽫷���main�������ܵ��ַ�������
#define pstr argv
#endif


int open_dev(const char *dev_name);
int set_port(const int fd);
int send_data(const int fd, const char *buffer, const int buffer_len);
#ifdef DEBUG
int read_data(const int fd, char *read_buffer);
#endif

 

int main(int argc, char *argv[])
{
int fd;
int i;
char *dev_name = DEFAULT_DEV;
#ifdef DEBUG
char read_buffer[READ_BUFFER_SIZE];
int read_buffer_size;
#endif

//�򿪴���
if((fd = open_dev(dev_name)) == FALSE){
perror("open error!");
return -1;
}

//���ô���
if(set_port(fd) == FALSE){
perror("set error!");
return -1;
}

//��������
do{
for(i = 1; i < STR_NUM; i++){
send_data(fd, pstr[i], strlen(pstr[i]));
}
#ifdef DEBUG
read_buffer_size = read_data(fd, read_buffer);
printf("read: %s\nsize: %d\n", read_buffer, read_buffer_size);
#endif

}while(0);

close(fd);

return 0;
}

//�򿪴���
int open_dev(const char *dev_name)
{
return open(dev_name, O_RDWR);
}

int set_port(const int fd)
{
//��������
struct termios opt;
if(tcgetattr(fd, &opt) != 0){
return FALSE;
}
cfsetispeed(&opt, DEFAULT_BAUND);
cfsetospeed(&opt,DEFAULT_BAUND);
tcsetattr(fd,TCSANOW,&opt);

opt.c_cflag &= ~CSIZE;

//��������λ
opt.c_cflag |= DEFAULT_DATABIT;

//����У��λ
opt.c_cflag &= DEFAULT_PAR;
opt.c_iflag &= DEFAULT_INP;

//����ֹͣλ
opt.c_cflag &= DEFAULT_STOPBIT;

tcflush(fd, TCIFLUSH);
opt.c_cc[VTIME] = DEFAULT_INTERVAL;
opt.c_cc[VMIN] = 0;
if(tcsetattr(fd, TCSANOW, &opt) != 0){
return FALSE;
}

return TRUE;
}

//��������    
int send_data(const int fd, const char *buffer, const int buffer_len)
{
return write(fd, buffer, buffer_len);
}

#ifdef DEBUG
//��ȡ����
int read_data(const int fd, char *read_buffer)
{
return read(fd, read_buffer, READ_BUFFER_SIZE);
}
#endif
