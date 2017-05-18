#include "Eye.hpp"
using namespace cv;



Rect Eye::setEyeZone(Rect const& face_side,Rect const& eye){
    Rect eye_zone= Rect(face_side.x + eye.x+eye.x*0.1,face_side.y + eye.y+(0.4*eye.height),eye.width-(0.20 *eye.width),eye.height-(0.5*eye.height));
    return eye_zone;
}

void Eye::show(int const& j,Mat const& eye_ROI){
    if (j==0){
        namedWindow("right eye",WINDOW_AUTOSIZE);
        imshow( "right eye",  eye_ROI);
    }
    else{
        namedWindow("left eye",WINDOW_AUTOSIZE);
        imshow( "left eye",eye_ROI );
    }
}
