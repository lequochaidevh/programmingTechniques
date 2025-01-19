#include <iostream>
#include <chrono>
#include <thread>

void showLoadingBar(int total, int progress, const std::string& sticker) {
    float percentage = (float)progress / total * 100;
    int barWidth = 50;  // Width of the loading bar
    int pos = barWidth * progress / total;

    std::cout << sticker << "\t" << " [";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) {
            std::cout << "#";  // Filled part of the bar
        } else {
            std::cout << " ";  // Empty part of the bar
        }
    }
    std::cout << "] " << int(percentage) << "%\r";  // \r returns the cursor to the start of the line
    std::cout.flush();
}

int main() {
    int total = 100;  // Total number of steps in the loading process
    std::string stickers[] = {"   /", "   ─", "   \\", "   |", "DONE"};
    int stickerIndex = 0;  // Used to cycle through the sticker states
    std::cout << "Loading ... " << std::endl;
    for (int i = 0; i <= total; ++i) {
        showLoadingBar(total, i, stickers[stickerIndex]);

        // Cycle through the sticker states
        stickerIndex = (stickerIndex + 1) % 4;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate work being done
    }
    showLoadingBar(total, 100, stickers[4]);
    std::cout << std::endl;  // Print a newline after loading bar is done
    std::cout << "Loading complete!" << std::endl;
    return 0;
}
