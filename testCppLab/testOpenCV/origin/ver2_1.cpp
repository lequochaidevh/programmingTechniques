#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main() {
    // Đọc ảnh đầu vào
    cv::Mat image = cv::imread("/home/devh/workspaceTesting/testOpenCV/origin/cat2.jpg");

    if (image.empty()) {
        std::cout << "Không thể mở ảnh!" << std::endl;
        return -1;
    }

    
    // Chuyển ảnh sang không gian màu xám
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Làm mờ ảnh để giảm nhiễu
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

    // Dùng Canny để phát hiện biên
    cv::Mat edges;
    cv::Canny(blurred, edges, 100, 200);

    // Dùng dilate để làm mờ các biên, giúp tạo vùng background
    cv::Mat sure_bg;
    cv::dilate(edges, sure_bg, cv::Mat(), cv::Point(-1, -1), 3);

    // Tạo foreground bằng erode
    cv::Mat sure_fg;
    cv::erode(edges, sure_fg, cv::Mat(), cv::Point(-1, -1), 3);

    // Chuyển foreground thành ảnh nhị phân
    cv::threshold(sure_fg, sure_fg, 0, 255, cv::THRESH_BINARY);

    // Tính toán vùng không xác định (unknown)
    cv::Mat unknown;
    cv::subtract(sure_bg, sure_fg, unknown);

    // Tạo marker (vùng nền và foreground)
    cv::Mat markers = sure_fg.clone();
    markers = markers + 1; // Gán giá trị nền foreground là 1
    sure_bg = sure_bg + 2; // Gán giá trị nền background là 2
    markers = markers + sure_bg;

    // Đảm bảo markers có kiểu dữ liệu CV_32SC1
    markers.convertTo(markers, CV_32SC1);

    // Áp dụng thuật toán watershed
    cv::watershed(image, markers);

    // Tô màu cho các biên phân tách (dấu hiệu phân tách)
    cv::Mat result = image.clone();
    result.setTo(cv::Scalar(0, 0, 255), markers == -1); // Vẽ biên phân tách bằng màu đỏ

    // Tô màu các vùng phân đoạn (tương ứng với các nhãn watershed)
    for (int i = 0; i < markers.rows; i++) {
        for (int j = 0; j < markers.cols; j++) {
            if (markers.at<int>(i, j) == 1) {
                result.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 0, 0); // Màu xanh cho foreground
            } else if (markers.at<int>(i, j) == 2) {
                result.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 255, 0); // Màu xanh lá cho background
            }
        }
    }

    // Hiển thị kết quả
    cv::imshow("Original Image", image);
    cv::imshow("Segmented Image", result);
    cv::waitKey(0);

    return 0;
}