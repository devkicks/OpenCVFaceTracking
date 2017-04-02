#include "FaceDetector.h"


FaceDetector::FaceDetector()
{
	// load default facedetector
	if (!m_faceDetector.load("data\\haarcascade_frontalface_alt.xml"))
	{
		std::cout << "FaceDetector: Error loading" << std::endl;
		exit(EXIT_FAILURE);
	}

}

FaceDetector::FaceDetector(std::string inStr)
{
	// load specified facedetector file
	if (!m_faceDetector.load(inStr))
	{
		std::cout << "FaceDetector: Error loading" << std::endl;
		exit(EXIT_FAILURE);
	}
}

// Function detectFaces: detects faces in an input image
void FaceDetector::detectFaces(const cv::Mat &inImage)
{
	cv::Mat inImageGray;

	cvtColor(inImage, inImageGray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(inImageGray, inImageGray);

	// Detect faces
	this->m_faceDetector.detectMultiScale(inImageGray, this->m_detectedFaces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(10, 10), cv::Size(200, 200));

	if (m_detectedFaces.empty())
	{
		currentStatus = false;
	}
	else
	{
		// check if the first face is actually a face
		cv::Mat firstFace = inImage(m_detectedFaces[0]).clone();
		//cv::Mat colorMask = cv::Mat::zeros(firstFace.size(), CV_8UC1);
		cv::cvtColor(firstFace, firstFace, CV_BGR2HSV);

		double countPixelsSkin = 0;
		for (int i = 0; i < firstFace.cols; i++)
			for (int j = 0; j < firstFace.rows; j++)
			{
				// Help from: http://vision.cse.psu.edu/people/kyleB/faceTracker/index.shtml
				if (firstFace.at<cv::Vec3b>(j, i)[0] > 245 || firstFace.at<cv::Vec3b>(j, i)[0] < 25)
					countPixelsSkin++;

			}
		if(countPixelsSkin > 0.3*((double)(firstFace.cols*firstFace.rows)))
			currentStatus = true;
		else
			currentStatus = false;
	}

	
}



void FaceDetector::displayFaceRect(cv::Mat &inImage)
{
	// display detected faces onto an image
	for (int i = 0; i < m_detectedFaces.size(); i++)
	{
		cv::Point pt1(m_detectedFaces[i].x, m_detectedFaces[i].y); // Display detected faces on main window - live stream from camera
		cv::Point pt2((m_detectedFaces[i].x + m_detectedFaces[i].width), (m_detectedFaces[i].y + m_detectedFaces[i].height));
		cv::rectangle(inImage, pt1, pt2, cv::Scalar(0, 255, 0), 2, 8, 0);
	}

}

bool FaceDetector::getCurrentStatus()
{
	return currentStatus;
}

cv::Mat FaceDetector::getFaceRect()
{
	// perhaps there could be a better way of recognizing faces?
	// Maybe use skin color to detect false positives
	// Or histogram matching with face from previous frames
	// Or feature matching to see if it is the same face as the speaker
	cv::Mat rectMat;

	rectMat = cv::Mat::zeros(4, 1, CV_32FC1);

	std::cout << "Num of Faces: " << m_detectedFaces.size() << std::endl;

	rectMat.at<float>(0) = m_detectedFaces[0].x;
	rectMat.at<float>(1) = m_detectedFaces[0].y;
	rectMat.at<float>(2) = m_detectedFaces[0].width;
	rectMat.at<float>(3) = m_detectedFaces[0].height;

	return rectMat;
}

FaceDetector::~FaceDetector()
{
	// release memory
	
}
