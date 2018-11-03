#include <stdio.h>
#include <iostream>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define TextSize 1.5
#define TextThickness 2

//std::string win1 = "preview";
//std::string win_color = "rgb";

typedef struct{
    int r,g,b;
}color_t;



int main(int argc, char* argv[]){
	int i;
	char fname_mov[256];
	char fname_temp[256];
	//char fname_out_ext[] = "bmp";
	cv::Mat frame, frame_src, frame_template, frame_template_src, matching_res;
	color_t col;

	double scale = 1;


	if(argc == 3){
//Usage: puled [movie file] [output file] 
		strcpy(fname_mov, argv[1]);
		strcpy(fname_temp, argv[2]);
	}else {
		std::cout << "Usage: puled [movie file] [template file] " << std::endl;
		return -1;
	}

//Open template
    frame_template_src =  cv::imread(fname_temp, -1);
	if( !frame_template_src.data ){
		std::cout << "Cannot find template file : " << fname_temp << std::endl;
		return -1;
	}
	cv::resize(frame_template_src, frame_template, cv::Size(), scale / 4.5, scale / 4.5);

//open Movie
	cv::VideoCapture video(fname_mov);
	if( !video.isOpened() ){
		std::cout << "Cannot find Movie file : " << fname_mov << std::endl;
		return -1;
	}

//to obtain frame size
	video >> frame_src;
	cv::resize( frame_src, frame, cv::Size(), scale, scale);
//rewind
	video.set(CV_CAP_PROP_POS_MSEC, 0 );

//output file
	cv::VideoWriter video_out("output.avi", CV_FOURCC('X','V','I','D'), 15, cv::Size(frame.cols, frame.rows));
//	cv::VideoWriter video_out("output.avi", CV_FOURCC('X','V','I','D'), 15, cv::Size(640, 480));
	if( !video_out.isOpened()){
		std::cout << "Cannot open output file" << std::endl;
		return -22;
	}

	cv::namedWindow("preview");
	cv::namedWindow("template");

	i=0; 
	while(1){			
		
		video >> frame_src;


		if(frame_src.empty() || cv::waitKey(30) >= 0 || video.get(CV_CAP_PROP_POS_AVI_RATIO) == 1){
			std::cout << i << "frames " << std::endl;
			break;
		}				

		cv::resize( frame_src, frame, cv::Size(), scale, scale);

		//template matching
		cv::matchTemplate(frame, frame_template, matching_res, CV_TM_CCOEFF_NORMED);

		cv::Point max_pt;
		double maxVal;
		cv::minMaxLoc( matching_res, NULL, &maxVal, NULL, &max_pt);


		col.b = 200;	col.g = 0; 	col.r = 0;

		std::cout << "pos= " << max_pt.x << ", "<<max_pt.y << std::endl;
		
		cv::rectangle(frame, 
				cv::Point((max_pt.x), 						(max_pt.y)), 
				cv::Point((max_pt.x)+frame_template.cols, 	(max_pt.y)+frame_template.rows), 
				cv::Scalar(col.b, col.g, col.r), 2, CV_AA);
			//cv::rectangle(frame, cv::Point(pos_x, pos_y), cv::Point(pos_x+20, pos_y-20), cv::Scalar(col.b, col.g, col.r), 1, CV_AA);


//		cv::rectangle(frame, cv::Point(pos_x, pos_y), cv::Point(pos_x+20, pos_y-20), cv::Scalar(col.b, col.g, col.r), 1, CV_AA);
		imshow("preview", frame);
		imshow("template", frame_template);
			

		video_out << frame;

		i++;

	}
}