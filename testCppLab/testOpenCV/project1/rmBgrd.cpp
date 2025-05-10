#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>

double logisticFunction(double x, double x0, double k) {
    // Hàm logistic với tham số dốc k tại điểm x0
    std::cout << 1.0 / (1.0 + exp(-k * (x - x0))) << " ---- ";
    return 1.0 / (1.0 + exp(-k * (x - x0)));
}

double scaleAndShift(double y, double maxA, double minB) {
    // Hàm để điều chỉnh y sao cho nằm trong khoảng [1, 5]
    return (maxA-minB) * abs(y-0.5) + minB;
}

double findScaleUp(double x, double x0, double maxA, double minB) {
    // Giả sử x0 = 2 và k điều chỉnh dốc (có thể thay đổi giá trị của k để kiểm soát độ dốc)
    double k = 5; // Độ dốc lớn ở x0
    double y = logisticFunction(x, x0, k);
    std::cout << y << std::endl;
    return scaleAndShift(y, maxA, minB);
}

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
    cv::Mat image = cv::imread("../imgTest/cat1.jpg");  
    // Kiểm tra nếu ảnh không đọc được
    if (image.empty()) {
        std::cout << "Không thể đọc ảnh!" << std::endl;
        return -1;
    }
    imshow("Origin Image", image);

    /*---------------------------------------------------------------------------------------------------------------------------*/

    // Tạo một mask cho vùng hình tròn (vùng bạn khoanh)
    cv::Mat maskCircle = cv::Mat::zeros(image.size(), CV_8UC1);
    // Vẽ một hình tròn vào mask (thay đổi tọa độ và bán kính theo vùng bạn khoanh)
    cv::Point center(image.cols / 2 - 20, image.rows / 2 + 20); // Tọa độ tâm hình tròn
    int radius = 90; // Bán kính hình tròn
    cv::circle(maskCircle, center, radius, cv::Scalar(255), -1); // Vẽ hình tròn vào mask

    // Đảm bảo rằng bạn đã có dominantColor (đã được xác định ở bước trước)
    cv::Scalar dominantColor = getDominantColor(image, maskCircle);
    std::cout << "Màu chủ đạo (HSV): " << dominantColor << std::endl;

    /*---------------------------------------------------------------------------------------------------------------------------*/

    // Lấy giá trị Hue, Saturation, và Value từ dominantColor (ở dạng Scalar)
    float dominantHue = dominantColor[0];  // Hue
    float dominantSaturation = dominantColor[1];  // Saturation
    float dominantValue = dominantColor[2];  // Value

    // In ra giá trị dominantColor
    std::cout << "Hue: " << dominantHue << ", Saturation: " << dominantSaturation << ", Value: " << dominantValue << std::endl;

    // Định nghĩa delta_hue
    //float delta_hue = 40;
    float delta_HSV = 0.055 * radius + (12345/radius/radius+0.33);
    float tempVal = 0;
    // Tính toán giá trị lower và upper cho Hue, Saturation, và Value
    tempVal = dominantHue / 2 - delta_HSV;
    float lower_hueH = (tempVal) < 0 ? 0 : (tempVal);
    tempVal = dominantHue * 15 * findScaleUp(dominantHue, 2.2, 5.0, 1.0) + delta_HSV;
    std::cout << findScaleUp(dominantHue, 2.2,  5.0, 1.0) << std::endl;
    float upper_hueH = (tempVal) > 255 ? 255 : (tempVal);

    // Tính toán cho Saturation (S)
    tempVal = dominantSaturation * 2.7 - delta_HSV;
    float lower_hueS = (tempVal) < 0 ? 0 : (tempVal);
    tempVal = dominantSaturation * 15 + delta_HSV;
    float upper_hueS = (tempVal) > 255 ? 255 : (tempVal);

    // Tính toán cho Value (V)
    tempVal = dominantValue * 4 - delta_HSV;
    float lower_hueV = (tempVal) < 0 ? 0 : (tempVal);
    tempVal = dominantValue * 14 + delta_HSV;
    float upper_hueV = (tempVal) > 255 ? 255 : (tempVal);
    
    // In kết quả
    std::cout << "Lower Hue: " << lower_hueH << ", Upper Hue: " << upper_hueH << std::endl;
    std::cout << "Lower Saturation: " << lower_hueS << ", Upper Saturation: " << upper_hueS << std::endl;
    std::cout << "Lower Value: " << lower_hueV << ", Upper Value: " << upper_hueV << std::endl;

    /*---------------------------------------------------------------------------------------------------------------------------*/

    // Đặt phạm vi màu trong không gian HSV dựa trên màu chủ đạo
    cv::Scalar lower_bound(lower_hueH, lower_hueS, lower_hueV);  // 
    cv::Scalar upper_bound(upper_hueH, upper_hueS, upper_hueV);  //  


    //cv::Scalar lower_gray(0, 0, 50);    // H, S gần 0 và V trong phạm vi từ 50 (tối) đến 200 (sáng)
    //cv::Scalar upper_gray(180, 70, 200); // H có thể dao động từ 0 đến 180, S gần 0 và V từ 50 đến 200 
    // Tạo mặt nạ (mask) lọc màu xám
    cv::Mat maskObj;
    // Chuyển đổi ảnh sang không gian màu HSV
    cv::Mat imgHSV;
    cv::cvtColor(image, imgHSV, cv::COLOR_BGR2HSV);
    cv::inRange(imgHSV, lower_bound, upper_bound, maskObj);

    // Lưu mặt nạ vào một file ảnh mới
    cv::imwrite("maskObject2.png", maskObj);  // Lưu mặt nạ dưới dạng file PNG

    // Hiển thị mặt nạ
    cv::imshow("Mask", maskObj);

    // Lọc ra những phần có màu đen từ ảnh gốc
    cv::Mat result;
    cv::bitwise_and(image, image, result, maskObj);  // Áp dụng mặt nạ lên ảnh

    // Hiển thị ảnh kết quả
    cv::imshow("Filtered Image", result);
    
    // Lưu ảnh kết quả vào file mới (nếu cần)
    // imwrite("resultImage.png", result);
    
    /*---------------------------------------------------------------------------------------------------------------------------*/

    // Áp dụng mask lên ảnh gốc (chỉ giữ lại các phần trong mask)
    cv::Mat imgProcessing1;
    image.copyTo(imgProcessing1, maskObj);  // Copy vùng được mask giữ lại vào imgProcessing1
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
	adaptiveThreshold(gray, thresholded, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 12);
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
    cv::Mat resultEnd = image.clone();
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
        if (area > 4000) { // Diện tích nhỏ có thể là nhiễu
            cv::drawContours(resultEnd, contours, (int)i, cv::Scalar(0, 0, 255), 2);  // Vẽ biên màu đỏ
        }
    }


    // 3. Ảnh kết quả cuối cùng (Vẽ biên vật thể)
    cv::imshow("resultEnd", resultEnd);


    /**** Design Function(imgage, center, redius) -> return img with circle *****/ 
    // Vẽ một hình tròn lên ảnh gốc để dễ dàng quan sát
    cv::circle(image, center, radius, cv::Scalar(0, 255, 0), 2); // Vẽ hình tròn màu xanh lá, độ dày 2
    cv::imshow("Image with Circle", image);  // Hiển thị ảnh với hình tròn


    // Vòng lặp chờ nhấn phím 'q' để thoát
    while (true) {
        char key = cv::waitKey(1);  // Kiểm tra phím nhấn mỗi 1ms
        if (key == 'q' || key == 'Q') {
            break;  // Thoát vòng lặp khi nhấn 'q' hoặc 'Q'
        }
    }
    return 0;
}
