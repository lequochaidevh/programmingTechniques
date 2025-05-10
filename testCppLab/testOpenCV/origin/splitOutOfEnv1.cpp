#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Đọc ảnh
    cv::Mat image = cv::imread("/home/devh/workspaceTesting/testOpenCV/origin/cat4.jpg");

    // Kiểm tra ảnh có tồn tại không
    if (image.empty()) {
        std::cout << "Không thể mở ảnh!" << std::endl;
        return -1;
    }

    // Chuyển ảnh sang ảnh xám
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Làm mờ ảnh để giảm nhiễu
    cv::Mat blurred[3];
    cv::GaussianBlur(gray, blurred[0], cv::Size(1, 1), 1);
    cv::GaussianBlur(gray, blurred[1], cv::Size(5, 5), 10);
    cv::GaussianBlur(gray, blurred[2], cv::Size(5, 5), 200);

    // Áp dụng Thresholding để tạo mặt nạ cho vật thể
    cv::Mat thresholded;
    cv::threshold(gray, thresholded, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

    // Tạo ảnh trắng đen (chỉ có giá trị 1 và 0)
    cv::Mat binaryImage;
    thresholded.convertTo(binaryImage, CV_8U, 1.0 / 255.0); // Chuyển giá trị thành 0 hoặc 1
    cv::imshow("Binary 1 Mask", thresholded);

    cv::Mat bw;	
    adaptiveThreshold(gray, bw, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 12);
    cv::imshow("adaptiveThreshold Mask", thresholded);

    // Tìm các biên của vật thể
    cv::Mat edges;
    cv::Canny(thresholded, edges, 0, 250);

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

    // Tạo một ảnh để vẽ các biên
    cv::Mat result = image.clone();

    // Tạo mặt nạ cho biên bằng cách dilate
    cv::Mat dilatedEdges;
    cv::dilate(edges, dilatedEdges, cv::Mat(), cv::Point(-1, -1), 1); // Dùng dilate để nối các biên lại

    // Vẽ biên vật thể lên ảnh gốc, lọc chỉ vẽ biên ngoài
    // Lọc nhiễu bên trong con mèo (giảm các dấu chấm nhỏ)
    cv::Mat cleanedEdges;
    cv::morphologyEx(dilatedEdges, cleanedEdges, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1, -1), 1); // Dùng closing để loại bỏ các nhiễu nhỏ

    // Cắt các vùng nhiễu bằng Threshold
    cv::Mat binaryEdges;
    cv::threshold(dilatedEdges, binaryEdges, 50, 100, cv::THRESH_OTSU);

    // Dùng FindContours để tìm các biên vật thể
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(dilatedEdges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Tạo một mặt nạ rỗng để tô màu chu dao cho đối tượng
    cv::Mat coloredMask = cv::Mat::zeros(image.size(), image.type());

    // Duyệt qua các contour để tô màu cho đối tượng
    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);

        // Nếu diện tích của contour đủ lớn, tô màu chu dao (màu đỏ) vào mặt nạ
        if (area > 450) { // Diện tích nhỏ có thể là nhiễu
            cv::drawContours(coloredMask, contours, (int)i, cv::Scalar(0, 0, 255), cv::FILLED); // Màu đỏ
        }
    }

    // Áp dụng mặt nạ màu chu dao vào ảnh gốc (Lấy chỉ phần vật thể và tô màu)
    cv::Mat resultWithMask;
    cv::addWeighted(image, 1.0, coloredMask, 0.5, 0, resultWithMask);

    // Hiển thị các ảnh:
    // 1. Ảnh trắng đen (Binary Mask)
    cv::imshow("Binary Mask", binaryImage);

    // 2. Ảnh viền của các vật thể (Edges)
    cv::imshow("Edges", edges);

    // 3. Ảnh kết quả cuối cùng (Vẽ biên vật thể với màu chu dao)
    cv::imshow("Result with Colored Mask", resultWithMask);
    
    cv::waitKey(0);

    return 0;
}
