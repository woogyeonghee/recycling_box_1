#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <algorithm>
#include <filesystem>
#include "CppLinuxSerial/SerialPort.hpp"

using namespace mn::CppLinuxSerial;
using namespace cv;
using namespace std;
using namespace filesystem;

#define SCALE 0.2
#define SAMPLENUM 5 // dir file count
#define dataSetPath "/home/jetson/recycle_Box/dataset/" // data path && capture path

//gscamera attr fuc
const string gstreamer_pipeline () 
{
    int capture_width = 1280;
    int capture_height = 720;
    int display_width =  800;
    int display_height = 600;
    int framerate = 60 ;
    int flip_method = 0;
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + to_string(capture_width) + ", height=(int)" +
           to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + to_string(flip_method) + " ! video/x-raw, width=(int)" + to_string(display_width) + ", height=(int)" +
           to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

//dir get files
const vector<string> get_files()
{
    vector<string> filepath;

    for (const directory_entry& entry : directory_iterator(dataSetPath))
    {
        filepath.push_back(entry.path().string());
    }

    sort(filepath.begin(), filepath.end());

    for(auto i = filepath.begin(); i != filepath.end(); i++)
	cout << *i << endl;

    return filepath;
}

//bottle label color matching
const bool colorLabelfind(Mat& img)
{
    //img = 2 * img;
    int numOfLabels = 0;
    Mat img_hsv, blue_mask;
    cvtColor(img, img_hsv, COLOR_BGR2HSV);
    Scalar lower_blue = Scalar(0, 100, 0);
    Scalar upper_blue = Scalar(255, 255, 255);

    inRange(img_hsv, lower_blue, upper_blue, blue_mask);

    int morph_size = 2;
    Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1),
        Point(morph_size, morph_size));
    morphologyEx(blue_mask, blue_mask, MORPH_OPEN, element);
    morphologyEx(blue_mask, blue_mask, MORPH_CLOSE, element);


    //Mat blue_image;
    //bitwise_and(img, img, blue_image, blue_mask);

    Mat img_labels, stats, centroids;
    numOfLabels = connectedComponentsWithStats(blue_mask, img_labels, stats, centroids, 8, CV_32S);

    int j;
    for (j = 1; j < numOfLabels; j++) {
        int area = stats.at<int>(j, CC_STAT_AREA);
        int left = stats.at<int>(j, CC_STAT_LEFT);
        int top = stats.at<int>(j, CC_STAT_TOP);
        int width = stats.at<int>(j, CC_STAT_WIDTH);
        int height = stats.at<int>(j, CC_STAT_HEIGHT);

        int centerX = centroids.at<double>(j, 0);
        int centerY = centroids.at<double>(j, 1);

        if (area > 1000) // 100 < area = no color show
        {
            circle(img, Point(centerX, centerY), 5, Scalar(255, 0, 0), 1);
            rectangle(img, Point(left, top), Point(left + width, top + height),          Scalar(0, 0, 255), 1);
            putText(img, to_string(area), Point(left, top), FONT_HERSHEY_PLAIN, 2.0,             Scalar(255, 0, 0), 1);

            return false;
        }
    }
    return true;
}

int main(int ac, char** av)
{
   //set serialPort
    SerialPort serialPort1("/dev/ttyACM0", 9600);// arduino port
    SerialPort serialPort2("/dev/getUsbPort", 9600);// node js port
    serialPort1.Open();
    serialPort2.Open();
    
    string read;
    string read2;
    
    int i = 0;
    int y1 = 0;
 	int y2 = 600;
  	int x1 = 200;
  	int x2 = 650;
    
    Mat img;
    Mat subImg;
    Mat subImg_;
    
    vector<Mat> matReadVector(SAMPLENUM);
    vector<Mat> resVecotr(SAMPLENUM);
    vector<Mat> res_normVector(SAMPLENUM);
    vector<double> maxvVector(SAMPLENUM);
    vector<Point> maxlocVector(SAMPLENUM);
    const vector<string> imgNames = get_files();

    for (int i = 0; i < SAMPLENUM; i++) {
    	matReadVector[i] = imread(imgNames[i], IMREAD_COLOR);
    }

    sleep(10);   
    
    cout << "start!" << endl;
    
    //tell ardu it will start
    serialPort1.Write("d");
    while (1)
    {
   	  cout << "while ON!" << endl;
      //check garbage full or not
        do {
            serialPort1.Read(read);          
   		if (read.length() == 1)
      			cout << "read1 : " << read <<endl;
                break;
        } while (1);
        serialPort2.Write(read); 
        do {
            serialPort1.Read(read2);
            if (read2.length() == 1)
      cout << "read2 : " << read2 <<endl;
                break;
        } while (1);
      //send to node js
	VideoCapture cap(gstreamer_pipeline(), CAP_GSTREAMER);    
	if (!cap.isOpened())
      	{
            cout << "camera video error " << endl;
         return -1;
      	}
      	while(1){
         if (!cap.read(img)) {
		      cout<<"Capture read error"<<endl;
      		break;
   		}
		flip(img, subImg, 0);
		//imshow("CSI Camera", subImg);
		 subImg_ = subImg(Range(y1, y2), Range(x1, x2));
  		 subImg_ = subImg_ + Scalar(10, 10, 10);
		 for(int i = 0; i < SAMPLENUM; i++){
		 	matchTemplate(subImg_, matReadVector[i], resVecotr[i], TM_CCOEFF_NORMED);
		 	normalize(resVecotr[i], res_normVector[i], 0, 255, NORM_MINMAX, CV_8U);
		 	minMaxLoc(resVecotr[i], 0, &maxvVector[i], 0, &maxlocVector[i]);
		 }
		 if(maxvVector[0] > 0.7){
		 	serialPort1.Write("N");
		 	cout << "CAP" <<endl;
		 	break;
		 }
		  if(maxvVector[1] > 0.7){
		 	serialPort1.Write("N");
		 	cout << "LINE" <<endl;
		 	break;
		 }
		  if(maxvVector[2] > 0.7){
		 	if(!colorLabelfind(subImg_)){
		 		serialPort1.Write("N");
		 		cout << "ON LABEL" <<endl;
		 	}else{
		 		serialPort1.Write("B");
		 		cout << "SERVO ON BOTTLE" <<endl;
		 	}
		 	break;
		 }
		  if(maxvVector[3] > 0.8){
		 	serialPort1.Write("C");
		 	cout << "SERVO ON CAN" <<endl;
		 	break;
		 }
		  if(maxvVector[4] > 0.7){
		 	serialPort1.Write("N");
		 	cout << "CAP AND LINE" <<endl;
		 	break;
		 }
        //waitKey(1);  
 	//if (keycode == 27) break ;
    
	};
    	 //destroyAllWindows();
	 cap.release();
    };
    serialPort1.Close();
    serialPort2.Close();
    return 0;
}
