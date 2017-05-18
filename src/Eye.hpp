#ifndef EYE_H
#define EYE_H


#include "opencv2/highgui/highgui.hpp"


class Eye: public cv::Rect {

public:

    /*
        @Param:
        * Rect const& face_side : côté d'un visage
        * Rect const& eye : eye fourni par le haarcascade
        @return:
        * Retourne un rectangle qui sera par la suite la base de notre traitement
    */
    cv::Rect setEyeZone(cv::Rect const& face_side,cv::Rect const& eye);

    /*
        @Param:
        *   int const& j: j=0 => right eye sinon left eye
        *   Mat const& eye_ROI: image à afficher lors de l'exécution
    */
    void show(int const & j,cv::Mat const & eye_ROI);


};

#endif
