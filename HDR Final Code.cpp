#include <opencv2/photo.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;
void loadExposureSeq(String, vector<Mat>&, vector<float>&);
int main()
{
    vector<Mat> images;
    vector<float> times;
    String path = "/home/atul/DeWinter/HDR";
    loadExposureSeq(path, images, times);
    Mat response;
    Ptr<CalibrateRobertson> calibrate = createCalibrateRobertson();
    calibrate->process(images, response, times);
    Mat hdr;
    Ptr<MergeRobertson> merge_Robertson = createMergeRobertson();
    merge_Robertson->process(images, hdr, times, response);
    Mat ldr;
    Ptr<TonemapDurand> tonemap = createTonemapDurand(2.2f);
    tonemap->process(hdr, ldr);
    Mat fusion;
    Ptr<MergeMertens> merge_mertens = createMergeMertens();
    merge_mertens->process(images, fusion);
    imwrite("fusion1.png", fusion * 255);
    imwrite("ldr.png", ldr * 255);
    imwrite("hdr.hdr", hdr);
    return 0;
}
void loadExposureSeq(String path, vector<Mat>& images, vector<float>& times)
{
    path = path + std::string("/");
    ifstream list_file((path + "list1.txt").c_str());
    string name;
    float val;
    while(list_file >> name >> val) {
        Mat img = imread(path + name);
        images.push_back(img);
        times.push_back(1 / val);
    }
    list_file.close();
}
