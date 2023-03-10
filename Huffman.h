//
// Created by Захар on 13.10.2021.
//

#ifndef ARCHIVER_HUFFMAN_H
#define ARCHIVER_HUFFMAN_H

#include "BufferedReader.h"
#include "BufferedWriter.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <ostream>

using Code = std::vector<int>;

namespace Huffman {
    static const int FILENAME_END = 256;
    static const int ONE_MORE_FILE = 257;
    static const int ARCHIVE_END = 258;
    static const size_t ALPHABET = 259;
    static const int BLOCK_LENGTH = 9;

    class Encoder {
    public:
        explicit Encoder(std::ofstream &stream);

        ~Encoder();

        void ProcessFile(const std::string &file_name);

    private:
        std::vector<int> buildCodesLengths(const std::string &file_name);

        void buildCanonicalHuffmanCodes(const std::string &file_name, const std::vector<int> &lengths);


        BufferedWriter writer_;
        bool first_file_;
        std::vector<std::vector<int>> codes_;
    };

    class Decoder {
    public:
        explicit Decoder(std::ifstream &stream);

        void ProcessFile();

        bool has_next_file;

    private:
        std::map<std::vector<int>, int> ReadCodes();

        BufferedReader reader_;
    };

    class HuffmanTree {
    public:
        HuffmanTree();
        HuffmanTree(int symbol);
        HuffmanTree(HuffmanTree* left, HuffmanTree *right);
        ~HuffmanTree();
        std::vector<int> getLengths();

    private:
        static const size_t ALPHABET = 259;
        class Node {
        public:
            Node(int symbol);
            Node(Node* left_son, Node* right_son);
            Node();

            Node *left_son;
            Node *right_son;
            int symbol;
        };
        void Delete(Node* node);
        Node* root_;
    private:
        void dfs(Node* root, std::vector<int>& lengths, int curr_length);
    };
}

#endif //ARCHIVER_HUFFMAN_H
