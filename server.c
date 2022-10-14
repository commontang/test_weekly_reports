#include "head.h"
 
int main()
{   int serverFd;
    serverFd=socket(AF_INET,SOCK_STREAM,0);
    //socket函数创建socket，对应于普通文件的打开操作，socket()返回一个socket描述符，唯一标识一个socket
    //这里用serverFd存放服务端的socket的描述符，socket描述符存在于协议族空间，还需要调用bind函数给它确定一个地址
    if(serverFd < 0){   
        printf("socket error:%s\n",strerror(errno));
        exit(-1);}
    //创建失败，打印错误信息，终止
    //成功，进入绑定部分

    struct sockaddr_in serveraddr;
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(SERVERPORT);
    inet_pton(AF_INET,SERVERIP,&serveraddr.sin_addr);
    //先将sockaddr_in结构类型的serveraddr清零，
    //再指定协议族为AF_INET，端口号为宏12345,IP地址为192.168.0.23，均转化为了网络字节序
    int ret;
    ret=bind(serverFd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    //将IP和端口绑定给socket
    //此时ret存放bind函数返回值
    if(ret!=0) {
        close(serverFd);
        printf("bind error:%s\n",strerror(errno));
        exit(-1);}
    //绑定失败，关闭socket，打印错误信息，终止
    //成功，进入监听部分

    ret=listen(serverFd,5);
    //socket()函数创建的socket默认是一个主动类型的，listen函数将socket变为被动类型的，等待客户的连接请求
    //此时ret存放listen函数返回值
    if(ret!=0){
       close(serverFd);
       printf("listen error:%s\n",strerror(errno));
       exit(-1);}
    //监听失败，关闭socket,打印错误信息,终止
    //成功，进入接受连接部分

    struct sockaddr_in clientaddr;
    socklen_t len;
    len=sizeof(clientaddr);
    bzero(&clientaddr,sizeof(clientaddr));
    //定义clientaddr来存储客户端的协议地址，先清零
    //定义len来存储客户端的协议地址的长度，先赋值
    int connfd;
    char readBuf[MAXBUFFER]={0};
    char ip[40]={0};
    //定义confd来存储已连接的socket的描述字，定义readbuf作为缓存区
    //定义ip字符数组来存储客户端的点分十进制ip地址用于在服务端打印一个提示
    while (1){
        connfd = accept(serverFd, (struct sockaddr *) &clientaddr, &len);
        printf("%s 连接到服务器 \n",inet_ntop(AF_INET,&clientaddr.sin_addr,ip,sizeof(ip)));
        //接受客户端的连接，返回已连接的socket的描述字，
        //存放返回的已连接的socket描述字到connfd，打印客户端IP连接到服务器的提示       
        if (connfd< 0) {
            printf("accept error : %s\n", strerror(errno));
            continue; }
        //接受连接失败，打印错误信息，重启大循环
        //成功，进入读写部分
        
        while((ret=read(connfd,readBuf,MAXBUFFER))){
            write(connfd,readBuf,MAXBUFFER);
            bzero(readBuf,MAXBUFFER);}
        //从已连接的socket读取数据存放到缓存区，从缓存区向已连接的socket写入，并清空缓存区
        //当ret被赋值0，即已连接的socket读到结束，终止小循环
        if(ret==0)  printf("客户端关闭连接\n");  
        else  printf("read error:%s\n",strerror(errno));
        //ret为0，打印正常关闭提示  
        //否则，打印错误信息
        
        close(connfd);
    }
    close(serverFd);
    //读写小循环结束，关闭已连接的socket
    //大循环结束，关闭服务端socket

    return 0;
}



