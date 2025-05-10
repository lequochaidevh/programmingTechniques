#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Đọc ảnh
    cv::Mat image = cv::imread("/home/devh/workspaceTesting/testOpenCV/origin/cat1.jpg");

    // Kiểm tra ảnh có tồn tại không
    if (image.empty()) {
        std::cout << "Không thể mở ảnh!" << std::endl;
        return -1;
    }

    // Chuyển ảnh sang ảnh xám
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Làm mờ ảnh xám để giảm nhiễu
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

    // Áp dụng Thresholding để tạo mặt nạ cho vật thể (các giá trị khác nền)
    cv::Mat thresholded;
    cv::threshold(blurred, thresholded, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

    // Tìm các biên của vật thể
    cv::Mat edges;
    cv::Canny(thresholded, edges, 100, 200);

    // Tạo một ảnh mới để sử dụng cho Watershed
    cv::Mat markers = cv::Mat::zeros(image.size(), CV_32S);

    // Đánh dấu các vùng nền (vùng đen) và vùng vật thể (vùng trắng)
    markers.setTo(cv::Scalar(1), edges); // Biên vật thể được gán giá trị 1
    markers.setTo(cv::Scalar(2), thresholded); // Vùng nền được gán giá trị 2

    // Áp dụng thuật toán Watershed
    cv::watershed(image, markers);

    // Đánh dấu các vùng vật thể với giá trị -1 (để làm biên giới)
    image.convertTo(image, CV_8U);
    image.setTo(cv::Scalar(255, 255, 255), markers == -1); // Thay đổi biên giới thành trắng

    // Hiển thị kết quả
    cv::imshow("Kết quả Watershed", image);
    cv::waitKey(0);

    return 0;
}