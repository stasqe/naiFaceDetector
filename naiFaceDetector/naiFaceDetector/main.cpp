#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

// funkcja main
int main(void)
{
	VideoCapture capture(0);

	if (!capture.isOpened())  // sprawdzam czy przechwytywanie zakonczone sukcesem
		return -1;

	// odczyt strumienia
	Mat frame;

	for (;;)
	{
		capture >> frame;

		//obsluga przerwania
		int c = waitKey(10);

		if (27 == char(c)) 
		{
			break;
		}
	}

	return 0;
}