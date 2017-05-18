#include "opencv2/imgproc/imgproc.hpp"


class Filtre
{

	 public:
		 /*
			 @Param :
			 *	cv::Mat & dst: image à modifier
			 @Return:
			 *	cv::Mat&: image apres traitement qui correspond à la méthode du minimum global
		 */
		cv::Mat& minglo_method(cv::Mat & dst);
		/*
			@Param :
			*	cv::Mat & dst: image à modifier
			@Return:
			*	cv::Mat&: image apres traitement qui correspond à la méthode de la moyenne des taches noires
		*/
		cv::Mat& avgBlack_method(cv::Mat & dst);
		/*
			@Param :
			*	cv::Mat & dst: image à modifier
			@Return:
			*	cv::Mat&: image apres traitement qui correspond à la méthode de detection du barycentre des taches
		*/
		cv::Mat& blob_method(cv::Mat & dst);
		/*
			@Param :
			*	cv::Mat & dst: image à modifier
			@Return:
			*	cv::Mat&: image apres traitement qui correspond à la méthode de detection de cercles avec la méthode de hough
		*/
		cv::Mat& houghCircles_method(cv::Mat & dst);
		/*
			@Param :
			*	cv::Mat & dst: image à modifier
			@Return:
			*	cv::Mat&: image apres traitement qui correspond à la méthode de croissance des régions
		*/
		cv::Mat& croissance_method( cv::Mat & dst);
		/*
			@Param :
			*	cv::Mat & dst: image à modifier
			@Return:
			*	cv::Mat&: image apres traitement qui correspond à la méthode infrarouge
		*/
		cv::Mat& IR_method(cv::Mat & dst);

} ;
