#include <jni.h>
#include <string>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/face.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;
using namespace cv::face;;


extern "C"
{
void JNICALL Java_fyp2018_hkust_vorusingopencv_MainActivity_salt(JNIEnv *env, jobject instance,
                                                                 jlong matAddrGray) {
    Mat frame = *(Mat *) matAddrGray;
    CascadeClassifier face_cascade;
    std::vector<Rect> faces;
    Mat frame_gray;

    face_cascade.load("/storage/emulated/0/vor/lbpcascade_frontalface_improved.xml");

    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    //Faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 5);

    for (size_t i = 0; i < faces.size(); i++) {
        rectangle(frame, Point(faces[i].x + faces[i].width, faces[i].y),
                  Point(faces[i].x, faces[i].y + faces[i].height), Scalar(255, 0, 0), 2, 8, 0);

        Mat faceROI = frame_gray(faces[i]);
    }

    static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
        std::ifstream file(filename.c_str(), ifstream::in);
        if (!file) {
            string error_message = "No valid input file was given, please check the given filename.";
            CV_Error(CV_StsBadArg, error_message);
        }
        string line, path, classlabel;
        while (getline(file, line)) {
            stringstream liness(line);
            getline(liness, path, separator);
            getline(liness, classlabel);
            if(!path.empty() && !classlabel.empty()) {
                images.push_back(imread(path, 0));
                labels.push_back(atoi(classlabel.c_str()));
            }
        }
    }

    //Recognition

    // These vectors hold the images and corresponding labels:
    vector<Mat> images;
    vector<int> labels;

    // Get the height from the first image.
    int im_width = images[0].cols;
    int im_height = images[0].rows;

    // Create a FaceRecognizer and train it on the given images:
    Ptr<FaceRecognizer> model = FisherFaceRecognizer::create();
    model->train(images, labels);


    for(;;) {
        // Clone the current frame:
        Mat original = frame.clone();
        // Convert the current frame to grayscale:
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        // Find the faces in the frame:
        vector< Rect_<int> > recognised_faces;
        face_cascade.detectMultiScale(gray, recognised_faces);
        for(int i = 0; i < recognised_faces.size(); i++) {
            // Process face by face:
            Rect face_i = recognised_faces[i];
            // Crop the face from the image.
            Mat face = gray(face_i);

            Mat face_resized;
            cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
            // Perform prediction
            int prediction = model->predict(face_resized);
         
            rectangle(original, face_i, CV_RGB(0, 255,0), 1);
            // Create the text we will annotate the box with:
            string box_text = format("Prediction = %d", prediction);
            // Calculate the position for annotated text (make sure we don't
            // put illegal values in there):
            int pos_x = std::max(face_i.tl().x - 10, 0);
            int pos_y = std::max(face_i.tl().y - 10, 0);
            // And now put it into the image:
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
        }
        // Show the result:
        imshow("face_recognizer", original);
        // And display it:
        char key = (char) waitKey(20);
        // Exit this loop on escape:
        if(key == 27)
            break;
    }
}
}