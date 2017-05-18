#include "Filtre.hpp"

using namespace cv;

Mat&  Filtre::minglo_method( Mat & dst){
    //Equalizes the histogram of a grayscale image
    equalizeHist( dst, dst );
    //segmentation method
    //permet d'avoir des images binaires
    threshold(dst, dst,30, 255, THRESH_BINARY);
    //Blurs an image using the median filter
    //une matrice de 7x7
    medianBlur(dst,dst,7);
    //avoir une image binaire apres la moyenne
    threshold(dst, dst,1, 255, THRESH_BINARY);
    //Dilates an image by using a specific structuring element
    //Type of a morphological operation :  MORPH_ELLIPSE
    dilate(  dst, dst,MORPH_ELLIPSE );

	return dst;

}

Mat&  Filtre::croissance_method( Mat & dst){
    //Equalizes the histogram of a grayscale image
    equalizeHist( dst, dst );
    //threshold==>THRESH_TOZERO
    int THRESH=40;
    for( int y = 0; y < dst.rows; y++ ){
        uchar *row;
        for( row=dst.ptr<uchar>(y); row < dst.ptr<uchar>(y)+dst.cols; row++ ){
            if(*row<THRESH)
                *row=0;
        }
    }
    //Blurs an image using the median filter
    //une matrice de 7x7
    medianBlur(dst,dst,7);
    return dst;

}

Mat&  Filtre::avgBlack_method( Mat & dst){
	equalizeHist( dst, dst );
	threshold(dst,dst, 30, 255, THRESH_BINARY);
    //Erodes an image by using a specific structuring element.
    //Type of a morphological operation :  MORPH_ELLIPSE
    //augmente la taille des taches noires
	erode(  dst, dst,MORPH_ELLIPSE );
    //Blurs an image using the median filter
    //une matrice de 9x9
	medianBlur(dst, dst, 9);
	return dst;
}

Mat& Filtre::blob_method(Mat & dst){
    int lowThreshold=200;
    /*
    Canny :Finds edges in an image using the [Canny86] algorithm.
    @Param:
    *   image – single-channel 8-bit input image.
    *   edges – output edge map; it has the same size and type as image .
    *   threshold1 – first threshold for the hysteresis procedure.
    *   threshold2 – second threshold for the hysteresis procedure.
    */
    Canny( dst, dst,lowThreshold , lowThreshold*3);
    //Blurs an image using the gaussien filter
    GaussianBlur(dst,dst,Size(7,7),0);
    equalizeHist( dst, dst );
    return dst;
}

Mat&  Filtre::houghCircles_method( Mat & dst){
    equalizeHist( dst, dst );
    //Blurs an image using the gaussien filter
    GaussianBlur( dst, dst, Size(9, 9), 0, 0 );
    threshold(dst, dst,35, 255, THRESH_BINARY);
    //affiner le résultat
    dilate(  dst, dst,MORPH_ELLIPSE );
    medianBlur(dst,dst,7);
    int lowThreshold=0;
    //detection of edges
    Canny( dst, dst,lowThreshold , lowThreshold*3);

	return dst;

}

Mat& Filtre::IR_method(Mat & dst){
    int lowThreshold=200;
    equalizeHist( dst, dst );
	threshold(dst, dst,25, 255, THRESH_BINARY);

    return dst;
}
