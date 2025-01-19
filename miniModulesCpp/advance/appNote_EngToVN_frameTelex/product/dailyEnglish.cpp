#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

// Function to convert Telex input to Vietnamese character based on Telex mapping
std::unordered_map<std::string, std::string> telexMap = {
    {"a", "a"}, {"as", "á"}, {"af", "à"}, {"ar", "ả"}, {"ax", "ã"}, {"aj", "ạ"},
    {"e", "e"}, {"es", "é"}, {"ef", "è"}, {"er", "ẻ"}, {"ex", "ẽ"}, {"ej", "ẹ"},
    {"o", "o"}, {"os", "ó"}, {"of", "ò"}, {"or", "ỏ"}, {"ox", "õ"}, {"oj", "ọ"},
    {"u", "u"}, {"us", "ú"}, {"uf", "ù"}, {"ur", "ủ"}, {"ux", "ũ"}, {"uj", "ụ"},
    {"i", "i"}, {"is", "í"}, {"if", "ì"}, {"ir", "ỉ"}, {"ix", "ĩ"}, {"ij", "ị"},
    {"y", "y"}, {"ys", "ý"}, {"yf", "ỳ"}, {"yr", "ỷ"}, {"yx", "ỹ"}, {"yj", "ỵ"},
    {"oo", "ô"}, {"oos", "ố"}, {"oof", "ồ"}, {"oor", "ổ"}, {"oox", "ỗ"}, {"ooj", "ộ"},
    {"aa", "â"}, {"aas", "ấ"}, {"aaf", "ầ"}, {"aar", "ẩ"}, {"aax", "ẫ"}, {"aaj", "ậ"},
    {"ow", "ơ"}, {"ows", "ớ"}, {"owf", "ờ"}, {"owr", "ở"}, {"owx", "ỡ"}, {"owj", "ợ"},
    {"aw", "ă"}, {"aws", "ắ"}, {"awf", "ằ"}, {"awr", "ẳ"}, {"awx", "ẵ"}, {"awj", "ặ"},
    {"uw", "ư"}, {"uws", "ứ"}, {"uwf", "ừ"}, {"uwr", "ử"}, {"uwx", "ữ"}, {"uwj", "ự"},
    {"ee", "ê"}, {"ees", "ế"}, {"eef", "ề"}, {"eer", "ể"}, {"eex", "ễ"}, {"eej", "ệ"},
    {"d", "d"}, {"dd", "đ"}
};

std::string telexToVietnamese(const std::string &word) {
    // Telex-to-Vietnamese mapping


    // If the word is found in the Telex map, return the corresponding Vietnamese word
    if (telexMap.find(word) != telexMap.end()) {
        return telexMap[word];
    }

    // If not found in the map, return the word as is
    return word;
}

std::string stringProducer(std::string stringPagram) {
    std::string input = stringPagram; // Telex input to be converted
    std::string output = "";
    std::string foundInMap = "";
    std::unordered_map<std::string,std::string>::const_iterator got;
    // Convert each word in the input string
    std::string word = "";
    for (char c : input) {
        if (c == ' ') {
            // Process the current word and append its Vietnamese translation
            output += word;
            if(foundInMap != "")  output += telexToVietnamese(foundInMap);
            output += " ";
            word = foundInMap = "";  // Reset word
        } else {
            if(foundInMap != ""){
                got = telexMap.find(foundInMap + c);
                if (got == telexMap.end()) {
                    word += telexToVietnamese(foundInMap);
                    foundInMap = "";
                    got = telexMap.find(&c);
                    if (got != telexMap.end()) goto CharLable;
                    word += c;
                }
                else {
                    foundInMap += c;
                }
                continue;
            }
            else {
                got = telexMap.end(); // OBLIGATE
                got = telexMap.find(&c); //get start left word
            }

            if (got != telexMap.end()){
CharLable:
                foundInMap += c;
                continue;
            }

            word += c;  // Add character to the current word
        }
    }

    // Don't forget to process the last word
    if (!word.empty()) {
        output += telexToVietnamese(word);
    }

    // Output the original and converted strings
    std::cout << "Original: " << input << std::endl;
    std::cout << "Converted: " << output << std::endl;

    return output;
}

std::string alignFunc(int lengA,int lengExpect,int spaceB = 40){
    std::string alpha = "";
    for(int i = lengA; i < (lengExpect - spaceB); i++){
        alpha += " ";
    }
    return  alpha;
}

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
    std::string word3 = "";
    word3 = stringProducer(word2);

    // Open the file in append mode
    std::ofstream outFile("engTranslate.txt", std::ios::app);
    
    // Check if the file opened successfully
    if (!outFile) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // Reuse word2 for other purpose
    // Write the words with a tab between them
    outFile << word1 << alignFunc(word1.length(), 80) << word3 << std::endl;

    // Close the file
    outFile.close();

    std::cout << "Words appended to engTranslate.txt successfully!" << std::endl;
    
    return 0;
}
