#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp> 

int main() {
    // Đọc ảnh gốc và ảnh mask
    cv::Mat image = cv::imread("cat4.jpg");  // Thay "path_to_your_image.jpg" bằng đường dẫn ảnh của bạn
    cv::Mat mask = cv::imread("maskObject.png", cv::IMREAD_GRAYSCALE);  // Đọc mask với chế độ grayscale

    // Kiểm tra nếu ảnh không đọc được
    if (image.empty() || mask.empty()) {
        std::cout << "Không thể đọc ảnh!" << std::endl;
        return -1;
    }

    // Kiểm tra nếu kích thước ảnh gốc và ảnh mask không khớp
    if (image.size() != mask.size()) {
        std::cout << "Kích thước ảnh gốc và mask không khớp!" << std::endl;
        return -1;
    }

    cv::imshow("image_Origin", image);

    // Áp dụng mask lên ảnh gốc (chỉ giữ lại các phần trong mask)
    cv::Mat imgProcessing1;
    image.copyTo(imgProcessing1, mask);  // Copy vùng được mask giữ lại vào imgProcessing1
    cv::imshow("imgProcessing_applyMask", imgProcessing1);

    // Chuyển ảnh sang ảnh xám
    cv::Mat gray;
    cv::cvtColor(imgProcessing1, gray, cv::COLOR_BGR2GRAY);
    // cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::imshow("gray Processing 2", gray);

    // Áp dụng Thresholding để tạo mặt nạ cho vật thể
    cv::Mat thresholded;
    cv::threshold(gray, thresholded, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
    cv::Mat bw;	
	adaptiveThreshold(gray, bw, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 12);
    cv::imshow("adaptiveThreshold Mask", bw);
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

    // Vẽ biên vật thể lên ảnh gốc
    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);

        // Nếu diện tích của contour đủ lớn, vẽ biên
        if (area > 450) { // Diện tích nhỏ có thể là nhiễu
            cv::drawContours(result, contours, (int)i, cv::Scalar(0, 0, 255), 2);  // Vẽ biên màu đỏ
        }
    }


    // 3. Ảnh kết quả cuối cùng (Vẽ biên vật thể)
    cv::imshow("Result", result);
    // Vòng lặp chờ nhấn phím 'q' để thoát
    while (true) {
        char key = cv::waitKey(1);  // Kiểm tra phím nhấn mỗi 1ms
        if (key == 'q' || key == 'Q') {
            break;  // Thoát vòng lặp khi nhấn 'q' hoặc 'Q'
        }
    }

    return 0;
}