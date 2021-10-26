#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <wiringSerial.h>

using namespace cv;
using namespace std;
using namespace filesystem;

#define SCALE 0.2
#define BINS 8
#define NUM 12 // dir file count
#define NOTHING 0
#define CAN 1 // default object is can
#define BOTTLE 2
#define dataSetPath "/home/pi/box_project/dataset/" // data path && capture path
#define BUZZER1 4

static string firstInputFileName; // capture img name
static int numOfLabels;


//dir get files
vector<string> get_files()
{
	vector<string> filepath;

	for (const directory_entry& entry : directory_iterator(dataSetPath))
	{
		//cout << entry.path().string() << endl;
		filepath.push_back(entry.path().string());
	};
	firstInputFileName = filepath[0];

	return filepath;
}
//CAN && bottle Object find
int findObject()
{
	string lastOutputObject = "CAN";
	vector<string> imgNames = get_files();
	vector<float> matching_score;
	for (string imgpath : imgNames) {
		cout << imgpath << endl;
	}

	Mat imgs[NUM];
	Mat imgsHLS[NUM];
	for (int i = 0; i < NUM; ++i)
	{
		imgs[i] = imread(imgNames[i], IMREAD_COLOR);
		if (imgs[i].data == 0)
		{
			cout << "Unable to read" << imgNames[i] << endl;
		}
		cvtColor(imgs[i], imgsHLS[i], COLOR_BGR2HLS);
	}
	imgNames.erase(imgNames.begin());
	Mat histogram[NUM];

	int channal_numbers[] = { 0,1,2 };
	for (int i = 0; i < NUM; ++i)
	{
		int* number_bins = new int[imgsHLS[i].channels()];
		for (int ch = 0; ch < imgsHLS[i].channels(); ch++)
		{
			number_bins[ch] = BINS;
		}
		float ch_range[] = { 0.0,255.0 };
		const float* channel_ranges[] = { ch_range,ch_range,ch_range };
		calcHist(&imgsHLS[i], 1, channal_numbers, Mat(), histogram[i], 			 			imgsHLS[i].channels(), number_bins, channel_ranges);
		normalize(histogram[i], histogram[i], 1.0);
	}
	cout << "Image Comparison by HISRCMP_CORREL" << endl;

	
	for (int i = 1; i < NUM; i++)
	{
		
		matching_score.push_back(compareHist(histogram[0], histogram[i], HISTCMP_CORREL));
		cout << matching_score[i-1] << endl;	
	}

	int i = 0;
	double max = 0;
	double min = 0;
	for (int x = 0; x < NUM - 1; x++) {
		cout << firstInputFileName << "-" << imgNames[x] << " : " << matching_score[x] << 			endl;
		max < matching_score[x] ? max = matching_score[x], i = x : min > 			matching_score[x] ? min = matching_score[x] : 0;
	}

	const string sp = dataSetPath;
	const string maxImgName = imgNames[i].substr(sp.length());
	cout << "best score : " << max << " same object is = " << maxImgName << endl;

	if (maxImgName.find("no") != string::npos) {
		lastOutputObject = "BOTTLE";
		cout << "Object is : " << lastOutputObject << endl;
		return BOTTLE;
	}
	else if (maxImgName.find("yes") != string::npos) {
		lastOutputObject = "CAP BOTTLE";
		cout << "Object is : " << lastOutputObject << endl;
		//뚜껑 보틀 예외 처리 필요. door close
	}
	else {
		// what object? && CAN 
		cout << "Object is : " << lastOutputObject << endl;
		return CAN;
	}
	return NOTHING;
}

//bottle label color matching
bool colorLabelfind(Mat& img)
{
	Mat img_hsv, blue_mask;
	cvtColor(img, img_hsv, COLOR_BGR2HSV);
	Scalar lower_blue = Scalar(100, 30, 30);
	Scalar upper_blue = Scalar(140, 255, 255);

	inRange(img_hsv, lower_blue, upper_blue, blue_mask);

	int morph_size = 2;
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1),
		Point(morph_size, morph_size));
	morphologyEx(blue_mask, blue_mask, MORPH_OPEN, element);
	morphologyEx(blue_mask, blue_mask, MORPH_CLOSE, element);


	Mat blue_image;
	bitwise_and(img, img, blue_image, blue_mask);

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

		if (area > 500) // 100 < area = no color show
		{
			circle(img, Point(centerX, centerY), 5, Scalar(255, 0, 0), 1);
			rectangle(img, Point(left, top), Point(left + width, top + height), 				Scalar(0, 0, 255), 1);
			putText(img, to_string(area), Point(left, top), FONT_HERSHEY_PLAIN, 2.0, 				Scalar(255, 0, 0), 1);
			//imshow("img_mask", blue_mask);
			//imshow("img_result", blue_image);
			//예외 처리 보틀라벨 존재
			buzzer();
			imshow("find labelcolor", img);			
			return false;			
		}
	}
	return true;
}

//Ras -> uno Servo
bool rasForAduServo()
{
	int fd;

	char str;
	if((fd = serialOpen("/dev/ttyACM0", 9600)) < 0) {
		printf("Device file open error!! use sudo ...\n");
		return false;
	}	
	str = "Y";
			
	serialPutchar(fd, str);
	serialFlush(fd);
	return true;
}

//read bottle or can sensor 
bool read_sensor() {
   int fd;
   bool b = false;
   char str;
   if((fd = serialOpen("/dev/ttyACM0", 9600)) < 0) {
      printf("Device file open error!! use sudo ...\n");

   }

   while(1) {
      if(serialDataAvail(fd)){
         char newchar =serialGetchar(fd);
         printf("%c",newchar);
         fflush(stdout);         
         break;   
      }
   }
   b = true;
   return b;
}

//예외 처리 부저 울림
void buzzer(){
   
   wiringPiSetup();
   pinMode(BUZZER1,OUTPUT);//BCM_GPIO 23
   
   for(;;)
   {
      digitalWrite(BUZZER1,1);
      delay(1000);
      digitalWrite(BUZZER1,0);
      delay(1000);
      break;
   }
}

int main(int ac, char** av)
{
	Mat img;
	Mat img_Capture;
	int objectNum = NOTHING;
	string imgPath = dataSetPath;

	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "camera video error " << endl;
		return -1;
	}

	//camera on
	while (1)
	{
		cap >> img_Capture;
		imshow("camera", img_Capture);

		int key = waitKey(33);
		if (read_sensor()) { // capture key ctrl + - key 45
			if (!img.empty()) {
				img.release();
				destroyWindow("img_color");
			}
			imwrite(imgPath + "0.png", img_Capture); // input img file dir
			objectNum = findObject(); // find object
		}else if(key == 27) break; // exit key esc 

		if (objectNum == CAN) {
			// object is CAN 
			cout << "is CAN" << endl;
			//door open
			if(rasForAduServo()){
				
			}
			continue;
		}

		if (objectNum == BOTTLE) { //object is bottled
			Mat readImg = imread(imgPath + "0.png");
			if(colorLabelfind(readImg)){
				rasForAduServo();
				cout << "servo On && 13 led on" << endl;
			}else
				cout << "bottle on label << endl;
			
			continue;
		}
		
	}
	return 0;
}
