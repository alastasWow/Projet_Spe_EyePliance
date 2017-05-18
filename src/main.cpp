#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>

#include "opencv2/objdetect/objdetect.hpp"
#include "Face.hpp"
#include "Eye.hpp"
#include "Filtre.hpp"
#include "Pupil.hpp"
using namespace std;
using namespace cv;

/** Function Headers */
void detect( Mat frame);
/** Global variables */
Mat frame;
Ptr<FaceRecognizer> model;
int camera_type;///0=IR 1=webcam
int face_recognition=0; // 0=sans , 1=avec
int cpt=0;//Cpt determine la durée à partir de laquelle le script s'active
bool watched=false;//true si Cpt = 15
bool named=false;// active la reconnaissance vocale
int frame_watched=0;
/*    /usr/local/share/OpenCV/haarcascades   */
string face_cascade_name = "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt2.xml";
string right_eye_cascade_name = "/usr/local/share/OpenCV/haarcascades/haarcascade_righteye_2splits.xml";
string left_eye_cascade_name = "/usr/local/share/OpenCV/haarcascades/haarcascade_lefteye_2splits.xml";
CascadeClassifier face_cascade;
CascadeClassifier right_eye_cascade;
CascadeClassifier left_eye_cascade;
/** @function main */
int main( int argc, const char** argv ){

	/*Gestion des argv */
	if (argc != 5){
		cerr << "Bad usage: track <VideoSource> <Camera_type> <Bool Face Recognition> <System>" <<endl;
        return -1;

	}
	//Quel type de camera utilisé (webcam ou IR)
	if ( strcmp(argv[2], "webcam")== 0 ){
		camera_type = 1;
	}else if (strcmp(argv[2], "IR")== 0){
		camera_type = 0;
	}else{
		cerr << "bad second argument: either IR or webcam" <<endl;
        return -1;
	}
	//Valide la reconnaissance des visage si argv[3]==1
	if (strcmp(argv[3], "1")== 0 )
		face_recognition=1;
	else if (strcmp(argv[3], "0")!= 0 ){
		cerr << "bad third argument: either 0 or 1" <<endl;
        return -1;
	}
    /*initialisation des  classifieurs*/
    if( !face_cascade.load( face_cascade_name ) ){
        cerr << "erreur loading cascade_face" <<endl;
        return -1;
    }
    if( !right_eye_cascade.load( right_eye_cascade_name )|| !left_eye_cascade.load( left_eye_cascade_name )){
        cerr << "erreur loading cascade_eyes" <<endl;
        return -1;
    }
    /*Model pour la reconnaissance de visage*/
    model = createLBPHFaceRecognizer();
    model->load("../bd/LBPHface.yml");
    /*lecture de la video*/
	//capture est initialisé avec argv[1]
    VideoCapture capture(atoi(argv[1]));
    if (!capture.isOpened())
        return -1;

	/*Redimensionnement*/
	//capture.set(CV_CAP_PROP_FRAME_WIDTH ,320);
	//capture.set(CV_CAP_PROP_FRAME_HEIGHT ,240);

	/*Liste formée de toutes les commandes*/
	vector<string> list_cmd;
	if(strcmp(argv[4], "OSX")==0){
		ifstream cmd_OSX("../commands/commands_OSX.txt", ios::in);  // on ouvre le fichier en lecture
		if(!cmd_OSX.fail()){
		string line;
		while(getline(cmd_OSX, line))
			list_cmd.push_back(line);
		cmd_OSX.close();
		cmd_OSX.close();
		}
		else{
			cerr << "Can not open file list_commands!" << endl;
			cmd_OSX.close();
		}
	}
	else if (strcmp(argv[4], "Linux")==0){
		ifstream cmd("../commands/commands_Linux.txt", ios::in); // on ouvre le fichier en lecture
		if(!cmd.fail()){
		string line;
		while(getline(cmd, line))
			list_cmd.push_back(line);
		cmd.close();
		cmd.close();
		}
		else{
			cerr << "Can not open file list_commands!" << endl;
			cmd.close();
		}
	}
	else{
		cerr << "bad fourth argument: either Linux or OSX" <<endl;
		return -1;
	}

	/*Boucle infinie*/
    while( true ){
		std::cout << "************************ main ***************************" << std::endl;
		/*Contrainte de sortie : mot clé "Jarvis"*/
		while(!named){
			/*Contrainte de sortie : Zone Caméra regardée pendant 15 frames*/
			while(!watched){
				capture >> frame;
				if( !frame.empty() ){
					//Main fonction
				    detect( frame);
					//affichage
					string window_name = "Face/Eyes detection";
					namedWindow(window_name,WINDOW_AUTOSIZE);
					imshow( window_name, frame );
				}
				else {
				    cout <<"pas d'image"<<endl;
				    break;
				}
				char cara = (char) waitKey(1);
				if(cara == 'c' )
					exit(0);
				// Ecrire dans un fichier
				else if(cara== 'f' ) {
					int filenumber;
					stringstream ssfn;
					ssfn <<"../photos/photo"<<filenumber << ".png";
					imwrite(ssfn.str(), frame);
					filenumber++;
				}
			}
			watched=false;
			/*Lancement de la reco vocale pour détecter le mot clé*/
			system("../speech-rec.sh");
			ifstream prop("../build/google_results.txt", ios::in);  // on ouvre le fichier en lecture
			vector<string> list_prop;
	        if(!prop.fail()){
				string line;
				while(getline(prop, line))
					list_prop.push_back(line);
	        	prop.close();
	        }
	        else
	            cerr << "Can not open file list_propositions!" << endl;
			if(list_prop.size()>0)
				if(list_prop[0].compare("Jarvis")==0){
					named=true;
					system(list_cmd[0].substr(7).c_str());
				}

		}
		named=false;
		/*Lancement de la reco vocale pour activer les commandes si elles existent*/
		std::cout << "************************ Yes sir ************************" << std::endl;
		system("../speech-rec.sh");
		system("play ../sound_effects/wait.wav");
		/*Lecture du fichier de reco vocale rendu par google*/
		ifstream prop("../build/google_results.txt", ios::in);  // on ouvre le fichier en lecture
		vector<string> list_prop;
        if(!prop.fail()){
			string line;
			while(getline(prop, line))
				list_prop.push_back(line);
        	prop.close();
        }
        else
            cerr << "Can not open file list_propositions!" << endl;

		/*Recherche du mot clé dans la liste des commandes*/
		bool found_cmd=false;
		if(list_prop.size()>0){
			String s = list_prop[0];
			//vecteur de chaine pour stocker chaque element de la chaine de commande
			vector<string> mots;
			std::string delimiter = " ";
			int pos = 0;
			//variable intermédiaire
			std::string token;
			while ((pos = s.find(delimiter)) != std::string::npos) {
				token = s.substr(0, pos);
				mots.push_back(token);
				s.erase(0, pos + delimiter.length());
			}
			mots.push_back(s);
			//string qui sera par la suite en param à la fonction system()
			String command;
			//commande simple
			if(mots.size()==1){
				if(mots[0].compare("shutdown")==0)
					return 0;
				for (int j = 0; j < list_cmd.size() && !found_cmd; ++j) {
					String curr_cmd=list_cmd[j].substr(0,list_cmd[j].find("="));
					//std::cout <<curr_cmd<< std::endl;
					if(mots[0].compare(curr_cmd)==0){
						found_cmd=true;
						command =list_cmd[j].substr(list_prop[0].size()+1);
					}
				}

			}
			//commande avec arguments
			else {
				for (int j = 0; j < list_cmd.size() && !found_cmd; ++j) {
					if(mots[0].compare(list_cmd[j].substr(0,mots[0].size()))==0){
						found_cmd=true;
						//std::cout <<"found "<< std::endl;
						command = list_cmd[j].substr(list_cmd[j].find("=")+1);
						command += mots[1];
						for (int k = 2; k < mots.size() ; ++k) {
							command+="+"+mots[k];
						}
						command += " &";///pour lancer plusieurs commandes
					}
				}
			}
			system(command.c_str());
			if(!found_cmd){
				std::cout << "******************** You must Repeat ********************" << std::endl;
				system("play ../sound_effects/cmd_not_found.wav");
			}
		}
		std::cout << "****************** Back to main program *****************" << std::endl;

	}
    return 0;
}


