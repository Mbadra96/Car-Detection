#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
 
using namespace std;
using namespace cv;


Mat rotate(Mat frame,double angle);
void DetectAndDisplay(Mat img,int x,int y);
CascadeClassifier car_cascade;
String xmlfile="/home/badra/image/xmls/cars.xml";


class VideoProcessing {
	public:
		VideoProcessing(String file,bool r,int x,int y) {
			VideoCapture cap(file);
			if (!cap.isOpened()) {
				cout << "Could not find the video\n";
			}


			while (1) {
				Mat frame;
				if (cap.read(frame)) {
					if(r){
						frame=rotate(frame,-90);
						resize(frame,frame,Size(frame.cols*0.5,frame.rows*0.5),0,0,CV_INTER_LINEAR);
					}
					
					DetectAndDisplay(frame,x,y);
				}

				if ((char)waitKey(10) == 27) {
					// if "esc" is pressed end the program
					cout << "Closing the program because esc pressed\n";
					break;
				}
			}
		}
		~VideoProcessing() {
			destroyWindow("Dataset");
		}
};
int main(int argc, char** argv)
{
	//String file="/home/badra/image/videos/4.mp4";
    //String file="/home/badra/image/videos/5.avi";
    String file="/home/badra/image/videos/Dataset.mp4";
    
    car_cascade.load(xmlfile);
    VideoProcessing video(file,0,150,150);                
    return 0;
}
void DetectAndDisplay(Mat frame,int x,int y){
    Mat frame_gray;
    cvtColor(frame,frame_gray,CV_BGR2GRAY);
    equalizeHist(frame_gray,frame_gray);
    // Detect Cars
    vector<Rect> cars;
    car_cascade.detectMultiScale(frame_gray, cars, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(x,y) );
    for( int i = 0; i < cars.size(); i++ )
    {
        //Point center( cars[i].x + cars[i].width*0.5, cars[i].y + cars[i].height*0.5 );
        //ellipse(frame, center, Size( cars[i].width*0.5, cars[i].height*0.5), 0, 0, 360, Scalar( 0,0,0 ), 4, 8, 0 );
        rectangle(frame,cars[i],Scalar(0,0,0),3,8,0);
    }
  imshow( "Detected cars", frame );
}

Mat rotate(Mat frame,double angle){
	Mat dst;
	Point2f pt(frame.cols/2.,frame.rows/2.);
	Mat r= getRotationMatrix2D(pt,angle,1.0);
	warpAffine(frame,dst,r,Size(frame.cols,frame.rows));
	return dst;
}
