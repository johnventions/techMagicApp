#pragma once

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/video.hpp"
#include "opencv2/ml.hpp"
#include <chrono>

using namespace cv;
using namespace cv::ml;
using namespace std;


#define MIN_0_TRACE_AREA	7600	//for M
#define MIN_1_TRACE_AREA	30000	//for 0
#define MIN_2_TRACE_AREA	12500	//for '4'
#define MIN_3_TRACE_AREA	23000	//for ~
#define CROPPED_IMG_MARGIN	10		//pixels
#define MAX_TRACE_SPEED		150		//pixels/second (30p/0.2sec)

class ImageProcessor
{
private:
	int _frameWidth;
	int _frameHeight;
	
	Mat cameraFrame;
	Mat _fgMaskMOG2;
	Mat wandMoveTracingFrame;
	
	SimpleBlobDetector::Params _params;
	
	Ptr<SimpleBlobDetector> _blobDetector;
	Ptr<BackgroundSubtractor> _pMOG2;
	
	vector<KeyPoint> blobKeypoints;

	deque<KeyPoint> tracePoints;
		
	Point traceUpperCorner;
	Point traceLowerCorner;
	
	std::chrono::time_point<std::chrono::high_resolution_clock> _lastKeypointTime;
	HOGDescriptor hog;
	double _distance(Point& p, Point& q);
	vector<KeyPoint> wandDetect(ushort[], int _numpixels);
	void ConvertVectortoMatrix(vector<float> inHOG, Mat &outMat);
	Mat cropSaveTrace();
	Mat deskew(Mat& img);

#if ENABLE_SPELL_TRAINING
	void loadTrainLabel(String pathName, vector<Mat> &trainCells, vector<int> &trainLabels);
	void CreateDeskewedTrain(vector<Mat> &deskewedTrainCells, vector<Mat> &trainCells);
	void CreateTrainHOG(vector<vector<float> > &trainHOG, vector<Mat> &deskewedTrainCells);
	void ConvertVectortoMatrix(vector<vector<float> > &inHOG, Mat &outMat);
	void getSVMParams(SVM *svm);
	void SVMtrain(Mat &trainMat, vector<int> &trainLabels);
	//void SVMevaluate(Mat &testResponse, float &count, float &accuracy, vector<int> &testLabels);
#endif

public:
	void init(int frameWidth, int frameHeight);
	Mat getWandTrace(ushort[], int _numpixels);
#if ENABLE_SPELL_TRAINING
	void spellRecognitionTrainer();
#endif
	bool wandVisible();
	bool checkTraceValidity();
	int recognizeSpell();
	void eraseTrace();
};