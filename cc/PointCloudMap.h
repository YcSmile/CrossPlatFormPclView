#include <iostream>

#include <pcl/pcl_base.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


using namespace std;
class PointCloudMap
{
  private:
    /* data */
    // 点云数据
    // 只能定义数据
    pcl::visualization::CloudViewer viewer;
    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud;  
    int user_data;

  public:
    // pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

    PointCloudMap();
    ~PointCloudMap();

    void viewerPsycho (pcl::visualization::PCLVisualizer& viewer);
    void viewerOneOff (pcl::visualization::PCLVisualizer& viewer);

    void InitMap();
    void LoadPoints();
    void ShowPoints();
};


// bytes 转 stream 转 point
