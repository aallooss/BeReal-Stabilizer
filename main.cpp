#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <cmath>

#define PI 3.14


int main()
{
    char * filename = new char[100];
    char * outputFilename = new char[100];
    cv::Mat frame = cv::imread("/home/hadensangree/FaceTranslation/tempinput/bereal0.jpeg");
    int j=0;
    while(frame.data)
    {
        sprintf(filename,"/home/hadensangree/FaceTranslation/tempinput/bereal%i.jpeg",j);
        frame = cv::imread(filename);

        if(!frame.data )
        {
            std::cout << "No more images" << std::endl;
            // no more images
            break;
        }

    double scale = 3.0;

	cv::CascadeClassifier faceCascade;
	faceCascade.load("/home/hadensangree/FaceTranslation/haarcascades/haarcascade_frontalface_default.xml");
	cv::CascadeClassifier eyeCascade;
	eyeCascade.load("/home/hadensangree/FaceTranslation/haarcascades/haarcascade_eye.xml");

    cv::Mat grayscale;
    cvtColor(frame,grayscale,cv::COLOR_BGR2GRAY);
    cv::resize(grayscale, grayscale, cv::Size(grayscale.size().width / scale, grayscale.size().height / scale));

    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(grayscale, faces,1.05,6,0, cv::Size(30,30));

    int index =0;
    int left_eye_x;
    int left_eye_y;
    int right_eye_x;
    int right_eye_y;
    int direction = -1;
    cv::Point left_center, right_center, right_angle;
    int left_bruh[2];
        for (cv::Rect area : faces)
		{
            cv::Scalar drawColor = cv::Scalar(255, 0, 0);
            rectangle(frame, cv::Point(cvRound(area.x * scale), cvRound(area.y * scale)),
            cv::Point(cvRound((area.x + area.width - 1) * scale), cvRound((area.y + area.height - 1) * scale)), drawColor, 2);
            cv::circle(frame, cv::Point(cvRound(area.x * scale), cvRound(area.y * scale)), 5, (255, 0, 0) , -1);

            std::vector<cv::Rect> eye;
            eyeCascade.detectMultiScale(grayscale, eye, 1.05, 6, 0, cv::Size(35,35));
            for (cv::Rect area : eye)
            {
                cv::Scalar drawColor = cv::Scalar(0, 255, 0);
                if (index ==0 )
                {
                    left_eye_x =  (area.x * scale)+(area.width* scale)/2;
                    left_eye_y = (area.y * scale)+(area.height * scale)/2;
                    cv::Point left_center(left_eye_x, left_eye_y);
                }
                else {
                    right_eye_x =  (area.x * scale)+(area.width* scale)/2;
                    right_eye_y = (area.y * scale)+(area.height * scale)/2;
                    cv::Point right_center(right_eye_x, right_eye_y);
                }
                rectangle(frame, cv::Point(cvRound(area.x * scale), cvRound(area.y * scale)),
                cv::Point(cvRound((area.x + area.width - 1) * scale), cvRound((area.y + area.height - 1) * scale)), drawColor, 2);
                cv::circle(frame, cv::Point(cvRound(area.x * scale), cvRound(area.y * scale)), 5, (255, 0, 0) , -1);
                index++;
            }                
            cv::Point left_center(left_eye_x, left_eye_y);
            cv::Point right_center(right_eye_x, right_eye_y);
            if (left_center.y > right_center.y)
            {

                cv::Point right_angle(right_center.x, left_center.y);
                direction = 1;
                cv::circle(frame, right_angle, 5, (255, 0, 0) , -1);
                cv::line(frame,right_center, left_center,(0,200,200),3);
                cv::line(frame,left_center, right_angle,(0,200,200),3);
                cv::line(frame,right_center, right_angle,(0,200,200),3);
            }
            else
            {
                cv::Point right_angle(left_center.x, right_center.y);
                direction = -1;
                cv::circle(frame, right_angle, 5, (255, 0, 0) , -1);
                cv::line(frame,right_center, left_center,(0,200,200),3);
                cv::line(frame,left_center, right_angle,(0,200,200),3);
                cv::line(frame,right_center, right_angle,(0,200,200),3);
            }

            double delta_x = right_eye_x - left_eye_x;
            double delta_y = right_eye_y - left_eye_y;
            double delta_x_1;
            double delta_y_1;
            double result = atan2 (delta_y,delta_x) * 180 / PI;

            int w = frame.cols;
            int h = frame.rows;
            cv::Point center(w / 2, h / 2);
            cv::Mat M = cv::getRotationMatrix2D(center, result, 1.0);
            cv::Mat dstMat;
            cv::warpAffine(frame, dstMat, M, frame.size());
            double dist_1 = sqrt((delta_x * delta_x) + (delta_y * delta_y));
            double dist_2 = sqrt((delta_x_1 * delta_x_1) + (delta_y_1 * delta_y_1));
            double ratio = dist_1/dist_2;

            h=1080;
            w=1920;
            cv::Mat resized;
            cv::resize(dstMat, resized, cv::Size(int(w*ratio), int(h*ratio)), 1, 1);
            sprintf(outputFilename,"/home/hadensangree/FaceTranslation/tempoutput/output%i.jpeg",j);
            cv::imwrite(outputFilename, resized);
        }
    j++;
    }
    return 0;
}