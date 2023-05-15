//
// Created by bnair on 27/04/2023.
//

#ifndef W5_CPP_FILEPROC_H
#define W5_CPP_FILEPROC_H
#include <cstdio>
#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>


namespace logging {

    class NamedFile {
    public:
        NamedFile(const std::string& file_name);

        NamedFile();

        ~NamedFile() {
            close();
        }

        void open(const std::string& file_name);

        // Closes the file
        void close();

        NamedFile& operator<<(const std::string& value);

        NamedFile& operator<<(const char *value);

        NamedFile& operator<<(char value);

        NamedFile& operator<<(int number);

        NamedFile& operator<<(double number);

        NamedFile& operator<<(bool value);

        bool is_open() const;

        bool error() const;

        bool eof() const;

        int lines() const;

        int size() const;

        std::string name() const;

    private:
        std::string file_name;
        FILE *file;
        int line_counter = 0;
        int byte_counter = 0;

    };

}
#endif //W5_CPP_FILEPROC_H
