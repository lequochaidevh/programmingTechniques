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

    // Áp dụng mask lên ảnh gốc (chỉ giữ lại các phần trong mask)
    cv::Mat result;
    image.copyTo(result, mask);  // Copy vùng được mask giữ lại vào result

    // Hiển thị ảnh gốc, mask và ảnh kết quả
    cv::imshow("Image 2", image);
    cv::imshow("Mask 2", mask);
    cv::imshow("Result 2", result);

    cv::waitKey(0);  // Đợi phím bấm

    return 0;
}