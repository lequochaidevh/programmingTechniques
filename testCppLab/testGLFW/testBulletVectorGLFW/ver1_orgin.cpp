#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

struct Bullet {
    float x, y;         // Vị trí viên đạn
    float speed;        // Tốc độ viên đạn
    float angle;        // Góc di chuyển của viên đạn
    float length;       // Chiều dài thân viên đạn (mũi tên)
    float width;        // Độ rộng của mũi tên
};

float mouseX = 0.0f, mouseY = 0.0f;
Bullet bullet = {400.0f, 300.0f, 0.5f, 0.0f, 30.0f, 5.0f};  // Khởi tạo viên đạn

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = static_cast<float>(xpos);
    mouseY = static_cast<float>(ypos);
}

// Hàm vẽ viên đạn dạng mũi tên
void drawBullet() {
    glColor3f(1.0f, 0.0f, 0.0f); // Màu đỏ cho viên đạn

    // Tính các điểm cần thiết để vẽ mũi tên
    float dx = cos(bullet.angle) * bullet.length;
    float dy = sin(bullet.angle) * bullet.length;
    
    // Vẽ thân mũi tên
    glBegin(GL_LINES);
        glVertex2f(bullet.x, bullet.y); // Điểm gốc
        glVertex2f(bullet.x + dx, bullet.y + dy); // Điểm đích
    glEnd();
    
    // Vẽ đầu mũi tên (hình tam giác)
    float arrowHeadLength = 10.0f;  // Chiều dài đầu mũi tên
    float arrowHeadAngle = 3.14159f / 6.0f;  // 30 độ
    
    // Các điểm của đầu mũi tên
    float leftX = bullet.x + dx - arrowHeadLength * cos(bullet.angle + arrowHeadAngle);
    float leftY = bullet.y + dy - arrowHeadLength * sin(bullet.angle + arrowHeadAngle);
    float rightX = bullet.x + dx - arrowHeadLength * cos(bullet.angle - arrowHeadAngle);
    float rightY = bullet.y + dy - arrowHeadLength * sin(bullet.angle - arrowHeadAngle);
    
    // Vẽ hai cạnh của đầu mũi tên
    glBegin(GL_TRIANGLES);
        glVertex2f(bullet.x + dx, bullet.y + dy); // Đỉnh mũi tên
        glVertex2f(leftX, leftY);   // Mũi tên bên trái
        glVertex2f(rightX, rightY); // Mũi tên bên phải
    glEnd();
}

// Hàm cập nhật vị trí viên đạn
void updateBulletPosition() {
    // Tính khoảng cách giữa viên đạn và con trỏ
    float dx = mouseX - bullet.x;
    float dy = mouseY - bullet.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 1.0f) {
        // Cập nhật góc di chuyển của viên đạn (theo hướng con trỏ chuột)
        bullet.angle = std::atan2(dy, dx);

        // Cập nhật vị trí viên đạn theo hướng con trỏ chuột
        bullet.x += bullet.speed * cos(bullet.angle);
        bullet.y += bullet.speed * sin(bullet.angle);

        // Giảm tốc độ viên đạn (chậm dần)
        bullet.speed *= 0.99f;  // Tốc độ giảm 1% mỗi lần
        if (bullet.speed < 0.01f) {
            bullet.speed = 0.01f; // Đảm bảo tốc độ không quá nhỏ
        }
    }
}

int main() {
    // Khởi tạo GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Khởi tạo cửa sổ
    GLFWwindow* window = glfwCreateWindow(800, 600, "Bullet Following Mouse", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    glOrtho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0); // Thiết lập hệ tọa độ OpenGL

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        updateBulletPosition(); // Cập nhật vị trí viên đạn
        drawBullet();           // Vẽ viên đạn

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
