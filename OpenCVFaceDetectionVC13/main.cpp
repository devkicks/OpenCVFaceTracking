// Main.cpp
// Code for Face Detection and Tracking
// Face Detection is achieved using Viola Jones classifier
// Face Tracking is achieved using simple Kalman Filter tracking
// Additionally skin tone segmentation is used to detect false positives

#include "Common.h"
#include "FaceDetector.h"
#include "FaceTracker.h"

int main()
{
	bool initTracker = false;
	FaceDetector faceDetect; // using default face detector settings
	FaceTracker faceTrack; // using default KF tracker settings
	
	cv::VideoCapture inVideo("data\\AddedVideo.mp4");
	int inVidWidth = (int)inVideo.get(CV_CAP_PROP_FRAME_WIDTH);
	int inVidHeight = (int)inVideo.get(CV_CAP_PROP_FRAME_HEIGHT);
	int inVidFrameRate = (int)inVideo.get(CV_CAP_PROP_FPS);

	// We are only interested in the video from 8:30 to 12:45
	int startingTime = (8 * 60 /*minutes to seconds */ + 30 /*seconds*/) * /* convert to milliseconds*/1000;
	int endingTime = (12 * 60 /*minutes to seconds */ + 45 /*seconds*/) * /* convert to milliseconds*/1000;

	// seek to the starting time
	inVideo.set(CV_CAP_PROP_POS_MSEC, (double)startingTime);

	// using lossless H264 codec to write back - this is partly dependent on the CODECS installed on your machine
	// if the video fails to write try replacing CV_FOURCC with another CODEC - MJPG or MP42 - or replace it with -1 to select CODEC using a popup
	cv::VideoWriter outVideo("data\\ProcessedVideoCropped.mp4", CV_FOURCC('X', '2', '6', '4'), inVidFrameRate, cv::Size(inVidWidth, inVidHeight));
	cv::VideoWriter outVideoPredRect("data\\ProcessedVideoRect.mp4", CV_FOURCC('X', '2', '6', '4'), inVidFrameRate, cv::Size(inVidWidth, inVidHeight));

	// read video and write back processed video
	cv::Mat inImage;
	int ch = 0;
	int currentTime = (int)inVideo.get(CV_CAP_PROP_POS_MSEC);

	// stop when all video done or use interrupts by pressing ESC key (27)
	while (inVideo.read(inImage) && ch != 27 && currentTime < endingTime)
	{
		//cv::resize(inImage, inImage, cv::Size(inVidWidth, inVidHeight));
		std::cout << "Current Time: " << currentTime << " Ending Time: " << endingTime << std::endl;

		// Process input video stream
		faceDetect.detectFaces(inImage);

		// Predict the tracker state
		faceTrack.predict();

		cv::Mat currentFaceRect;
		if (faceDetect.getCurrentStatus())
		{
			currentFaceRect = faceDetect.getFaceRect();
			if (!initTracker)
			{
				faceTrack.setFirstState(currentFaceRect);
				initTracker = true;
			}

			faceTrack.update(currentFaceRect);
		}

		// write images to video files
		cv::Mat outImage = inImage.clone();
		
		faceTrack.currentPredictDisplay(outImage);
		
		outVideo.write(outImage);

		
		faceDetect.displayFaceRect(outImage);
		faceTrack.currentPredictDisplayRect(outImage);
		outVideoPredRect.write(outImage);

		cv::imshow("Show Image", outImage);
		ch = cv::waitKey(1);
		currentTime = (int)inVideo.get(CV_CAP_PROP_POS_MSEC);
	}
	outVideo.release();
	outVideoPredRect.release();
}
