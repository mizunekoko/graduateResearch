#include <stdio.h>
#include <iostream>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef struct{
    int r,g,b;
}color_t;


int main(int arg, char* argv[]){

    char movie[256];
    char image[256];

    cv::Mat frame, template_image, matching_res;
    color_t col;

    if(arg == 3){
        strcpy(movie, argv[1]);  //movie copy
        strcpy(image, argv[2]); //image copy
    }else{
        std::cout << "[movie file] [image]" << std::endl;
        return -1;
    }

    // image
    template_image = cv::imread(image, -1);
    if(!template_image.data){  //not image
        std::cout << "Not Image : "<< image <<std::endl;
        return -1;
    }

    //open movie
    cv::VideoCapture video(movie);
    if(!video.isOpened()){  // not open movie
        std::cout << "Cannot find movie file : " << movie  << std::endl;
        return -1;
    }

    //make movie
    cv::VideoWriter video_out("output.avi", CV_FOURCC('X', 'V', 'I', 'D'), 15, cv::Size(frame.cols, frame.rows));

    if(!video_out.isOpened()){
        std::cout << "Cannot open output file " << std::endl;
        return -22;
    }

    //window name 
    cv::namedWindow("movie");
    cv::namedWindow("template");

    while(1){

        cv::matchTemplate(frame, template_image, matching_res, CV_TM_CCOEFF_NORMED);

        cv::Point max_pt;
        double maxVal;

        col.b = 200;    col.g = 0;     col.r = 0;
        cv::rectangle(frame,
                cv::Point((max_pt.x),                         (max_pt.y)), 
                cv::Point((max_pt.x)+template_image.cols,     (max_pt.y)+template_image.rows), 
                cv::Scalar(col.b, col.g, col.r), 2, CV_AA);

        imshow("preview", frame);
        imshow("template", template_image);

        video_out << frame;
    }

}