
#include "PclPointCient.h"
#include "PclPointCloudPoint.pb.h"


using namespace LiDarPointPack;
using namespace std;

PclPointCient::PclPointCient(/* args */)
{
    // 创建客户端
    if( InitClient("127.0.0.1",8001))
    {

        cout << "Init Client Succ"<<endl;
    }
    running = true;
}

PclPointCient::~PclPointCient()
{

}

// 创建客户端;
bool PclPointCient::InitClient(string address, int port)
{
    //
    if (sock == -1)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            cout << "Could not create socket" << endl;
            return false;
        }
    }
    if (inet_addr(address.c_str()) == -1)
    {
        struct hostent *he;
        struct in_addr **addr_list;
        if ((he = gethostbyname(address.c_str())) == NULL)
        {
            herror("gethostbyname");
            cout << "Failed to resolve hostname\n";
            return false;
        }
        addr_list = (struct in_addr **)he->h_addr_list;
        for (int i = 0; addr_list[i] != NULL; i++)
        {
            server.sin_addr = *addr_list[i];
            break;
        }
    }
    else
    {
        server.sin_addr.s_addr = inet_addr(address.c_str());
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // 显示信息
    int fds = connect(sock, (struct sockaddr *)&server, sizeof(server)) ;
    if ( fds< 0)
    {
        cout << fds<<endl;
        perror("connect failed. Error");
        return false;
    }

    return true;
}
// long 转 byte
uint8_t *PclPointCient::Parselong2byte(long d)
{
    uint8_t *buf = (uint8_t *)malloc(8);
    memcpy(buf, (uint8_t *)&d, 8);
    return buf;
}
// byte 转 long
long PclPointCient::Parsebyte2Long(uint8_t *buff, int buff_len)
{
    uint8_t b[8] = {0};
    // 进行指针转化
    memcpy(b, buff, buff_len);

    return (*(long *)b);
}

void PclPointCient::Receive()
{
    // 清空缓存
    uint8_t longBuf[8];
    char tmep[1024];
    cout<<"working"<<endl;
    while (running)
    {
        long recved_index = 0;
        long once_size = 0;
        ssize_t recved_size = 0;
        long recved_total  = 0;

        // 读取数据
        // 读取一个long 值
        memset(buffer, 0, buffersize);  // 清空
        memset(longBuf, 0, 8);          // qingk
        if (recv(sock, longBuf, 8, 0) <= 0)
        {
            // 接收小于等于0 为断开
            cout <<"return";
            return;
        }
        //
        recved_total = Parsebyte2Long(longBuf, 8);
        printf("rece - long %d",recved_total);
        while (recved_index < recved_total)
        {
            if (recved_total - recved_index > 1024)
            {
                once_size = 1024;
            }
            else
            {
                once_size = recved_total - recved_index;
            }
            memset(tmep, 0, 1024);
            ssize_t recv_size = recv(sock, tmep, once_size, 0);
            if (recv_size < 0)
            {
                cout <<"return";
                return;
            }
            // 进行复制
            mempcpy(&buffer[recved_index],tmep,recv_size);
            recved_index += recv_size;        
        }
        // 接收结束
        PointDataList points;
        points.ParseFromArray(buffer,recved_total);
        printf(" count %d ",points.points_size());
        cout << points.points(0).laserid() << endl;
        if(callBF != NULL)
        {
            callBF(points);
        }
    }
}