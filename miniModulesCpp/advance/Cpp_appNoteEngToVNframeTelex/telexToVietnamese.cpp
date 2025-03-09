#include <iostream>
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
    {"aa", "â"}, {"aas", "á"}, {"aaf", "ầ"}, {"aar", "ẩ"}, {"aax", "ẫ"}, {"aaj", "ậ"},
    {"ow", "ơ"}, {"ows", "ớ"}, {"owf", "ờ"}, {"owr", "ở"}, {"owx", "ỡ"}, {"owj", "ợ"},
    {"aw", "aw"}, {"aws", "ắ"}, {"awf", "ằ"}, {"awr", "ẳ"}, {"awx", "ẵ"}, {"awj", "ặ"},
    {"uw", "ư"}, {"uws", "ứ"}, {"uwf", "ừ"}, {"uwr", "ử"}, {"uwx", "ữ"}, {"uwj", "ự"}
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

int main() {
    std::string input = "aa oon  n phuwowng thaajt lofng thuoosc"; // Telex input to be converted
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
            //std::cout << "c: " << c << "  foundInMap: " << foundInMap << std::endl;
            if(foundInMap != ""){
                got = telexMap.find(foundInMap + c);//std::cout << "foundInMap2c: " << foundInMap+c << std::endl;
                if (got == telexMap.end()) {
                    //std::cout << "foundInMap3c: " << foundInMap+c << std::endl;
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
                //std::cout << "found c when foundInMap= "" " << c << std::endl;
            }

            if (got != telexMap.end()){
                CharLable:
                foundInMap += c;std::cout << "foundInMap1: " << foundInMap << std::endl;
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

    return 0;
}

