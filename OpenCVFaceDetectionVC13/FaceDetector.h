#pragma once
#include "Common.h"
class FaceDetector
{
private:
	// objects for detecting face
	cv::CascadeClassifier m_faceDetector;
	std::vector<cv::Rect> m_detectedFaces;
	bool currentStatus;
public:
	FaceDetector();
	FaceDetector(std::string inStr);

	void detectFaces(const cv::Mat &inImage);
	void displayFaceRect(cv::Mat &inImage);
	bool getCurrentStatus();
	cv::Mat getFaceRect();
	~FaceDetector();
};

