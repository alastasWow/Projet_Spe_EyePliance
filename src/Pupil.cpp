#include <list>
#include "Pupil.hpp"

using namespace std;
using namespace cv;


void Pupil::minglo_detect(Mat & dst){
    //ordonnée du point avec le Minimum d'intensité
    int rowm(0);
    //minimum global d'intensité suivant y
    long int mingloy=1000000000;
    //parcours de l'image
    for( int y = 0; y < dst.rows; y++ ){
        uchar *row;
        long int minloc(0);
        for( row=dst.ptr<uchar>(y); row < dst.ptr<uchar>(y)+dst.cols; row++ ){
            //somme d'intensité suivant y
            minloc+=*row;
        }
        if(minloc<=mingloy){
            mingloy=minloc;
            rowm=y;
        }
    }
    //abscisse du point avec le Minimum d'intensité
    int colm(0);
    //minimum global d'intensité suivant x
    long int minglox=1000000000;
    //parcours de l'image
    for( int x = 0; x < dst.cols; x++ ){
        uchar* col=dst.ptr<uchar>(x);
        long int minloc(0);
        for( int y = 0; y < dst.rows; y++ ){
            //somme d'intensité suivant x
            minloc+=dst.at<uchar>(y,x);
        }
        if(minloc<minglox){
            minglox=minloc;
            colm=x;
        }
    }
    //set center
    center=Point(cvRound(colm),cvRound(rowm));
}


void Pupil::searching(int & x, int & y,int *cpt, double *xb, double *yb, Mat & dst){
    if(int(dst.at<uchar>(y,x))==0){
        //rendre le pixel noir en pixel blanc
        dst.at<uchar>(y,x)=255;
        //création d'une liste qui contiendra les pixels noirs voisins
        list<Point> waitlist;
        //recherche parmis les voisins
        for(int i=-1;i<2;++i){
            for(int j=-1;j<2;++j){
                if(dst.at<int>(y+i,x+j)==0){
                    //rajouter pixel noir dans la liste
                    waitlist.push_back(Point(x+j,y+i));
                }
            }
        }
        //additionner x et y à xb et yb
        *xb=*xb+x;
        *yb=*yb+y;
        //incrémentation du compteur
        *cpt=*cpt+1;
        //traitemant de la liste
        while(!waitlist.empty()){
            //appel récursif
            searching(waitlist.front().x,waitlist.front().y,cpt,xb,yb,dst);
            waitlist.pop_front();
        }
    }
}

void Pupil::croissance_detect(Mat & dst){

        double  xb(0);
        double  yb(0);
        int cpt(0);
        int y=1;
        //parcous de l'image
        while(y<dst.rows-1){
            int x=1;
            while(x<dst.cols-1){
                searching(x,y,&cpt,&xb,&yb,dst);
                x++;
            }
            y++;
        }
        //définition du barycentre
        xb=xb/(cpt);
        yb=yb/(cpt);
        center=Point(xb,yb);
}

void Pupil::avgBlack_detect( Mat & dst){
	int nbPixel = 0;
	int black_x=0;
	int black_y=0;
	for(int x = 0; x < dst.cols; x++){
		for(int y = 0; y < dst.rows; y++){
			if( (int) (dst.data[(x*dst.rows+y)] ) == 0){
				black_x+=x;
				black_y+=y;
				(nbPixel)++;
			}
        }
    }
	if (nbPixel!=0){
        center = Point(cvRound(black_x/nbPixel), cvRound(black_y/nbPixel));
    }
}

void Pupil::blob_detect( Mat & dst){
    //création du detecteur de blob
    SimpleBlobDetector detector;
    //vecteur contenant les KeyPoints
    std::vector<KeyPoint> keypoints;
    //detection du centre de masse
    detector.detect( dst, keypoints );
    if(keypoints.size()==1)
        center=Point(cvRound(keypoints[0].pt.x), cvRound(keypoints[0].pt.y));
}

void Pupil::houghCircles_detect( Mat & dst){
    //vecteur de points avec 3 Paramètres (Center_x,Center_y,R)
    vector<Vec3f> circles;
    //detection des cercles
	HoughCircles(dst, circles, CV_HOUGH_GRADIENT,7, dst.cols,270, 10);
	if (0<circles.size()){
        center=Point(cvRound(circles[0][0]), cvRound(circles[0][1]));
	}
}


Point Pupil::draw(Mat & frame,Point eye_pos){
    circle( frame,center+eye_pos , 2, Scalar(0,0,255), -1, 8, 0 );
    return center+eye_pos;
}


/////IR
bool Pupil::detectedGaze(Mat frame, Rect eye){
    //Creation d'un rectangle autour de la pupille detectée
	int radius=3;
	cv::Rect r= Rect(center.x-radius+eye.x, center.y-radius+eye.y, radius*2,radius*2);
	cv::Mat croppedRef(frame, r);
	cv::Mat cropped;

	croppedRef.copyTo(cropped);

	rectangle(frame,r,Scalar( 0, 255, 0 ), 1, 8, 0);
	int cpt =0;
    //on compte les pixels d'intensité  > 200
	for(int x = 0; x < cropped.cols; x++) // Calcul des pixels
	{
		for(int y = 0; y < cropped.rows; y++)
		{
			if( (int) (cropped.data[(x*cropped.cols+y)] ) > 200)
			{
				cpt++;
			}
		}

	}
	if (cpt>0){
		return true;
	} else {
		return false;
	}
}





Mat Pupil::cropIrisZone(Mat eye_zone){
    /// On parcours notre oeil pour tracer un rectangle autour des points noirs (contenant l'iris)
	///on initialise a l'opposé et on reduit a zone
	int x_maxB=0;
	int x_minB=eye_zone.cols;
	int y_maxB=0;
	int y_minB=eye_zone.rows;
	for( int y = 0; y < eye_zone.rows; y++ ){
		uchar *row;
		int x=0;
		for( row=eye_zone.ptr<uchar>(y); row < eye_zone.ptr<uchar>(y)+eye_zone.cols; row++ ){
			int intensity =*row;
			if(intensity==0){
				if ( x < x_minB){
					x_minB=x;
				}
				if (x > x_maxB){
					x_maxB=x;
				}

				if ( y < y_minB){
					y_minB=y;
				}
				if (y > y_maxB){
					y_maxB=y;
				}
			}
				x++;
		}
	}

	Rect r = Rect(x_minB, y_minB,x_maxB-x_minB, y_maxB-y_minB);
	Mat  eye_zone_cropped= Mat(eye_zone,r);
    //On renvoie une matrice contenant le rectangle autour de l'iris
	return eye_zone_cropped;
}


int Pupil::getRatio(Mat eye_zone_cropped){
    //On parcours la matrice contenant l'iris pour compter les pixels blanc et noirs
	int nbPixelW = 0;
	int nbPixelB = 0;
	for( int y = 0; y < eye_zone_cropped.rows; y++ ){
        uchar *row;
        for( row=eye_zone_cropped.ptr<uchar>(y); row < eye_zone_cropped.ptr<uchar>(y)+eye_zone_cropped.cols; row++ ){
            int intensity =*row;
			if(intensity==255){
				(nbPixelW)++;
			}else {
				(nbPixelB)++;
			}
        }
    }

    //On retourne le ratio pixel Blanc/Noir
	if (nbPixelB!=0){
		return  nbPixelW/nbPixelB;
	}else {
		return -1;
	}


}
