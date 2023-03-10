//
// Created by Zakhar on 04.10.2021.
//

#ifndef ARCHIVER_BUFFEREDWRITER_H
#define ARCHIVER_BUFFEREDWRITER_H

#include <fstream>
#include <deque>

class BufferedWriter {
private:
    std::ostream &stream;
    std::deque<int> buffer;
    const int CHAR_SIZE = 8;

public:
    BufferedWriter(std::ostream &stream);

    ~BufferedWriter();

    void write1bit(int n);

    void write_bits(int n, size_t count);
};


#endif //ARCHIVER_BUFFEREDWRITER_H
