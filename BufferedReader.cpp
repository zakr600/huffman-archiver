//
// Created by Захар on 04.10.2021.
//

#include "BufferedReader.h"

BufferedReader::BufferedReader(std::istream &stream): stream(stream) {}

BufferedReader::~BufferedReader() {
}

int BufferedReader::read1bit() {
    if (buffer.empty()) {
        int32_t c = static_cast<unsigned char>(stream.get());
        for (int i = 0; i < CHAR_SIZE; ++i) {
            buffer.push_back((c >> (CHAR_SIZE - i - 1)) & 1);
        }
    }
    int32_t ans = buffer.front();
    buffer.pop_front();
    return ans;
}

int BufferedReader::read_bits(size_t count) {
    int ans = 0;
    for (int i = 0; i < count; ++i) {
        ans += (1 << i) * read1bit();
    }
    return ans;
}