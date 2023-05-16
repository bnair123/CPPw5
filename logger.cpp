//
// Created by bnair on 28/04/2023.
//

#include "logger.h"
#include <ctime>
#include <ostream>

logger& logger::get() {
    static logger instance;
    return instance;
}

void logger::log(const std::string& msg) {
    time_t rawtime;
    struct tm* timeinfo;
    char time_buffer[80];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(time_buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    out_ << time_buffer << ": " << msg << '\n';
    out_.flush();
}

logger::logger() : out_("output2.txt") {}