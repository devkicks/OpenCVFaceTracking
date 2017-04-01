#include "Common.h"
#include "FaceDetector.h"

int main()
{
	FaceDetector faceDetect; // using default face detector settings

	cv::VideoCapture inVideo("data\\AddedVideo.mp4");
	int inVidWidth = (int)inVideo.get(CV_CAP_PROP_FRAME_WIDTH);
	int inVidHeight = (int)inVideo.get(CV_CAP_PROP_FRAME_HEIGHT);
	int inVidFrameRate = (int)inVideo.get(CV_CAP_PROP_FPS);

	// We are only interested in the video from 8:30 to 12:45
	int startingTime = (8 * 60 /*minutes to seconds */ + 30 /*seconds*/) * /* convert to milliseconds*/1000;
	int endingTime = (8 * 60 /*minutes to seconds */ + 32 /*seconds*/) * /* convert to milliseconds*/1000;

	// seek to the starting time
	inVideo.set(CV_CAP_PROP_POS_MSEC, (double)startingTime);

	// using lossless H264 codec to write back - this is partly dependent on the CODECS installed on your machine
	// if the video fails to write try replacing CV_FOURCC with another CODEC - MJPG or MP42 - or replace it with -1 to select CODEC using a popup
	cv::VideoWriter outVideo("data\\ProcessedVideo.mp4", CV_FOURCC('X', '2', '6', '4'), inVidFrameRate, cv::Size(inVidWidth, inVidHeight));

	// read video and write back processed video
	cv::Mat inImage;
	int ch = 0;
	int currentTime = (int)inVideo.get(CV_CAP_PROP_POS_MSEC);

	// stop when all video done or use interrupts by pressing ESC key (27)
	while (inVideo.read(inImage) && ch != 27 && currentTime < endingTime)
	{
		std::cout << "Current Time " << currentTime << " Ending Time " << endingTime << std::endl;

		// Process input video stream
		faceDetect.detectFaces(inImage);

		cv::Mat outImage = faceDetect.getImageFacesDisplay();
		// Write processed image
		outVideo.write(outImage);
		cv::imshow("Show Image", inImage);
		ch = cv::waitKey(1);
		currentTime = (int)inVideo.get(CV_CAP_PROP_POS_MSEC);
	}
	outVideo.release();
}
//// Function Headers
//void myDetectAndDisplay(cv::Mat &frame);
//void createVideo();
//
//
//// Global variables
//// Copy this file from opencv/data/haarscascades to target folder
//std::string face_cascade_name = "data\\haarcascade_frontalface_alt.xml";
//cv::CascadeClassifier face_cascade;
//std::string window_name = "Capture - Face detection";
//int filenumber; // Number of file to be saved
//std::string filename;
//
//// Function main
//int main(void)
//{
//	// Load the cascade
//	if (!face_cascade.load(face_cascade_name)){
//		printf("--(!)Error loading\n");
//		return (-1);
//	}
//	cv::VideoCapture cap("data\\AddedVideo.mp4");
//
//	// Read the image file
//	cv::Mat frame;
//
//	int ch = cv::waitKey(10);
//
//	while (ch != 27)
//	{
//		if (!cap.read(frame))
//			break;
//		// Apply the classifier to the frame
//		if (!frame.empty()){
//			myDetectAndDisplay(frame);
//		}
//		else{
//			printf(" --(!) No captured frame -- Break!");
//		}
//
//		ch = cv::waitKey(1);
//	}
//	return 0;
//}
//
//// Function main
//void createVideo(void)
//{
//	cv::VideoCapture inVideo(0);
//
//	cv::Mat inMat;
//	while (inMat.empty())
//		inVideo >> inMat;
//
//	//cv::VideoWriter outVideo("writtenVideo.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(640, 480));
//	cv::VideoWriter outVideo("writtenVideo.mp4", CV_FOURCC('X', '2', '6', '4'), 30, cv::Size(640, 480));
//	std::cout << inMat.rows << ", " << inMat.cols << std::endl;
//	int ch = cv::waitKey(1);
//	while (ch != 27)
//	{
//		inVideo >> inMat;
//		outVideo.write(inMat);
//
//		cv::imshow("sh", inMat);
//		ch = cv::waitKey(1);
//	}
//	outVideo.release();
//}
//
//
//// Function detectAndDisplay
//void myDetectAndDisplay(cv::Mat &frame)
//{
//	std::vector<cv::Rect> faces;
//	cv::Rect dispRect;
//
//	dispRect.x = 0;
//	dispRect.y = 0;
//	dispRect.height = 256;
//	dispRect.width = 256;
//
//	cv::Mat frame_gray;
//	cv::Mat crop;
//
//	cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
//	cv::equalizeHist(frame_gray, frame_gray);
//
//	// Detect faces
//	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(10, 10));
//	if (faces.size() > 0)
//	{
//		crop = frame(faces[0]).clone();
//		resize(crop, crop, cv::Size(256, 256), 0, 0, cv::INTER_LINEAR); // This will be needed later while saving images
//		//
//
//		////imwrite(filename, gray);
//
//		cv::Point pt1(faces[0].x, faces[0].y); // Display detected faces on main window - live stream from camera
//		cv::Point pt2((faces[0].x + faces[0].height), (faces[0].y + faces[0].width));
//		cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0), 2, 8, 0);
//
//		// Form a filename
//		//filename = "";
//		//stringstream ssfn;
//		//ssfn << filenumber << ".png";
//		//filename = ssfn.str();
//		//filenumber++;
//
//
//		// Show image
//		//string text;
//		//   stringstream sstm;
//
//		//   sstm << "Crop area size: " << roi_b.width << "x" << roi_b.height << " Filename: " << filename;
//		//   text = sstm.str();
//
//		//   putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
//		//crop = cv::Mat::zeros(256, 256, CV_8UC3);
//	}
//	else
//		crop = cv::Mat::zeros(256, 256, CV_8UC3);
//
//	// copy 256x256 thumbnail to the original image
//	// Help from: http://answers.opencv.org/question/37568/how-to-insert-a-small-size-image-on-to-a-big-image/
//	crop.copyTo(frame(dispRect));
//
//	cv::imshow("Disp Image", frame);
//	cv::imshow("Crop", crop);
//}