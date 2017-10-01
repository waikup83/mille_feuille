// Lab1.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>


//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;


int main()
{
	Mat Image; // Image utilisée pour les couches.
	Mat ImageFinition, ImageVerifiee; // Images pour la détection de grandeur + vérifier le dessus
	std::vector<std::vector<Point>> Contours;
	int minX, maxX, minY, maxY;
	int Compte;
	String Liste[5] = { "1.jpg", "2.jpg", "3.jpg", "4.jpg", "5.jpg" };
	String Resultat;

	for (int i = 0; i < 5; i++) {
		Resultat = "";
		Compte = 0;
		Image = imread(Liste[i], CV_LOAD_IMAGE_COLOR);



		//*************** VÉRIFIER RATIO ***************
		//Préparation de l'image
		ImageFinition = Image.clone();
		cvtColor(ImageFinition, ImageFinition, CV_BGR2GRAY);
		threshold(ImageFinition, ImageFinition, 171, 255, CV_THRESH_BINARY_INV);

		resize(ImageFinition, ImageFinition, Size(1000, 600));
		ImageFinition = ImageFinition(Rect(0, 50, ImageFinition.cols, ImageFinition.rows - 70)).clone();

		//Réduire les formes
		erode(ImageFinition, ImageFinition, Mat::ones(Size(7, 7), CV_8UC1));

		//Trouver contours
		findContours(ImageFinition, Contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

		//Valeur par défaut
		minX = ImageFinition.cols;
		maxX = 0;
		minY = ImageFinition.rows;
		maxY = 0;

		//Vérifier chaque contour trouvée
		for (int c = 0; c < Contours.size(); c++) {
			approxPolyDP(Contours[c], Contours[c], 20, FALSE);

			if (Contours[c].size() > 2) {
				//Trouver max et min de la forme
				for (int p = 0; p < Contours[c].size(); p++) {
					if (Contours[c][p].x < minX)
						minX = Contours[c][p].x;
					if (Contours[c][p].x > maxX)
						maxX = Contours[c][p].x;
					if (Contours[c][p].y < minY)
						minY = Contours[c][p].y;
					if (Contours[c][p].y > maxY)
						maxY = Contours[c][p].y;
				}
			}
		}

		//Résultat
		if ((float)(maxX - minX) / (maxY - minY) < 2.3)
			Resultat = "Trop petit";



		//*************** VÉRIFIER DESSUS ***************
		//Préparation de l'image
		ImageFinition = Image.clone();
		cvtColor(ImageFinition, ImageFinition, CV_BGR2GRAY);
		threshold(ImageFinition, ImageFinition, 171, 255, CV_THRESH_BINARY_INV);

		resize(ImageFinition, ImageFinition, Size(200, 1000));
		ImageFinition = ImageFinition(Rect(0, 50, ImageFinition.cols, ImageFinition.rows / 2 - 70)).clone();

		//Réduire les formes
		dilate(ImageFinition, ImageFinition, Mat::ones(Size(3, 3), CV_8UC1));

		//Trouver contours
		ImageVerifiee = Mat::zeros(ImageFinition.rows, ImageFinition.cols, CV_8UC3);
		findContours(ImageFinition, Contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
		for (int i = 0; i < Contours.size(); i++) {
			approxPolyDP(Contours[i], Contours[i], 20, FALSE);

			if (Contours[i].size() > 2 && Contours[i].size() < 6) {
				minX = ImageFinition.cols;
				maxX = 0;
				//Trouver max et min en X de la forme
				for (int p = 0; p < Contours[i].size(); p++) {
					if (Contours[i][p].x < minX)
						minX = Contours[i][p].x;
					if (Contours[i][p].x > maxX)
						maxX = Contours[i][p].x;
				}

				if (maxX - minX > 20)
					Compte++;
			}
		}

		if (Compte <= 2 && Resultat == "")
			Resultat = "Dessus invalide";
		//*****************************************************









		putText(Image, Resultat, Point(240, 320), cv::HersheyFonts(), 8, Scalar(0, 0, 255), 16);
		resize(Image, Image, Size(800, 600)); imshow("Image", Image);


		waitKey(0);
	}
	return 0;
}


