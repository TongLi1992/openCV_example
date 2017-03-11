#include <opencv2/viz.hpp>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;
static void help()
{
    cout
    << "--------------------------------------------------------------------------"   << endl
    << "This program shows how to use makeTransformToGlobal() to compute required pose,"
    << "how to use makeCameraPose and Viz3d::setViewerPose. You can observe the scene "
    << "from camera point of view (C) or global point of view (G)"                    << endl
    << "Usage:"                                                                       << endl
    << "./transformations [ G | C ]"                                                 << endl
    << endl;
}
static Mat cvcloud_load()
{
    ifstream ifs2("bunny.ply");
    int count = 0;
    int header_length = 13;
    string str;
    while(!ifs2.eof()){
        count += 1;
        getline(ifs2, str);
    }
    count -= header_length;
    ifs2.close();
    ifstream ifs("bunny.ply");
    Mat cloud(1, 10000, CV_32FC3);

    for(size_t i = 0; i < header_length; ++i) {
        getline(ifs, str);
        cout<<str<<endl;
    }
    Point3f* data = cloud.ptr<cv::Point3f>();
    float dummy1, dummy2;
    for(size_t i = 0; i < 10000; ++i)
    {
        cout<<data[i].x<<"----------------------------------   "<<endl;
        ifs >> data[i].x >> data[i].y >> data[i].z >> dummy1 >> dummy2;
        cout<<data[i].x<<endl;
        //double temp=0;
        //ifs>>temp;
        //getline(ifs, str);
        //cout<<str<<endl;
    }
    cloud *= 5.0f;
    return cloud;
}
int main(int argn, char **argv)
{
    help();
    if (argn < 2)
    {
        cout << "Missing arguments." << endl;
        return 1;
    }
    bool camera_pov = (argv[1][0] == 'C');
    viz::Viz3d myWindow("Coordinate Frame");
    myWindow.showWidget("Coordinate Widget", viz::WCoordinateSystem());
    Vec3f cam_pos(3.0f,3.0f,3.0f), cam_focal_point(3.0f,3.0f,2.0f), cam_y_dir(-1.0f,0.0f,0.0f);
    Affine3f cam_pose = viz::makeCameraPose(cam_pos, cam_focal_point, cam_y_dir);
    Affine3f transform = viz::makeTransformToGlobal(Vec3f(0.0f,-1.0f,0.0f), Vec3f(-1.0f,0.0f,0.0f), Vec3f(0.0f,0.0f,-1.0f), cam_pos);
    Mat bunny_cloud = cvcloud_load();
    cout<<"1\n";
    viz::WCloud cloud_widget(bunny_cloud, viz::Color::green());
    cout<<"2\n";
    Affine3f cloud_pose = Affine3f().translate(Vec3f(0.0f,0.0f,3.0f));
    cout<<"3\n";
    Affine3f cloud_pose_global = transform * cloud_pose;
    if (!camera_pov)
    {
        viz::WCameraPosition cpw(0.5); // Coordinate axes
        viz::WCameraPosition cpw_frustum(Vec2f(0.889484, 0.523599)); // Camera frustum
        myWindow.showWidget("CPW", cpw, cam_pose);
        myWindow.showWidget("CPW_FRUSTUM", cpw_frustum, cam_pose);
    }
    myWindow.showWidget("bunny", cloud_widget, cloud_pose_global);
    if (camera_pov)
        myWindow.setViewerPose(cam_pose);
    myWindow.spin();
    return 0;
}
