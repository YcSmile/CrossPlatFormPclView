#include "PointCloudMap.h"
// pcl 进行类封装
//
//
//
//



// http://www.pcl-users.org/Begginging-PCL-td4026994.html

// If you want to declare a Point Cloud as a member of a class I have found the best method is: 

// .h 
// private: 
// pcl::PointCloud<pcl::PointXYZRGBA>::Ptr CloudPtr; 

// .cpp 
// CloudPtr.reset( new pcl::PointCloud<pcl::PointXYZ> ); 


// Also, if you are just new to c++ you could do yourself some favours and declare a TypeDef for your PCL members: 

// TypeDef PointT pcl::PointXYZRGBA; 
// TypeDef PointCloudT pcl::PointCloud< PointT >; 

// This will save you a LOT of extra typing + headaches later, particularly if you are going to class out multiple modules. Either do this at the top of your Main if your going monolithic, or create typedef.cpp and include it in all of your headers. 


// Hope this helps.

PointCloudMap::PointCloudMap()
{
    // creat viewer
    viewer.reset(new pcl::visualization::PCLVisualizer("3D Viewer"));
    cloud.reset(new pcl::PointCloud<pcl::PointXYZRGBA>);
}

PointCloudMap::~PointCloudMap()
{
}
void PointCloudMap::InitMap()
{
    // 创建Map
    LoadPoints();
    ShowPoints();
}

 

  
void PointCloudMap::viewerOneOff (pcl::visualization::PCLVisualizer& viewer)  
{  
    // viewer->setBackgroundColor (1.0, 0.5, 1.0);
    // pcl::PointXYZ o;  
    // o.x = 1.0;  
    // o.y = 0;  
    // o.z = 0;  
    // viewer->addSphere (o, 0.25, "sphere", 0);  
    // std::cout << "i only run once" << std::endl;
  
}  
  
void PointCloudMap::viewerPsycho (pcl::visualization::PCLVisualizer& viewer)  
{  
    // static unsigned count = 0;  
    // std::stringstream ss;  
    // ss << "Once per viewer loop: " << count++;  
    // viewer->removeShape ("text", 0);  
    // viewer->addText (ss.str(), 200, 300, "text", 0);  
  
    // //FIXME: possible race condition here:  
    // user_data++;  
}  

void PointCloudMap::LoadPoints()
{

    pcl::io::loadPCDFile ("../my_point_cloud.pcd", *cloud);  

}
void PointCloudMap::ShowPoints()
{
        //blocks until the cloud is actually rendered  
    // viewer->showCloud(cloud);  
  
    // //use the following functions to get access to the underlying more advanced/powerful  
    // //PCLVisualizer  
  
    // //This will only get called once  
    // viewer->runOnVisualizationThreadOnce (&PointCloudMap::viewerOneOff);  
  
    // //This will get called once per visualization iteration  
    // viewer->runOnVisualizationThread (&PointCloudMap::viewerPsycho);  
}