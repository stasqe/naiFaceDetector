#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

// naglowek funkcji
void detectAndDisplay(Mat frame);

// zmienne globalne
string face_cascade_name = "c:/haarcascade_frontalface_alt.xml"; //biblioteka wykrywania twarzy
CascadeClassifier face_cascade;
string window_name = "Detekcja twarzy na obrazie z kamery";
int filenumber; // numerowanie plikow
string filename;

// funkcja main
int main(void)
{
	VideoCapture capture(0);

	if (!capture.isOpened())  // sprawdzam czy przechwytywanie zakonczone sukcesem
		return -1;

	// ladowanie kaskad z pliku
	if (!face_cascade.load(face_cascade_name))
	{
		printf("--(!)Blad ladowania kaskad\n");
		return (-1);
	};

	// odczyt strumienia
	Mat frame;

	for (;;)
	{
		capture >> frame;

		// dodanie klasyfikatora
		if (!frame.empty())
		{
			detectAndDisplay(frame);
		}
		else
		{
			printf("Blad przechwytywania");
			break;
		}

		int c = waitKey(10);

		if (27 == char(c))
		{
			break;
		}
	}

	return 0;
}

// funkcja detectAndDisplay
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;
	Mat crop;
	Mat res;
	Mat gray;
	string text;
	stringstream sstm;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	// detekcja twarzy
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	// definicja obszaru "poszukiwan"
	cv::Rect roi_b;
	cv::Rect roi_c;

	size_t ic = 0; // index biezacego elementu
	int ac = 0; //index obszaru

	size_t ib = 0; // index najwiekszego elementu
	int ab = 0; // obszar najwiekszego elementu

	for (ic = 0; ic < faces.size(); ic++) // iteracja wszystkich wykrytych twarzy

	{
		roi_c.x = faces[ic].x;
		roi_c.y = faces[ic].y;
		roi_c.width = (faces[ic].width);
		roi_c.height = (faces[ic].height);

		ac = roi_c.width * roi_c.height; // pobranie obszaru na ktorym wykryto twarz

		roi_b.x = faces[ib].x;
		roi_b.y = faces[ib].y;
		roi_b.width = (faces[ib].width);
		roi_b.height = (faces[ib].height);

		ab = roi_b.width * roi_b.height; // pobranie obszaru najwiekszego elementu

		if (ac > ab)
		{
			ib = ic;
			roi_b.x = faces[ib].x;
			roi_b.y = faces[ib].y;
			roi_b.width = (faces[ib].width);
			roi_b.height = (faces[ib].height);
		}

		crop = frame(roi_b);
		resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR); // przycinanie przechwyconego obrazu
		cvtColor(crop, gray, CV_BGR2GRAY); 

		// tworzenie nazwy pliku wyjsciowego
		filename = "";
		stringstream ssfn;
		ssfn << filenumber << ".png";
		filename = ssfn.str();
		filenumber++;

		imwrite(filename, gray);

		Point pt1(faces[ic].x, faces[ic].y); // oznaczanie wykrytych twarzy na streamie
		Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
		rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
	}

	// wyswietl obraz
	sstm << "Rozmiar przycietego obrazu: " << roi_b.width << "x" << roi_b.height << " Nazwa pliku: " << filename;
	text = sstm.str();

	putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
	imshow("Obraz z kamery", frame);

	if (!crop.empty())
	{
		imshow("Wykryta twarz", crop);
	}
	else
		destroyWindow("Wykryta twarz");
}