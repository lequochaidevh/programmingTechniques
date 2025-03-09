#include <iostream>
#include <cmath>
#include <array>

// Hàm tạo ma trận chiếu phối cảnh cho OpenGL
std::array<std::array<float, 4>, 4> PerspectiveMatrixOpenGL(float fovY, float aspect, float nearZ, float farZ) {
    // Chuyển đổi FOV từ độ sang radian
    float tanHalfFovY = tanf(fovY / 2.0f);
    
    // Ma trận chiếu cho OpenGL
    std::array<std::array<float, 4>, 4> mat = {{
        {1.0f / (tanHalfFovY * aspect), 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f / tanHalfFovY, 0.0f, 0.0f},
        {0.0f, 0.0f, (farZ + nearZ) / (nearZ - farZ), (2.0f * farZ * nearZ) / (nearZ - farZ)},
        {0.0f, 0.0f, -1.0f, 0.0f}
    }};
    return mat;
}

// Hàm tạo ma trận chiếu phối cảnh cho DirectX
std::array<std::array<float, 4>, 4> PerspectiveMatrixDirectX(float fovY, float aspect, float nearZ, float farZ) {
    // Chuyển đổi FOV từ độ sang radian
    float tanHalfFovY = tanf(fovY / 2.0f);

    // Ma trận chiếu cho DirectX
    std::array<std::array<float, 4>, 4> mat = {{
        {1.0f / (tanHalfFovY * aspect), 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f / tanHalfFovY, 0.0f, 0.0f},
        {0.0f, 0.0f, farZ / (farZ - nearZ), -farZ * nearZ / (farZ - nearZ)},
        {0.0f, 0.0f, 1.0f, 0.0f}
    }};
    return mat;
}

// Hàm in ma trận
void PrintMatrix(const std::array<std::array<float, 4>, 4>& mat) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << mat[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

int main() {
    // Các tham số đầu vào
    float fovY = 45.0f * (M_PI / 180.0f);  // 45 độ (chuyển sang radian)
    float aspect = 16.0f / 9.0f;            // Tỷ lệ khung hình (16:9)
    float nearZ = 0.1f;                     // Khoảng cách đến mặt phẳng gần
    float farZ = 1000.0f;                   // Khoảng cách đến mặt phẳng xa

    // Tạo ma trận chiếu cho OpenGL
    std::array<std::array<float, 4>, 4> matOpenGL = PerspectiveMatrixOpenGL(fovY, aspect, nearZ, farZ);
    std::cout << "OpenGL Perspective Matrix:\n";
    PrintMatrix(matOpenGL);

    std::cout << "\n";

    // Tạo ma trận chiếu cho DirectX
    std::array<std::array<float, 4>, 4> matDirectX = PerspectiveMatrixDirectX(fovY, aspect, nearZ, farZ);
    std::cout << "DirectX Perspective Matrix:\n";
    PrintMatrix(matDirectX);

    return 0;
}
