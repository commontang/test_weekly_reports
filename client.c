#include "head.h"
 
int main()
{   int clientFd;
    clientFd=socket(AF_INET,SOCK_STREAM,0);
    //socket函数创建socket，对应于普通文件的打开操作，socket()返回一个socket描述符，唯一标识一个socket
    //这里用clientFd存放客户端的socket的描述符
    if(clientFd < 0) {
        printf("socket error:%s\n",strerror(errno));
        exit(-1);}
    //创建失败，打印错误信息，终止
    //成功，进入连接部分

    struct sockaddr_in serveraddr;
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(SERVERPORT);
    inet_pton(AF_INET,SERVERIP,&serveraddr.sin_addr);
    //先将sockaddr_in结构类型的serveraddr清零，
    //再指定协议族为AF_INET，端口号为宏12345,IP地址为192.168.0.23，均转化为了网络字节序
    int ret;
    ret=connect(clientFd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    //连接服务端
    //此时ret存放connect函数返回值
    if(ret!=0){
        close(clientFd);
        printf("connect error:%s\n",strerror(errno));
        exit(-1); }
    //连接失败，关闭socket,打印错误信息,终止
    //成功，进入读写部分
    
    char buf[MAXBUFFER];
    while(1){
        bzero(buf,sizeof(buf));
        //定义buf作为缓存区
        //并清空
        scanf("%s",buf);
        write(clientFd,buf,sizeof(buf));
        bzero(buf,sizeof(buf));
        read(clientFd,buf,sizeof(buf));
        printf("echo:%s\n",buf);
        //将客户端的输入存放到缓存区，再从缓存区写入socket,并清空缓存区
        //从socket读取数据存放到缓存区，再从缓存区打印该内容
    }
    close(clientFd);
    //客户端使用ctrl c终止程序，关闭客户端socket
    return (EXIT_SUCCESS);
}