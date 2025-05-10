#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Đọc ảnh
    Mat img = imread("cat3.jpg");
    if (img.empty()) {
        cout << "Không thể đọc ảnh!" << endl;
        return -1;
    }
    imshow("Origin Image", img);

    // // Chuyển đổi ảnh sang không gian màu HSV
    // Mat imgHSV;
    // cvtColor(img, imgHSV, COLOR_BGR2HSV);

    // // Đặt phạm vi màu đen trong không gian HSV
    // Scalar lower_black(0, 0, 0);  // Màu đen thường có Hue 0, Saturation và Value gần 0
    // Scalar upper_black(180, 255, 50); // Hãy thử các giá trị này cho màu đen

    // // Tạo mặt nạ (mask) lọc màu đen
    // Mat maskBLACK;
    // inRange(imgHSV, lower_black, upper_black, maskBLACK);

    // // Đặt phạm vi màu xám trong không gian HSV
    // // Màu xám có Saturation gần 0, và Value từ 50 đến 200 để bao phủ nhiều sắc độ xám
    // Scalar lower_gray(0, 0, 50);    // H, S gần 0 và V trong phạm vi từ 50 (tối) đến 200 (sáng)
    // Scalar upper_gray(180, 70, 200); // H có thể dao động từ 0 đến 180, S gần 0 và V từ 50 đến 200    



    // Màu chủ đạo (BGR) mà bạn tính toán được từ trước
    //cv::Scalar dominantColorBGR(28, 29, 31);

    // Chuyển đổi ảnh sang không gian màu HSV
    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

    // Tạo mặt nạ (mask) lọc màu chủ đạo
    // Bạn cần xác định phạm vi màu chủ đạo trong không gian HSV
    // cv::Scalar lower_bound(28-20, 40, 40); // Hue - 10 để mở rộng phạm vi, Saturation và Value có thể điều chỉnh
    // cv::Scalar upper_bound(28+30, 255, 255); // Hue + 10 để mở rộng phạm vi
    // cv::Scalar lower_bound(0, 10+30, 20+20); // Hue - 10 để mở rộng phạm vi, Saturation và Value có thể điều chỉnh
    // cv::Scalar upper_bound(10+30, 50+200, 60+190); // Hue + 10 để mở rộng phạm vi
    float delta_hue = 37;
    float lower_hueH = (3.42 - delta_hue) < 0 ? 0 : (3.42 - delta_hue);  // Hue không được nhỏ hơn 0
    float upper_hueH = (3.42 + delta_hue*2) > 179 ? 179 : (3.42 + delta_hue);  // Hue không được lớn hơn 179

    float lower_hueS = (20.7979 - delta_hue/2) < 0 ? 0 : (20.7979 - delta_hue/2);  // Hue không được nhỏ hơn 0
    float upper_hueS = (20.7979 + delta_hue*5) > 255 ? 255 : (20.7979 + delta_hue*5);  // Hue không được lớn hơn 179

    float lower_hueV = (31.4529- delta_hue/2) < 0 ? 0 : (31.4529- delta_hue/2);  // Hue không được nhỏ hơn 0
    float upper_hueV = (31.4529 + delta_hue*5) > 255 ? 255 : (31.4529 + delta_hue*5);  // Hue không được lớn hơn 179

    // Đặt phạm vi màu nâu nhạt trong không gian HSV dựa trên màu chủ đạo
    cv::Scalar lower_bound(lower_hueH, lower_hueS, lower_hueV);  // Hue từ 3.42-10 đến 3.42+10, Saturation từ 20 đến 30, Value từ 31 đến 41
    cv::Scalar upper_bound(upper_hueH, upper_hueS, upper_hueV);  // Hue từ 3.42+10 đến 3.42-10, Saturation từ 50 đến 70, Value từ 60 đến 90

    // Đặt phạm vi màu nâu nhạt trong không gian HSV
    cv::Scalar lower_brown_light(10, 20, 100);  // Hue từ 10 đến 30, Saturation thấp (20) và Value ở mức 100
    cv::Scalar upper_brown_light(30, 100, 200); // Hue từ 10 đến 30, Saturation từ 20 đến 100, Value từ 100 đến 200



    // Tạo mặt nạ (mask) lọc màu xám
    Mat mask;
    inRange(imgHSV, lower_bound, upper_bound, mask);

    // Lưu mặt nạ vào một file ảnh mới
    imwrite("maskObject2.png", mask);  // Lưu mặt nạ dưới dạng file PNG

    // Hiển thị mặt nạ
    imshow("Mask", mask);

    // Lọc ra những phần có màu đen từ ảnh gốc
    Mat result;
    bitwise_and(img, img, result, mask);  // Áp dụng mặt nạ lên ảnh

    // Hiển thị ảnh kết quả
    imshow("Filtered Image", result);
    
    // Lưu ảnh kết quả vào file mới (nếu cần)
    imwrite("resultImage.png", result);

    // Đợi người dùng nhấn phím để đóng cửa sổ
    waitKey(0);
    return 0;
}
