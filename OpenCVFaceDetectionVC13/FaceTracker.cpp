#include "FaceTracker.h"


FaceTracker::FaceTracker()
{
	// load default params
	this->m_dynamicModelDim = 6;
	this->m_measureModelDim = 4;
	this->m_faceTracker.init(m_dynamicModelDim, m_measureModelDim);

	// initialize the dynamic model and measurement model
	float dynamicModelData[] = { 1, 0, 1, 0, 0, 0,
		0, 1, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0,
		0, 0, 0, 1, 0, 0,
		0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 1 };

	cv::Mat dynamicModel = cv::Mat(6, 6, CV_32FC1, dynamicModelData);
	m_faceTracker.transitionMatrix = dynamicModel.clone();

	float measurementModelData[] = { 1, 0, 0, 0, 0, 0,
								0, 1, 0, 0, 0, 0,
								0, 0, 0, 0, 1, 0,
								0, 0, 0, 0, 0, 1};


	cv::Mat measurementModel = cv::Mat(6, 6, CV_32FC1, measurementModelData);
	m_faceTracker.measurementMatrix = measurementModel.clone();

	// update the process,measurement noise and error covariance
	cv::setIdentity(m_faceTracker.processNoiseCov, cv::Scalar::all(1e-4));
	cv::setIdentity(m_faceTracker.measurementNoiseCov, cv::Scalar::all(10));
	cv::setIdentity(m_faceTracker.errorCovPost, cv::Scalar::all(.1));
}

FaceTracker::FaceTracker(int dynamDim, int measDim)
{
	// load default params
	this->m_dynamicModelDim = dynamDim;
	this->m_measureModelDim = measDim;
	this->m_faceTracker.init(m_dynamicModelDim, m_measureModelDim);

	// initialize the dynamic model and measurement model
	float dynamicModelData[] = { 1, 0, 1, 0, 0, 0,
		0, 1, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0,
		0, 0, 0, 1, 0, 0,
		0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 1 };

	cv::Mat dynamicModel = cv::Mat(6, 6, CV_32FC1, dynamicModelData);
	m_faceTracker.transitionMatrix = dynamicModel.clone();

	float measurementModelData[] = { 1, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 1 };


	cv::Mat measurementModel = cv::Mat(6, 6, CV_32FC1, measurementModelData);
	m_faceTracker.measurementMatrix = measurementModel.clone();

	// update the process,measurement noise and error covariance
	cv::setIdentity(m_faceTracker.processNoiseCov, cv::Scalar::all(1e-4));
	cv::setIdentity(m_faceTracker.measurementNoiseCov, cv::Scalar::all(10));
	cv::setIdentity(m_faceTracker.errorCovPost, cv::Scalar::all(.1));
}

cv::Mat FaceTracker::predict()
{
	return m_faceTracker.predict();
}

void FaceTracker::update(const cv::Mat &inMat)
{
	m_faceTracker.correct(inMat);
}

void FaceTracker::currentPredictDisplay(cv::Mat &inImage)
{
	// code for displaying predicted rectangle on image
	cv::Mat tempPred;
	tempPred = m_faceTracker.measurementMatrix * m_faceTracker.statePost;
	
	std::cout << tempPred;

	cv::Point pt1(tempPred.at<float>(0), tempPred.at<float>(1)); // Display detected faces on main window - live stream from camera
	cv::Point pt2((tempPred.at<float>(0) + tempPred.at<float>(2)), (tempPred.at<float>(1) + tempPred.at<float>(3)));
	cv::rectangle(inImage, pt1, pt2, cv::Scalar(255, 0, 0), 2, 8, 0);

}

FaceTracker::~FaceTracker()
{

}
