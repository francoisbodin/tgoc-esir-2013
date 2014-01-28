#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include "apirequester.h"
#include "filter5x5.h"

using namespace std;

#define INDXr(i,j)  (step * i + j*3 + 2)  
#define INDXg(i,j)  (step * i + j*3 + 1)  
#define INDXb(i,j)  (step * i + j*3 + 0)  
#define INDX(i,j)   (step * i + j + 0)  


void processImageColor(cv::Mat *src, cv::Mat *dest, const int filter[5][5]){
  if (!src || !dest) return;
  unsigned char *input  = (unsigned char*)(src->data);
  unsigned char *output = (unsigned char*)(dest->data);
  const size_t stencil = 2;
  int step = src->step;
  const int pond = 
      filter[0][0] + filter[0][1] + filter[0][2] + filter[0][3] + filter[0][4]
    + filter[1][0] + filter[1][1] + filter[1][2] + filter[1][3] + filter[1][4]
    + filter[2][0] + filter[2][1] + filter[2][2] + filter[2][3] + filter[2][4]
    + filter[3][0] + filter[3][1] + filter[3][2] + filter[3][3] + filter[3][4]
    + filter[4][0] + filter[4][1] + filter[4][2] + filter[4][3] + filter[4][4];



  std::cout << "cols: " << src->cols << std::endl;
  std::cout << "rows: " << src->rows << std::endl;  
  std::cout << "step: " <<step << std::endl;  
  // image couleur
  for(int i = stencil;i < src->rows-stencil;i++){
      for(int j = stencil;j < src->cols-stencil;j++){
	//output[INDXb(i,j)] = 0;
	output[INDXg(i,j)] = 0;
	output[INDXr(i,j)] = 0;
	float v;
	v = filter[0][0] * input[INDXb(i-2,j-2)] + filter[0][1] * input[INDXb(i-2,j-1)] 
	  + filter[0][2] * input[INDXb(i-2,j)] + filter[0][3] * input[INDXb(i-2,j+1)] 
	  + filter[0][4] * input[INDXb(i-2,j+2)]

	  + filter[1][0] * input[INDXb(i-1,j-2)] + filter[1][1] * input[INDXb(i-1,j-1)] 
	  + filter[1][2] * input[INDXb(i-1,j)] + filter[1][3] * input[INDXb(i-1,j+1)] 
	  + filter[1][4] * input[INDXb(i-1,j+2)]

	  + filter[2][0] * input[INDXb(i  ,j-2)] + filter[2][1] * input[INDXb(i  ,j-1)] 
	  + filter[2][2] * input[INDXb(i  ,j)] + filter[2][3] * input[INDXb(i  ,j+1)] 
	  + filter[2][4] * input[INDXb(i  ,j+2)]
	  
	  + filter[3][0] * input[INDXb(i+1,j-2)] + filter[3][1] * input[INDXb(i+1,j-1)] 
	  + filter[3][2] * input[INDXb(i+1,j)] + filter[3][3] * input[INDXb(i+1,j+1)] 
	  + filter[3][4] * input[INDXb(i+1,j+2)]

	  + filter[4][0] * input[INDXb(i+2,j-2)] + filter[4][1] * input[INDXb(i+2,j-1)] 
	  + filter[4][2] * input[INDXb(i+2,j)] + filter[4][3] * input[INDXb(i+2,j+1)] 
	  + filter[4][4] * input[INDXb(i+2,j+2)];
	v = v / pond;
	//v = input[INDXb(i,j)] / pond;
	output[INDXb(i,j)] = v;
      }
  }
}

void processImageGray(cv::Mat *src, cv::Mat *dest, const int filter[5][5]){
  if (!src || !dest) return;
  unsigned char *input  = (unsigned char*)(src->data);
  unsigned char *output = (unsigned char*)(dest->data);
  const size_t stencil = 2;
  int step = src->step;
  const int pond = 
      filter[0][0] + filter[0][1] + filter[0][2] + filter[0][3] + filter[0][4]
    + filter[1][0] + filter[1][1] + filter[1][2] + filter[1][3] + filter[1][4]
    + filter[2][0] + filter[2][1] + filter[2][2] + filter[2][3] + filter[2][4]
    + filter[3][0] + filter[3][1] + filter[3][2] + filter[3][3] + filter[3][4]
    + filter[4][0] + filter[4][1] + filter[4][2] + filter[4][3] + filter[4][4];

  std::cout << "cols: " << src->cols << std::endl;
  std::cout << "rows: " << src->rows << std::endl;  
  std::cout << "step: " <<step << std::endl;  
  // image grise
  for(int i = stencil;i < src->rows-stencil;i++){
      for(int j = stencil;j < src->cols-stencil;j++){
	float v;
	v = filter[0][0] * input[INDX(i-2,j-2)] + filter[0][1] * input[INDX(i-2,j-1)] 
	  + filter[0][2] * input[INDX(i-2,j)] + filter[0][3] * input[INDX(i-2,j+1)] 
	  + filter[0][4] * input[INDX(i-2,j+2)]

	  + filter[1][0] * input[INDX(i-1,j-2)] + filter[1][1] * input[INDX(i-1,j-1)] 
	  + filter[1][2] * input[INDX(i-1,j)] + filter[1][3] * input[INDX(i-1,j+1)] 
	  + filter[1][4] * input[INDX(i-1,j+2)]

	  + filter[2][0] * input[INDX(i  ,j-2)] + filter[2][1] * input[INDX(i  ,j-1)] 
	  + filter[2][2] * input[INDX(i  ,j)] + filter[2][3] * input[INDX(i  ,j+1)] 
	  + filter[2][4] * input[INDX(i  ,j+2)]
	  
	  + filter[3][0] * input[INDX(i+1,j-2)] + filter[3][1] * input[INDX(i+1,j-1)] 
	  + filter[3][2] * input[INDX(i+1,j)] + filter[3][3] * input[INDX(i+1,j+1)] 
	  + filter[3][4] * input[INDX(i+1,j+2)]

	  + filter[4][0] * input[INDX(i+2,j-2)] + filter[4][1] * input[INDX(i+2,j-1)] 
	  + filter[4][2] * input[INDX(i+2,j)] + filter[4][3] * input[INDX(i+2,j+1)] 
	  + filter[4][4] * input[INDX(i+2,j+2)];
	v = v / pond;
	output[INDX(i,j)] = v;
	//output[INDX(i,j)] = input[INDX(i,j)];
      }
  }
}

int main(int argc, char** argv){
	
	vector<Graffiti> *list = new std::vector<Graffiti>();

	ApiRequester::getAllGraffitis(list,4); //FBO 4 is an optional parameter... default number is 10000

	//Here list contains all the graffiti present on the database
	for(vector<Graffiti>::iterator it = list->begin(); it != list->end(); it++){
		Graffiti g = *it;
		ApiRequester::saveImageFromServer(&g,"/tmp/discovart");
		cv::Mat image;
		cv::Mat processedimage;
		cv::Mat image_gray;
		cv::Mat processedimage_gray;
		std::cout << "geting image: " << g.getLocalUrl() << std::endl;
		image = cv::imread(g.getLocalUrl(), 1 );
		processedimage = cv::imread(g.getLocalUrl(), 1 );
		cvtColor( image, image_gray, CV_BGR2GRAY );
		cvtColor( processedimage, processedimage_gray, CV_BGR2GRAY );

		//start
		processImageGray(&image_gray,&processedimage_gray,Stencil5Sharpen);
		cv::namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
		cv::imshow( "Display Image", processedimage_gray );
		cv::waitKey(0);		
		break; //FBO only want one here
	}

	return 0;
}
