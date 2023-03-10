//
// Created by Zakhar on 04.10.2021.
//

#ifndef ARCHIVER_BUFFEREDREADER_H
#define ARCHIVER_BUFFEREDREADER_H

#include <fstream>
#include <deque>

class BufferedReader {
private:
    std::istream &stream;
    std::deque<int> buffer;
    const int CHAR_SIZE = 8;

public:
    BufferedReader(std::istream &stream);

    ~BufferedReader();

    int read1bit();

    int read_bits(size_t count);
};


#endif //ARCHIVER_BUFFEREDREADER_H
