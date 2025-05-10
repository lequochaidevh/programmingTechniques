#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

cv::Scalar getDominantColor(const cv::Mat& image, const cv::Mat& mask) {
    // Lọc các điểm ảnh theo mask (chỉ giữ lại các điểm ảnh trong vùng mask)
    cv::Mat maskedImage;
    image.copyTo(maskedImage, mask);

    // Kiểm tra xem vùng trong mask có chứa dữ liệu màu không
    if (cv::countNonZero(mask) == 0) {
        std::cout << "Vùng mask không có dữ liệu màu!" << std::endl;
        return cv::Scalar(0, 0, 0);  // Nếu không có dữ liệu, trả về màu đen
    }

    // Chuyển đổi ảnh sang không gian màu HSV để dễ dàng tìm màu chủ đạo
    cv::Mat hsvImage;
    cv::cvtColor(maskedImage, hsvImage, cv::COLOR_BGR2HSV);

    // Biến dữ liệu để thực hiện K-means
    std::vector<cv::Mat> channels;
    cv::split(hsvImage, channels);

    // Lấy tất cả dữ liệu từ các kênh Hue, Saturation và Value
    cv::Mat data;
    cv::Mat temp[] = {channels[0].reshape(1, channels[0].total()),  // Hue
                      channels[1].reshape(1, channels[1].total()),  // Saturation
                      channels[2].reshape(1, channels[2].total())}; // Value
    cv::merge(temp, 3, data);  // Gộp lại thành 3 kênh

    // Đảm bảo rằng data có kiểu CV_32F
    data.convertTo(data, CV_32F);

    // Tiến hành phân cụm với K-means
    int k = 1;  // Sử dụng 1 cụm để tìm màu chủ đạo
    cv::Mat labels, centers;
    cv::kmeans(data, k, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 100, 0.2), 10, cv::KMEANS_RANDOM_CENTERS, centers);

    // Tìm màu chủ đạo từ kết quả của K-means (lấy trung tâm cụm)
    cv::Scalar dominantColorHSV = cv::Scalar(centers.at<float>(0, 0), centers.at<float>(0, 1), centers.at<float>(0, 2));

    return dominantColorHSV;  // Trả về màu chủ đạo trong không gian HSV
}

int main() {
    // Đọc ảnh gốc
    cv::Mat image = cv::imread("cat3.jpg");  // Thay "path_to_your_image.jpg" bằng đường dẫn ảnh của bạn

    // Kiểm tra nếu ảnh không đọc được
    if (image.empty()) {
        std::cout << "Không thể đọc ảnh!" << std::endl;
        return -1;
    }

    // Tạo một mask cho vùng hình tròn (vùng bạn khoanh)
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);

    // Vẽ một hình tròn vào mask (thay đổi tọa độ và bán kính theo vùng bạn khoanh)
    cv::Point center(image.cols / 2, image.rows / 2); // Tọa độ tâm hình tròn
    int radius = 100; // Bán kính hình tròn
    cv::circle(mask, center, radius, cv::Scalar(255), -1); // Vẽ hình tròn vào mask

    // Vẽ một hình tròn lên ảnh gốc để dễ dàng quan sát
    cv::circle(image, center, radius, cv::Scalar(0, 255, 0), 2); // Vẽ hình tròn màu xanh lá, độ dày 2

    // Tìm màu chủ đạo trong vùng hình tròn đã khoanh
    cv::Scalar dominantColor = getDominantColor(image, mask);

    std::cout << "Màu chủ đạo (HSV): " << dominantColor << std::endl;

    // Hiển thị ảnh và mask
    cv::imshow("Image with Circle", image);  // Hiển thị ảnh với hình tròn
    cv::imshow("Mask", mask);                // Hiển thị mask
    cv::waitKey(0);

    return 0;
}
