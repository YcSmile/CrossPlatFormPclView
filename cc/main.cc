// 跨平台进行点云数据传输和展示


#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

#include <pthread.h>

#include "PclPointCient.h"

#include <pcl/pcl_base.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>

using namespace std;
using namespace LiDarPointPack;


int user_data;  
  
void viewerOneOff (pcl::visualization::PCLVisualizer& viewer)  
{  
    viewer.setBackgroundColor (1.0, 0.5, 1.0);  
    pcl::PointXYZ o;  
    o.x = 1.0;  
    o.y = 0;  
    o.z = 0;  
    viewer.addSphere (o, 0.25, "sphere", 0);  
    std::cout << "i only run once" << std::endl;  
  
}  
  
void viewerPsycho (pcl::visualization::PCLVisualizer& viewer)  
{  
    static unsigned count = 0;  
    std::stringstream ss;  
    ss << "Once per viewer loop: " << count++;  
    viewer.removeShape ("text", 0);  
    viewer.addText (ss.str(), 200, 300, "text", 0);  
  
    //FIXME: possible race condition here:  
    user_data++;  
}  

pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);  
pcl::visualization::CloudViewer viewer("Cloud Viewer");  
// PclClient回调函数
// 重新装填点云
void ReloadCloud(PointDataList pd)
{
    cout<<"reload";
    // 清空
    cloud->clear();
    cloud->points.resize( pd.points_size());
    for(int c_ =0;c_ < pd.points_size();c_++)
    {
        cloud->points[c_].x = pd.points(c_).coordinate().x();
        cloud->points[c_].y = pd.points(c_).coordinate().y();
        cloud->points[c_].z = pd.points(c_).coordinate().z();
    }
    viewer.showCloud(cloud);  
    cout<<"reload Succ"<<endl;
}

// 主函数
int main()
{
    pthread_t revcThreads;
    PclPointCient client = PclPointCient();
    client.callBF = &ReloadCloud;
    // 创建
    // 启动异步线程进行TCP接收
    int ret = pthread_create(&revcThreads,NULL,&PclPointCient::ReceiveHelper,(void *)&client);
    if(ret != 0)
    {
        cout<<"pthread_create error : "<<ret<<endl;
    }
    // 加载pcd文件
    //pcl::io::loadPCDFile ("/home/yfei/git_cloud/pcl/c/pcl1/my_point_cloud.pcd", *cloud);  
  
    //blocks until the cloud is actually rendered  
    //viewer.showCloud(cloud);  
  
    //use the following functions to get access to the underlying more advanced/powerful  
    //PCLVisualizer  
  
    //This will only get called once  
    viewer.runOnVisualizationThreadOnce (viewerOneOff);  
  
    //This will get called once per visualization iteration  
    viewer.runOnVisualizationThread (viewerPsycho);  
    while (!viewer.wasStopped ())  
    {  
        //you can also do cool processing here  
        //FIXME: Note that this is running in a separate thread from viewerPsycho  
        //and you should guard against race conditions yourself...  
        user_data++;  
    }  
    return 0; 
}
