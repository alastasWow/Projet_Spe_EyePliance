#include "Face.hpp"

using namespace cv;
using namespace std;





bool Face::detect(Mat & frame,Mat & frame_gray,Rect face,Ptr<FaceRecognizer> & model,int & face_recognition ){
    rectangle(frame, face, Scalar( 255, 0, 0), 1, 8, 0);
    bool valid=false;
    if(face_recognition==1){
        //la personne detectée
        string Pname ;
        Mat face_rec = Mat(frame_gray,face) ;
        //reconnaissance de visage en  utilisant le model
        int label = -1; double confidence = 0;
        model->predict(face_rec,label,confidence);
        Pname = "-----";
        //label dtermine une personne
        switch (label){
            case 40: Pname = "BADR";
                valid=true;
                break;
            case 41: Pname = "AHMED";
                valid=true;
                break;
            default: Pname = "unknown";
                break;
        }
        //affichage de  Pname dans le coin supérieur du rectangle face
        putText(frame, Pname, Point(face.x, face.y), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(0, 255, 0), 1.0);
        return valid;
    }else
        return !valid;
}

std::vector<Rect> Face::setSides(Rect face){
    std::vector<cv::Rect> sides;
    sides.push_back(Rect(face.x, face.y, face.width/2, face.height));
    sides.push_back(Rect(face.x+face.width/2, face.y, face.width/2, face.height));
    return sides;
}
