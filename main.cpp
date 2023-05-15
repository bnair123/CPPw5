#include <iostream>
#include "fileProc.h"
#include "logger.h"

int main() {
    logging::NamedFile out("output.txt");

    out << 10.55 << ' ' << 'A' << ' ' << "This is a line \n";
    out << true << "\n";
    out << "I enjoy driving\n";
    out << "The zoo is cool\n";

    std::cout << std::endl;

    logger::get().log("CPP is not very fun");

    return 0;
}
