//
// Created by bnair on 28/04/2023.
//

#ifndef W5_LOGGER_H
#define W5_LOGGER_H

#include "fileProc.h"
#include <ctime>
#include <ostream>

class logger {
public:
    static logger& get();
    void log(const std::string& msg);

private:
    logger();
    logging::NamedFile out_;
};


#endif  //W5_CPP_LOGGER_H
