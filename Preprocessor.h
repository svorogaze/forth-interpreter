#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include <string>


class Preprocessor {
public:
    explicit Preprocessor(std::string file_path);
    std::string GetCurrentText();
    void ToOneLine(); // transform file to one line for easier parsing
private:

public:


private:
    std::string current_text;

};



#endif //PREPROCESSOR_H
