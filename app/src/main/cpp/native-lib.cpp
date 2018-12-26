#include <jni.h>
#include <string>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

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
}
}