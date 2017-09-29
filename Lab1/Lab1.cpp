// Lab1.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <windows.h>


//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;


int main()
{
	Mat Image, // Image utilisée pour les couches.
		ImageDessus; // Image utilisée pour le dessus.
	String Liste[5] = { "1.jpg", "2.jpg", "3.jpg", "4.jpg", "5.jpg" };


	for (int i = 0; i < 5; i++) {
		Image = imread(Liste[i], CV_LOAD_IMAGE_GRAYSCALE);
		threshold(Image, Image, 171, 255, CV_THRESH_BINARY_INV);



		
		//************************ Vérifier dessus *****************************
		std::vector<std::vector<Point>> Contours;
		std::vector<Vec3f> Cercle;
		Mat ImageFinition, ImageVerifiee;

		ImageFinition = Image.clone();
		resize(ImageFinition, ImageFinition, Size(800, 600));
		ImageFinition = ImageFinition(Rect(0, 0, ImageFinition.cols, ImageFinition.rows / 2)).clone();


		ImageVerifiee = Mat::zeros(ImageFinition.rows, ImageFinition.cols, CV_8UC3);

		//Trouver contours
		findContours(ImageFinition, Contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

		for (int i = 0; i < Contours.size(); i++)
			drawContours(ImageVerifiee, Contours, i, Scalar(255, 0, 0), 3);
		
		imshow("Image dessus", ImageVerifiee);

		//*********************************************************************
		







		/*
		medianBlur(Image, Image, 31);
		morphologyEx(Image, ImageDessus, MORPH_CLOSE, Mat::ones(Size(9, 9), CV_8UC1));
		medianBlur(ImageDessus, Image, 31);
		*/


		//putText(Image, "Verifier", Point(240, 320), cv::HersheyFonts(), 8, Scalar(255, 0, 255), 16);
		resize(Image, Image, Size(800, 600)); imshow("Image", Image);
		



		waitKey(0);
	}
	return 0;
}


