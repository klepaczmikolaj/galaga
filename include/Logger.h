#include <fstream>
#include <iostream>
#include <ctime>
#include <chrono>
#include <string>
#include <sstream>

#ifndef LOGGER_H
#define LOGGER_H

class Logger
{
    public:
        enum outputType {
            file,
            console
        };
        outputType out;
        Logger();
        Logger(outputType outType);
        ~Logger();

        std::fstream loggerFile;

        enum class severity{
            error,
            info,
            warning
        };

        int errorNumber, infoNumber, warningNumber;

        std::string getStringFromEnum(severity);
        void appendInfoNumber(severity);

        std::ostream &outStream = (out == outputType::console ? std::cout : loggerFile);


    template<typename T>
    void appendLogger(severity sev, T object) {
        std::time_t time = std::time(nullptr);
        std::string timeString = std::ctime(&time);
        timeString.pop_back();
        appendInfoNumber(sev);
        outStream << "[" << timeString << "]" << "[" << getStringFromEnum(sev) << "]" << object << std::endl;
    }

};


#endif // LOGGER_H

