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


	cv::Mat measurementModel = cv::Mat(4, 6, CV_32FC1, measurementModelData);
	m_faceTracker.measurementMatrix = measurementModel.clone();

	// update the process,measurement noise and error covariance
	cv::setIdentity(m_faceTracker.processNoiseCov, cv::Scalar::all(1e-4));
	cv::setIdentity(m_faceTracker.measurementNoiseCov, cv::Scalar::all(1/10));
	cv::setIdentity(m_faceTracker.errorCovPost, cv::Scalar::all(.1));

	std::cout << "DynamicalModel: " << m_faceTracker.transitionMatrix << std::endl;
	std::cout << "MeasurementModel: " << m_faceTracker.measurementMatrix<< std::endl;
	std::cout << "ProcessNoise: " << m_faceTracker.processNoiseCov << std::endl;
	std::cout << "MeasurementNoiseCov: " << m_faceTracker.measurementNoiseCov << std::endl;
	std::cout << "errorCovPost: " << m_faceTracker.errorCovPost << std::endl;

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

	cv::Mat dynamicModel = cv::Mat(4, 6, CV_32FC1, dynamicModelData);
	m_faceTracker.transitionMatrix = dynamicModel.clone();

	float measurementModelData[] = { 1, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 1 };


	cv::Mat measurementModel = cv::Mat(6, 6, CV_32FC1, measurementModelData);
	m_faceTracker.measurementMatrix = measurementModel.clone();

	// update the process,measurement noise and error covariance
	cv::setIdentity(m_faceTracker.processNoiseCov, cv::Scalar::all(1e-4));
	cv::setIdentity(m_faceTracker.measurementNoiseCov, cv::Scalar::all(1/10));
	cv::setIdentity(m_faceTracker.errorCovPost, cv::Scalar::all(.1));

	std::cout << "DynamicalModel: " << m_faceTracker.transitionMatrix << std::endl;
	std::cout << "MeasurementModel: " << m_faceTracker.measurementMatrix << std::endl;
	std::cout << "ProcessNoise: " << m_faceTracker.processNoiseCov << std::endl;
	std::cout << "MeasurementNoiseCov: " << m_faceTracker.measurementNoiseCov << std::endl;
	std::cout << "errorCovPost: " << m_faceTracker.errorCovPost << std::endl;

}

cv::Mat FaceTracker::predict()
{
	return m_faceTracker.predict();
}

void FaceTracker::update(const cv::Mat &inMat)
{
	m_faceTracker.correct(inMat);
}

void FaceTracker::currentPredictDisplayRect(cv::Mat &inImage)
{
	// code for displaying predicted rectangle on image
	cv::Mat tempPred;
	tempPred = m_faceTracker.measurementMatrix * m_faceTracker.statePost;

	std::cout << tempPred;

	cv::Point pt1(tempPred.at<float>(0), tempPred.at<float>(1)); // Display detected faces on main window - live stream from camera
	cv::Point pt2((tempPred.at<float>(0) + tempPred.at<float>(2)), (tempPred.at<float>(1) + tempPred.at<float>(3)));
	cv::rectangle(inImage, pt1, pt2, cv::Scalar(255, 0, 0), 2, 8, 0);

}

void FaceTracker::currentPredictDisplay(cv::Mat &inImage)
{
	// code for displaying predicted region on image
	cv::Rect displayRect;
	
	displayRect.x = 0;
	displayRect.y = 0;
	displayRect.width = 256;
	displayRect.height = 256;

	cv::Mat tempPred;
	tempPred = m_faceTracker.measurementMatrix * m_faceTracker.statePost;

	cv::Rect predFace;
	predFace.x = tempPred.at<float>(0);
	predFace.y = tempPred.at<float>(1);
	predFace.width = tempPred.at<float>(2);
	predFace.height = tempPred.at<float>(3);

	cv::Mat cFace = inImage(predFace).clone();
	resize(cFace, cFace, cv::Size(256, 256), 0, 0, cv::INTER_LINEAR);

	//	// copy 256x256 thumbnail to the original image
	//	// Help from: http://answers.opencv.org/question/37568/how-to-insert-a-small-size-image-on-to-a-big-image/
	cFace.copyTo(inImage(displayRect));
}

void FaceTracker::setFirstState(cv::Mat &inMat)
{
	m_faceTracker.statePre.at<float>(0) = inMat.at<float>(0);
	m_faceTracker.statePre.at<float>(1) = inMat.at<float>(1);
	m_faceTracker.statePre.at<float>(2) = 0;
	m_faceTracker.statePre.at<float>(3) = 0;
	m_faceTracker.statePre.at<float>(4) = inMat.at<float>(2);
	m_faceTracker.statePre.at<float>(5) = inMat.at<float>(3);
}

FaceTracker::~FaceTracker()
{

}
