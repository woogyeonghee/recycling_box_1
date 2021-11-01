#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

string gstreamer_pipeline (int sensor_id, int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc sensor_id=" + to_string(sensor_id) + " ! video/x-raw(memory:NVMM), width=(int)" + to_string(capture_width) + ", height=(int)" +
           to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + to_string(flip_method) + " ! video/x-raw, width=(int)" + to_string(display_width) + ", height=(int)" +
           to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}
int qrcode();
int control();
int main()
{
	qrcode();
	cout <<"end"<<endl;
	control();
	return 0;
}

int qrcode()
{
	int sensor_id = 1;
    int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int framerate = 60 ;
    int flip_method = 0 ;

    string pipeline = gstreamer_pipeline(sensor_id,
	capture_width,
	capture_height,
	display_width,
	display_height,
	framerate,
	flip_method);
    cout << "Using pipeline: \n\t" << pipeline << "\n";
 
    VideoCapture cap(pipeline, CAP_GSTREAMER);
    if(!cap.isOpened()) {
	cout<<"Failed to open camera."<<endl;
	return (-1);
    }
	QRCodeDetector detector;

    namedWindow("CSI Camera", WINDOW_AUTOSIZE);
    Mat img;
	Mat frame,gray;
    cout << "Hit ESC to exit" << "\n" ;
    while(true)
    {
		cap >> img;
        imshow("carmera img",img);
        frame = img;

        if (frame.empty()) {
            cerr << "Frame load failed!" << endl;
            break;
        }

		cvtColor(frame, gray, COLOR_BGR2GRAY);

        vector<Point> points;

        if (detector.detect(gray, points)) {
            polylines(frame, points, true, Scalar(0, 255, 255), 2);

            String info = detector.decode(gray, points);
            if (!info.empty()) {
                polylines(frame, points, true, Scalar(0, 0, 255), 2);
                cout << "Decoded Data : " << info << endl;
                break;
            }
		}
		imshow("frame", frame);
        waitKey(30);

    }

    cap.release();
    destroyAllWindows() ;
    return 0;
}

int control()
{
	int sensor_id = 0;
    int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int framerate = 60 ;
    int flip_method = 0 ;

    string pipeline = gstreamer_pipeline(sensor_id,
	capture_width,
	capture_height,
	display_width,
	display_height,
	framerate,
	flip_method);
    cout << "Using pipeline: \n\t" << pipeline << "\n";
 
    VideoCapture cap(pipeline, CAP_GSTREAMER);
    if(!cap.isOpened()) {
	cout<<"Failed to open camera."<<endl;
	return (-1);
    }
	QRCodeDetector detector;

    namedWindow("CSI Camera", WINDOW_AUTOSIZE);
    Mat img;
	Mat frame,gray;
    cout << "Hit ESC to exit" << "\n" ;
    while(true)
    {
		cap >> img;
        imshow("carmera img",img);
        frame = img;

        if (frame.empty()) {
            cerr << "Frame load failed!" << endl;
            break;
        }

		cvtColor(frame, gray, COLOR_BGR2GRAY);

        vector<Point> points;

        if (detector.detect(gray, points)) {
            polylines(frame, points, true, Scalar(0, 255, 255), 2);

            String info = detector.decode(gray, points);
            if (!info.empty()) {
                polylines(frame, points, true, Scalar(0, 0, 255), 2);
                cout << "Decoded Data : " << info << endl;
                break;
            }
		}
		imshow("frame", frame);
        waitKey(30);

    }

    cap.release();
    destroyAllWindows() ;
    return 0;
}