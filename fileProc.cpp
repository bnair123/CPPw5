#include "fileProc.h"

#include <cstdlib>



    logging::NamedFile::NamedFile(const std::string& file_name) : file_name(file_name), file(nullptr) {
        open(file_name);
    }

    void  logging::NamedFile::open(const std::string& file_name) {
        close();
        this->file_name = file_name;
        file = std::fopen(file_name.c_str(), "w");
    }

    void  logging::NamedFile::close() {
        if (file) {
            std::fclose(file);
            file = nullptr;
        }
    }

    logging::NamedFile& logging::NamedFile::operator<<(const std::string& value) {
        std::fprintf(file, "%s", value.c_str());
        return *this;
    }

    logging::NamedFile&  logging::NamedFile::operator<<(const char *value) {
        std::fprintf(file, "%s", value);
        return *this;
    }

    logging::NamedFile&  logging::NamedFile::operator<<(char value) {
        std::fprintf(file, "%c", value);
        return *this;
    }

    logging::NamedFile&  logging::NamedFile::operator<<(int number) {
        std::fprintf(file, "%d", number);
        return *this;
    }

    logging::NamedFile&  logging::NamedFile::operator<<(double number) {
        std::fprintf(file, "%f", number);
        return *this;
    }

    logging::NamedFile &logging::NamedFile::operator<<(bool value){
        if (value) {
            std::fprintf(file, "%s", "true");
        } else {
            std::fprintf(file, "%s", "false");
        }
        return *this;
    }

    bool  logging::NamedFile::is_open() const {
        return file != nullptr;
    }

    bool  logging::NamedFile::error() const {
        return std::ferror(file);
    }

    bool  logging::NamedFile::eof() const {
        return std::feof(file);
    }

    int  logging::NamedFile::lines() const {
        int lines = 0;
        char ch;
        while (std::fscanf(file, "%c", &ch) != EOF) {
            if (ch == '\n') {
                lines++;
            }
        }
        return lines;
    }

    int  logging::NamedFile::size() const {
        int size = 0;
        char ch;
        while (std::fscanf(file, "%c", &ch) != EOF) {
            size++;
        }
        return size;
    }

    std::string  logging::NamedFile::name() const {
        return file_name;
    }

