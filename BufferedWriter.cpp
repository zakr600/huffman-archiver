//
// Created by Захар on 04.10.2021.
//

#include "BufferedWriter.h"

BufferedWriter::BufferedWriter(std::ostream &stream): stream(stream) {}

BufferedWriter::~BufferedWriter() {
    while (buffer.size() > 0) {
        char now = 0;
        for (int i = 0; i < CHAR_SIZE && !buffer.empty(); ++i) {
            now += (1 << (CHAR_SIZE - i - 1)) * buffer.front();
            buffer.pop_front();
        }
        stream.put(now);
    }
}

void BufferedWriter::write1bit(int n) {
    buffer.push_back(n);
    while (buffer.size() >= CHAR_SIZE) {
        unsigned char now = 0;
        for (int i = 0; i < CHAR_SIZE; ++i) {
            now = 2 * now + buffer.front();
            buffer.pop_front();
        }
        stream.put(now);
    }
}

void BufferedWriter::write_bits(int n, size_t count) {
    for (int i = 0; i < count; ++i) {
        write1bit(n % 2);
        n /= 2;
    }
}