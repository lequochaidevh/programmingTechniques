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

    // Chuyển ảnh gốc thành ảnh 8-bit
    image.convertTo(image, CV_8U);

    // Thay đổi màu viền khoanh (ví dụ: màu đỏ)
    // for (int i = 0; i < markers.rows; i++) {
    //     for (int j = 0; j < markers.cols; j++) {
    //         if (markers.at<int>(i, j) == -1) {
    //             // Nếu là biên giới, tô màu đỏ (BGR: 0, 0, 255)
    //             image.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255); // Màu đỏ
    //         }
    //     }
    // }

    // Hiển thị kết quả
    cv::imshow("Kết quả với viền màu đỏ và điểm trung tâm", image);
    cv::waitKey(0);

    return 0;
}
