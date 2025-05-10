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
    cv::threshold(blurred, thresholded, 0, 50, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

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

    // Chuyển markers sang kiểu CV_8U để sử dụng với findContours
    cv::Mat markers8U;
    markers.convertTo(markers8U, CV_8U);

    // Tạo mặt nạ chỉ giữ các biên vật thể
    cv::Mat result1 = image.clone(); // Tạo bản sao của ảnh gốc để vẽ biên

    // Tạo mặt nạ cho biên bằng cách dilate
    cv::Mat dilatedEdges;
    cv::dilate(edges, dilatedEdges, cv::Mat(), cv::Point(-1, -1), 1); // Dùng dilate để nối các biên lại

    // Vẽ biên vật thể lên ảnh gốc
    // for (int i = 0; i < dilatedEdges.rows - 10; i++) {
    //     for (int j = 0; j < dilatedEdges.cols -10 ; j++) {
    //         if (dilatedEdges.at<uchar>(i, j) == 255) {
    //             // Nếu là biên, tô màu đỏ (BGR: 0, 0, 255)
    //             result1.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255); // Màu đỏ
    //         }
    //     }
    // }

     // Vẽ biên vật thể lên ảnh gốc, lọc chỉ vẽ biên ngoài
    // Lọc nhiễu bên trong con mèo (giảm các dấu chấm nhỏ)
    cv::Mat cleanedEdges;
    cv::morphologyEx(dilatedEdges, cleanedEdges, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1, -1), 1); // Dùng closing để loại bỏ các nhiễu nhỏ

    // Cắt các vùng nhiễu bằng Threshold
    cv::Mat binaryEdges;
    cv::threshold(dilatedEdges, binaryEdges, 100, 255, cv::THRESH_OTSU);

    // Dùng FindContours để tìm các biên vật thể
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaryEdges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Vẽ biên vật thể lên ảnh gốc
    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);

        // Nếu diện tích của contour đủ lớn, vẽ biên
        if (area > 0) { // Diện tích nhỏ có thể là nhiễu
            cv::drawContours(result1, contours, (int)i, cv::Scalar(0, 0, 255), 2);  // Vẽ biên màu đỏ
        }
    }

    // cv::Mat result = image.clone(); // Tạo bản sao của ảnh gốc để vẽ biên
    // Tìm các biên của hình ảnh (contours)
    // std::vector<std::vector<cv::Point>> contours;
    // cv::findContours(markers8U, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE); // Chỉ tìm biên ngoài

    // // Vẽ các biên lên ảnh gốc (đảm bảo chỉ vẽ các biên ngoài)
    // for (size_t i = 0; i < contours.size(); i++) {
    //     // Nếu diện tích contour đủ lớn, vẽ biên
    //     double area = cv::contourArea(contours[i]);

    //     // Kiểm tra xem contour có tiếp xúc với biên ngoài ảnh không
    //     bool isOuterContour = false;
    //     int tolerance = 0;  // Đặt dung sai, ví dụ 10 pixel

    //     for (size_t j = 0; j < contours[i].size(); j++) {
    //         cv::Point pt = contours[i][j];

    //         // Kiểm tra xem điểm có nằm trong khoảng dung sai gần biên ảnh hay không
    //         if (pt.x == tolerance || pt.x >= markers.cols - tolerance ||
    //             pt.y == tolerance || pt.y >= markers.rows - tolerance) {
    //             isOuterContour = true;
    //             //break;  // Nếu một điểm tiếp xúc với biên ngoài ảnh, đánh dấu là biên ngoài
    //         }
    //         isOuterContour = true;
    //     }

    //     // Nếu diện tích đủ lớn và không phải biên ngoài ảnh, vẽ biên
    //     if (area > 200 && !isOuterContour) {
    //         cv::drawContours(result1, contours, (int)i, cv::Scalar(0, 0, 255), 2);  // Vẽ biên màu đỏ
    //     }
    // }
    // Hiển thị kết quả
    cv::imshow("v", result1);
    cv::waitKey(0);

    return 0;
}
