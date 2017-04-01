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
	this->m_faceDetector.detectMultiScale(inImageGray, this->m_detectedFaces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(10, 10));

	this->m_imageFacesRect = inImage.clone();

	// display detected faces onto an image
	for(int i = 0; i < m_detectedFaces.size(); i++)
	{
		cv::Point pt1(m_detectedFaces[i].x, m_detectedFaces[i].y); // Display detected faces on main window - live stream from camera
		cv::Point pt2((m_detectedFaces[i].x + m_detectedFaces[i].height), (m_detectedFaces[i].y + m_detectedFaces[i].width));
		cv::rectangle(m_imageFacesRect, pt1, pt2, cv::Scalar(0, 255, 0), 2, 8, 0);
	}
}

cv::Mat FaceDetector::getImageFacesDisplay()
{
	return m_imageFacesRect;
}

FaceDetector::~FaceDetector()
{
	// release memory
	
}
