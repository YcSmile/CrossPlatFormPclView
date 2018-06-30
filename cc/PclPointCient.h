#ifndef PclPointCient_H
#define PclPointCient_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netdb.h>
#include <vector>
#include <pthread.h>
#include "PclPointCloudPoint.pb.h"

using namespace LiDarPointPack;
using namespace std;


class PclPointCient
{
  private:
  
    /* data */
    // 参数设置
    int sock = -1;
    std::string address;
    int port;
    struct sockaddr_in server;
    bool running = false;
    static const int buffersize = 102400;
    char buffer[buffersize];

    // 创建buffer
    // 定义回调函数类型
    typedef void(*CallbackFun)(PointDataList l_);

  public:
    PclPointCient(/* args */);
    ~PclPointCient();

    bool InitClient(string address, int port);
    long Parsebyte2Long(uint8_t *buff, int buff_len);
    uint8_t *Parselong2byte(long d);
    CallbackFun callBF = NULL;          // 回调函数
    //void (*CallBackFun)( const PointDataList l_);
    // void (*CallbackFun)(void);
    // callBF = NULL;
    void Receive();

    // static void *ReceiveHelper(void *context);
    static void *ReceiveHelper(void *context)
    {
        cout<<"working"<<endl;
        ((PclPointCient *)context)->Receive();

    }

};

#endif
