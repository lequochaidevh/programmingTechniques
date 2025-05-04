#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

// Hàm F(x) từ 0.5 đến 1 khi x giảm từ giá trị lớn đến 0
double F_Sigmoid(double x) {
    double k = 1.0;  // Hằng số điều chỉnh tốc độ giảm
    return 0.5 + 0.5 * (1.0 - 1.0 / (1.0 + exp(-k * x))); // Sử dụng sigmoid để điều chỉnh tốc độ
}

double F_Sigmoid_f01t0(double x) {
    double k = 1.0;  // Hằng số điều chỉnh tốc độ giảm
    
    if(x < 6.28) {
        //std::cout << x << std::endl;
        return 0;
    }
    double result = 0.01 *(1 - exp(-k * x));  // Hàm sigmoid điều chỉnh
    //if ((result - 0.01f) > 0.01) std::cout << result << std::endl;
    
    return result;
}

struct Bullet {
    float x, y;         // Vị trí viên đạn
    float speed;        // Tốc độ viên đạn
    float angle;        // Góc di chuyển của viên đạn
    float length;       // Chiều dài thân viên đạn (mũi tên)
    float width;        // Độ rộng của mũi tên
};

float mouseX = 0.0f, mouseY = 0.0f;
Bullet bullet = {400.0f, 300.0f, 1.0f, 0.0f, 30.0f, 40.0f};  // Khởi tạo viên đạn

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
        glVertex2f(bullet.x, bullet.y); // Điểm gốc (đỉnh mũi tên)
        glVertex2f(bullet.x - 1.5 * dx, bullet.y - 1.5 * dy); // Điểm đích
    glEnd();
    
    // Vẽ đầu mũi tên (hình tam giác)
    float arrowHeadLength = 10.0f;  // Chiều dài đầu mũi tên
    float arrowHeadAngle = 3.14159f / 6.0f;  // 30 độ
    
    // Các điểm của đầu mũi tên (dùng điểm gốc là đỉnh tam giác)
    float leftX = bullet.x - dx + arrowHeadLength * cos(bullet.angle + arrowHeadAngle);
    float leftY = bullet.y - dy + arrowHeadLength * sin(bullet.angle + arrowHeadAngle);
    float rightX = bullet.x - dx + arrowHeadLength * cos(bullet.angle - arrowHeadAngle);
    float rightY = bullet.y - dy + arrowHeadLength * sin(bullet.angle - arrowHeadAngle);
    
    // Vẽ hai cạnh của đầu mũi tên
    glBegin(GL_TRIANGLES);
        glVertex2f(bullet.x, bullet.y); // Đỉnh mũi tên (đỉnh tam giác)
        glVertex2f(leftX, leftY);   // Mũi tên bên trái
        glVertex2f(rightX, rightY); // Mũi tên bên phải
    glEnd();
}

// Hàm cập nhật vị trí viên đạn
void updateBulletPosition() {
    static uint64_t lastest_time = 0;
    static float latest_deltaPosition = 0;
    lastest_time += 3;
    // Tính khoảng cách giữa viên đạn và con trỏ
    float dx = mouseX - bullet.x;
    float dy = mouseY - bullet.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    if((distance - latest_deltaPosition) > 2.1) {
        lastest_time = 0;
        //std::cout << "New cursor updated" << std::endl;
    } 
    latest_deltaPosition = distance;
    // Nếu khoảng cách bằng 0, viên đạn sẽ dừng di chuyển và chỉ xoay
    if (distance == 0.0f) {
        return;  // Dừng di chuyển và chỉ xoay nếu khoảng cách bằng 0
    }

    // Tính góc di chuyển viên đạn hướng về con trỏ chuột
    float targetAngle = std::atan2(dy, dx);
    float angleDiff = targetAngle - bullet.angle;

    // Điều chỉnh góc lệch để xoay từ từ
    if (angleDiff > M_PI) {
        angleDiff -= 2 * M_PI;
    } else if (angleDiff < -M_PI) {
        angleDiff += 2 * M_PI;
    }

    // Tính toán tốc độ xoay
    float turnSpeed = 0.05f * F_Sigmoid(distance) * std::abs(angleDiff) * 0.0066f * lastest_time*0.00038;

    // Xoay viên đạn từ từ về hướng con trỏ chuột
    if (angleDiff > 0) {
        bullet.angle += turnSpeed;  // Xoay theo chiều thuận
    } else {
        bullet.angle -= turnSpeed;  // Xoay theo chiều ngược
    }

    // Cập nhật vị trí viên đạn nếu khoảng cách không bằng 0
    bullet.x += bullet.speed * cos(bullet.angle);
    bullet.y += bullet.speed * sin(bullet.angle);

    // Giảm tốc độ viên đạn dần dần
    // bullet.speed *= 0.99f;  // Tốc độ giảm 1% mỗi lần
    // if (bullet.speed < 0.01f) {
    //     bullet.speed = 0.01f; // Đảm bảo tốc độ không quá nhỏ
    // }
    bullet.speed = F_Sigmoid_f01t0(distance); //distance/10000;
    if(bullet.speed == 0) { lastest_time = 0; }

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
