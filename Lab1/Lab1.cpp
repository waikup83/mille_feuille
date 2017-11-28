/*
	David Desbiens
	Geremy Desmanche
	17 octobre 2017
*/

#include "stdafx.h"
#include <windows.h>
#include <vector>

//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;



/* Portion vérifiant le dessus et les proportions,
mise dans une fonction car programmée séparément. */
bool verifierDessusEtProportions(Mat Image, String* resultat) {
	Mat ImageAttente; // Image utilise pour les couches.
	std::vector<std::vector<Point>> Contours;

	//Valeur par défaut
	int minX = Image.cols, maxX = 0, minY = Image.rows, maxY = 0;
	int Compte = 0;
	String Liste[5] = { "1.jpg", "2.jpg", "3.jpg", "4.jpg", "5.jpg" };
	String Resultat;


	//*************** VRIFIER RATIO *************
	// Préparation de l'image.
	threshold(Image, Image, 171, 255, CV_THRESH_BINARY_INV);

	ImageAttente = Image.clone();
	resize(Image, Image, Size(1000, 600));
	Image = Image(Rect(0, 50, Image.cols, Image.rows - 70)).clone();

	//Rduire les formes
	erode(Image, Image, Mat::ones(Size(7, 7), CV_8UC1));

	//Trouver contours
	findContours(Image, Contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	//Vrifier chaque contour trouve
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

	//Rsultat
	if ((float)(maxX - minX) / (maxY - minY) < 2.3) {
		*resultat = "Il n'y a qu'un fragment de mille-feuille.";
		return true;
	}

	//*************** VÉRIFIER DESSUS *************
	//Préparation de l'image

	resize(ImageAttente, Image, Size(200, 1000));
	Image = Image(Rect(0, 50, Image.cols, Image.rows / 2 - 70));

	//Réduire les formes
	dilate(Image, Image, Mat::ones(Size(3, 3), CV_8UC1));

	//Trouver contours
	findContours(Image, Contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < Contours.size(); i++) {
		approxPolyDP(Contours[i], Contours[i], 20, FALSE);

		if (Contours[i].size() > 2 && Contours[i].size() < 6) {
			minX = Image.cols;
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

	if (Compte <= 2 && Resultat == "") {
		*resultat = "Le dessus du mille-feuille est invalide.";
		return true;
	}
	return false;
}

int main()
{
	Mat Image, // Image utilisée pour les couches.
		ImageCouches; // Image utilisée pour les couches. 
	String Liste[5] = { "1.jpg", "2.jpg", "3.jpg", "4.jpg", "5.jpg" };
	String Resultat;


	unsigned int val; // val1;
	unsigned int j;
	unsigned int largeur, hauteur;

	Rect boiteContenante[5];
	std::vector<std::vector<Point>> Contours;


	for (int i = 0; i < 5; i++) {
		/* Read image */
		Image = imread(Liste[i], CV_LOAD_IMAGE_GRAYSCALE);

		/***********************************************************************************************************/
		/* Vérifications. Le traitement s'arrête dès que le mille feuille est rejetté (pas de traitement inutile). */

		if (!verifierDessusEtProportions(Image.clone(), &Resultat)) {
			val = Image.rows / 2;
			/* Sépare l'image entre le dessus et les couches. */
			threshold(Image(Rect(0, val, Image.cols, 2 * val / 3)), ImageCouches, 171, 1, CV_THRESH_BINARY_INV);
			resize(ImageCouches, ImageCouches, Size(ImageCouches.cols / 2, ImageCouches.rows / 2));

			/* Transforme le bruit (des biscuits) en grosses bandes blanches.
			Accomplie la chose en tranchant dans la morphologie de l'image comme un gros malade. */
			medianBlur(ImageCouches, ImageCouches, 21);
			morphologyEx(ImageCouches, ImageCouches, MORPH_CLOSE, Mat::ones(Size(71, 35), CV_8UC1));
			morphologyEx(ImageCouches, ImageCouches, MORPH_ERODE, Mat::ones(Size(111, 27), CV_8UC1));
			morphologyEx(ImageCouches, ImageCouches, MORPH_ERODE, Mat::ones(Size(241, 3), CV_8UC1));

			// Trouver contours, puis couper plus court pourt en garder un minimum pertinent. 
			findContours(ImageCouches, Contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
			for (j = 0; j < Contours.size(); j++) {
				approxPolyDP(Contours[j], Contours[j], 24, FALSE);
				/* Ne reste qu'à savoir ce qui est plus rapide entre TRUE et FALSE*/
				boiteContenante[j] = boundingRect(Contours[j]);
			}

			/* Vérification des proportions. Finalement laissée à la portion suppérieure. */
			//  largeur = hauteur = 0;
			//  /* Sommation de l'ensemble des largeurs et des hauteurs des biscuits du mille feuilles. */
			//  for (val = 0; val < j; val += 1) {
			//  	largeur += boiteContenante[val].width;
			//  	hauteur += boiteContenante[val].height;
			//  }
			//  
			//  /* Si le mille feuille n'est pas plus de deux fois plus long que haut, ses proportions sont mauvaises : fragment. */
			//  /* if (3 * hauteur > largeur)
			//  	Resultat = "Il n'y a là qu'un fragment de mille feuilles !";
			//  
			//  
			/* Vérification du nombre de couche. */
			if (j < 3) {

				/* Met les rectangles en ordre de hauteur. */
				if (boiteContenante[0].y > boiteContenante[1].y) {
					boiteContenante[4] = boiteContenante[0];
					boiteContenante[0] = boiteContenante[1];
					boiteContenante[1] = boiteContenante[4];
				}

				/* S'il manque une couche, déterminer laquelle. */
				if (j == 2) {
					/* Remplacé par la portion supérieure qui fonctionnait mieux.
					if (debutmajore[0] > ImageCouches.rows / 5)
					Resultat = "La couche supérieure est manquante !\r\n";
					else*/ if (boiteContenante[0].height > boiteContenante[1].height * 1.4f)
						Resultat = "Il manque la couche de creme !";
					else
						Resultat = "Il manque la couche de costarde !";
				}
				else {
					Resultat = "Il manque... tout ? ><";
				}
			}
			else {
				Resultat = "Le mille-feuille est parfait !";
			}
		}

		putText(Image, Resultat, Point(240, 320), cv::HersheyFonts(), 5, Scalar(0, 0, 255), 16);
		resize(Image, Image, Size(840, 620));
		imshow("ImageCouches", Image);
		ImageCouches.release();
		Image.release();
		waitKey(0);
	}

	return 0;
}