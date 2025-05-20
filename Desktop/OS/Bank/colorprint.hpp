#ifndef LIBCOLORPRINT_COLORPRINT_HPP
#define LIBCOLORPRINT_COLORPRINT_HPP

#include <vector>
#include <string>

class Painter {
public:
    explicit Painter(std::ostream &stream,
                     const std::vector<std::string> &successPatterns = defaultSuccessPatterns(),
                     const std::vector<std::string> &failPatterns = defaultFailPatterns());

    void printColoredLine(const std::string &);

private:
    std::ostream &stream;
    std::vector<std::string> successPatterns;
    std::vector<std::string> failPatterns;

    static std::vector<std::string> defaultSuccessPatterns();

    static std::vector<std::string> defaultFailPatterns();
};

#endif //LIBCOLORPRINT_COLORPRINT_HPP
