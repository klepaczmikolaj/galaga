
#include "Logger.h"


Logger::Logger(){
    errorNumber = 0;
    infoNumber = 0;
    warningNumber = 0;

    loggerFile.open("txt/logger.txt", std::ios::out);
    if(!loggerFile.is_open())
        std::cout << "Unable to open logger file" << std::endl;
    outStream << "Logger for Galaga Game" << std::endl << std::endl;

}

Logger::Logger(outputType outType): out(outType){
        errorNumber = 0;
        infoNumber = 0;
        warningNumber = 0;

        loggerFile.open("txt/logger.txt", std::ios::out);
        if(!loggerFile.is_open())
            std::cout << "Unable to open logger file" << std::endl;
        outStream << "Logger for Galaga Game" << std::endl << std::endl;

}
Logger::~Logger()
{
    outStream << std::endl << std::endl;
    outStream << "Warnings: " << warningNumber << std::endl;
    outStream << "Errors: " << errorNumber << std::endl << "Number of info: " << infoNumber << std::endl;

    loggerFile.close();
}

void Logger::appendInfoNumber(severity sev){
    if(sev == severity::error)
        errorNumber++;
    else if(sev == severity::warning)
        warningNumber++;
    else
        infoNumber++;
}

std::string Logger::getStringFromEnum(severity sev){
    if(sev == severity::error)
        return "ERROR";
    else if(sev == severity::warning)
        return "WARNING";
    else
        return "INFO";
}

