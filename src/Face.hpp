#ifndef FACE_H
#define FACE_H


#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/contrib/contrib.hpp"

class Face {

public:

    /*
        @Param:
        *   cv::Mat & frame : frame affichée
        *   cv::Mat & frame_gray : frame grisée
        *   cv::Mat & face : frame de chaque visage
        *   cv::Ptr<FaceRecognizer> & model : model généré par la base de donnée de reconnaissance des visages
        *   int & face_recognition : boolean pour activer ou non la reconnaissance des visages
        @return:
        *   bool : variable necessaire pour valider la suite du traitement, car si false stop le traitement
    */
    bool detect(cv::Mat & frame,cv::Mat & frame_gray,cv::Rect face,cv::Ptr<cv::FaceRecognizer> & model,int & face_recognition );
    /*
        @Param:
        *   cv::Rect face: face donné par le haarcascades
        @return:
        *   std::vector<cv::Rect>: tableau de rectangles qui contient les deux côtés d'un visage
    */
    std::vector<cv::Rect> setSides(cv::Rect face);

protected:

};

#endif