/*
	@Param :
	* Mat : la frame principale qui sera affichée par la suite
	* img_width, img_height : nécessaire pour le Redimensionnement de l'image pour la reconnaissance des visages
	@Do:
	* Detect faces, eyes, pupils and gaze.

*/
void detect( Mat frame){
    //detection des visages
	Mat frame_gray;
	cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    std::vector<cv::Rect> faces;
	/*detectMultiScale :
		@Param :
		const Mat& image: Matrix of the type CV_8U containing an image where objects are detected.
		vector<Rect>& objects: Vector of rectangles where each rectangle contains the detected object.
		double scaleFactor=1.1: Parameter specifying how much the image size is reduced at each image scale.
		int minNeighbors=3: Parameter specifying how many neighbors each candidate rectangle should have to retain it.
		Size minSize=Size(): Minimum possible object size. Objects smaller than that are ignored.
		Size maxSize=Size(): Maximum possible object size. Objects larger than that are ignored.
	*/
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
    for( int i = 0; i < faces.size(); i++ ){
        Face face;
        if(face.detect(frame,frame_gray,faces[i],model,face_recognition)){
            vector<Rect> sides=face.setSides(faces[i]);
			/*Variable pour la detection du regard en IR*/
			int r_ratio=0;
			int l_ratio = 0;
			/*Variable pour la detection du regard en webcam simple*/
			/*detection du regard à droite avec les deux yeux*/
			bool look_right_r=false;
			bool look_right_l=false;
			/*detection du regard à gauche avec les deux yeux*/
			bool look_left_r=false;
			bool look_left_l=false;
			/*detection du regard tout droit avec les deux yeux*/
			bool look_forward_r=false;
			bool look_forward_l=false;
			/*parcours de chaque côté du visage*/
            for( int j = 0; j < sides.size(); j++ ){
				vector<Rect> eyes;
                Mat side_face_ROI = Mat(frame,sides[j]);
				/*j=0 => right eye*/
                if (j==0)
				/*detectMultiScale :
					@Param :
					const Mat& image: Matrix of the type CV_8U containing an image where objects are detected.
					vector<Rect>& objects: Vector of rectangles where each rectangle contains the detected object.
					double scaleFactor=1.1: Parameter specifying how much the image size is reduced at each image scale.
					int minNeighbors=3: Parameter specifying how many neighbors each candidate rectangle should have to retain it.
					Size minSize=Size(): Minimum possible object size. Objects smaller than that are ignored.
					Size maxSize=Size(): Maximum possible object size. Objects larger than that are ignored.
				*/
                    right_eye_cascade.detectMultiScale( side_face_ROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
				/*j=1 => left eye*/
                else
                    left_eye_cascade.detectMultiScale( side_face_ROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
				/*Enlever les faux positifs de l'oeil*/
                if(eyes.size()==1){
                    for( int k = 0; k < eyes.size(); k++ ){
                        Eye eye;
						/*Crop unz zone précise de l'oeil pour améliorer le traitement*/
                        Rect eye_ROI=eye.setEyeZone(sides[j],eyes[k]);
                        Mat eye_zone =Mat(frame_gray,eye_ROI);
                        Filtre filtre;
						Pupil pupil;
						/*Detection infrarouge*/
						if (camera_type == 0){
							/*fonction de Filtrage*/
                    		eye_zone=filtre.IR_method(eye_zone);
							/*on restreint la zone a traiter à l'iris*/
							Mat eye_zone_cropped= pupil.cropIrisZone(eye_zone);
							eye.show(j,eye_zone);
							/*Calcul du ratio pixel blancs/noirs*/
							/*j=0 => right eye*/
							if (j==0)
								r_ratio = pupil.getRatio(eye_zone_cropped);
							/*j=1 => left eye*/
							else
								l_ratio = pupil.getRatio(eye_zone_cropped);

						} else {
							/*fonction de Filtrage*/
		                    eye_zone=filtre.croissance_method(eye_zone);
							/*Donne le resultat apres filtrage*/
							//eye.show(j,eye_zone);
							/*Fonction de detection des pupilles*/
		                    pupil.croissance_detect(eye_zone);
		                    Point center=pupil.draw(frame,Point(eye_ROI.x,eye_ROI.y));
		                    //Position approximative des Lcorner et Rcorner
		                    Point Rcorner=Point(cvRound(eye_ROI.x+eye_ROI.width*0.1),cvRound(eye_ROI.y+eye_ROI.height/2));
		                    Point Lcorner=Point(cvRound(eye_ROI.x+eye_ROI.width),cvRound(eye_ROI.y+eye_ROI.height/2)) ;
		                    circle( frame,Rcorner , 3, Scalar(0,255,0), -1, 8, 0 );
		                    circle( frame,Lcorner, 3, Scalar(0,255,0), -1, 8, 0 );
							//Determiner la direction du regard
		                    if(abs(center.x-Rcorner.x)<=eye_ROI.width*0.35 && abs(center.x-Lcorner.x)>=eye_ROI.width*0.58){
								/*j=0 => right eye*/
								if(j==0)
									look_right_r=true;
								/*j=1 => left eye*/
								else
									look_right_l=true;
							}else if(abs(center.x-Lcorner.x)<=eye_ROI.width*0.35 && abs(center.x-Rcorner.x)>=eye_ROI.width*0.58){
								/*j=0 => right eye*/
								if(j==0)
									look_left_r=true;
								/*j=1 => left eye*/
								else
									look_left_l=true;

							}else {
								if(j==0)
									look_forward_r=true;
								else
									look_forward_l=true;
							}
						}
                    }
                }
            }
			if(camera_type == 1){
				//Detection du regard vers la gauche grace aux booleans (look_left(right_eye && left_eye))
				if(look_left_l && look_left_r){
					cpt=0;
					//std::cout << "l'utilisateur regarde à gauche"<<endl;
				}
				//Detection du regard vers la droite grace aux booleans (look_right(right_eye && left_eye))
				else if(look_right_l && look_right_r){
					cpt=0;
					//std::cout << "l'utilisateur regarde à droite"<<endl;
				}
				//Detection du regard en direction de l'objet grace aux booleans (look_forward(right_eye && left_eye))
				else if(look_forward_l && look_forward_r){
					//std::cout << "l'utilisateur me regarde"<<endl;
					cpt++;
					//cout << cpt << endl;
					if (cpt==15){
						cpt=0;
						watched=true;
					}
				}
			}
			//Detection du regard en IR et lancement de la reconnaissance vocale
			//si 2 frames de suite ont detecté un regard
			if ( camera_type == 0 && (r_ratio+l_ratio)/2.0 >1.1 ){
				frame_watched++;

			} else{
				frame_watched=0;
			}
			if ( frame_watched == 2 ){
				watched=true;
				//cout << "regard" << endl;
				frame_watched=0;
			}
        }
    }
}
