#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    // Check if exactly 3 arguments are passed (program name + 2 words)
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <word1> <word2>" << std::endl; //arg0 is name of file .bin
        return 1;
    }
    int numChar = argc;
    int wordTrans = 2;
    std::string word2 = "";
    // Arguments: argv[1] is word1, argv[2] is word2
    std::string word1 = argv[1];
    while(numChar-2){
        word2 += argv[wordTrans]; wordTrans++;
        word2 += " "; numChar--;
    }


    // Open the file in append mode
    std::ofstream outFile("engTranslate.txt", std::ios::app);
    
    // Check if the file opened successfully
    if (!outFile) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // Write the words with a tab between them
    outFile << word1 << "\t\t\t\t\t" << word2 << std::endl;

    // Close the file
    outFile.close();

    std::cout << "Words appended to engTranslate.txt successfully!" << std::endl;
    
    return 0;
}
