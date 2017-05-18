#ifndef PUPIL_H
#define PUPIL_H


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

class Pupil {

public:
    /*
        minglo_detect: Permet d'avoir le point(x,y) tel que Intensité(x) et Intensité(y) sont minimum
        @Param :
        *	cv::Mat & dst: image sur la quelle est faite la detection
    */
    void minglo_detect(cv::Mat & dst);
    /*
        avgBlack_detect: Permet d'avoir le point P(x,y) tel que P est le barycentre des pts noirs
        @Param :
        *	cv::Mat & dst: image sur la quelle est faite la detection
    */
    void avgBlack_detect( cv::Mat & dst);
    /*
        blob_detect: Permet d'avoir le point P(x,y) tel que P est le centre de masse de l'iris
        @Param :
        *	cv::Mat & dst: image sur la quelle est faite la detection
    */
    void blob_detect( cv::Mat & dst);
    /*
        houghtCircles_detect: Permet d'avoir le point P(x,y) tel que P est le centre du cercle donné par la méthode de hough
        @Param :
        *	cv::Mat & dst: image sur la quelle est faite la detection
    */
    void houghCircles_detect( cv::Mat & dst);
    /*
        croissance_detect: Permet d'avoir le point P(x,y) tel que P est le barycentre trouvé avec la méthode de croissance de région
        @Param :
        *	cv::Mat & dst: image sur la quelle est faite la detection
    */
    void croissance_detect(cv::Mat & dst);
    /*
        draw: Permet de dessiner le centre trouvé par l'une des méthodes au dessus
        @Param :
        *	cv::Mat & frame: frame principale.
        *   cv::Point eye_pos: position de l'oeil dans le rectangle face.
        @return:
        *   cv::Point: centre de la pupille par rapport à toute la frame.
    */
    cv::Point draw(cv::Mat & frame,cv::Point eye_pos);
    /*
        detectedGaze: Permet de detecter le regard en IR
        @Param :
        *	cv::Mat frame: frame principale.
        *   cv::Rect eye: rectangle délimitant la zone de l'oeil.
        @return:
        *   boolean: si regard trouvé.
    */
    bool detectedGaze(cv::Mat frame, cv::Rect eye);
    /*
        cropIrisZone: permet de selectionner la zone de l'iris
        @Param :
        *	cv::Mat eye_zone: rectangle de l'oeil
        @return:
        *   cv::Mat: Image contenant que l'iris
    */
    cv::Mat cropIrisZone(cv::Mat eye_zone);
    /*
        getRatio: permet d'avoir le ration blanc/noir
        @Param :
        *	cv::Mat eye_zone_cropped: zone de l'iris
        @return:
        *   int ratio: rapport pixels_Blancs/pixels_Noirs
    */
    int getRatio(cv::Mat eye_zone_cropped);

protected:
    cv::Point center;
    /*
        searching: Permet de retrouver un point noir s'il existe et de trouver ses voisins de la même couleur et de les additionner au barycentre en entrée 
        @Param :
        *	int & x: abcisse du point à vérifier
        *   int & y: ordonnée du point à vérifier
        *   int *cpt: pointeur sur le compteur de pixels_Noirs
        *   double *xb: pointeur sur abscisse du barycentre
        *   double *yb: pointeur sur ordonnée du barycentre
        *   cv::Mat & dst: image à parcourir
    */
    void searching(int & x, int & y,int *cpt, double *xb, double *yb, cv::Mat & dst);
};

#endif
