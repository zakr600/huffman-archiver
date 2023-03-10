//
// Created by Захар on 13.10.2021.
//

#include "Huffman.h"
#include "BinHeap.h"
#include <map>




void increment(std::vector<int> &a) {
    for (size_t i = a.size() - 1; i >= 0; --i) {
        if (a[i] == 1) {
            a[i] = 0;
        } else {
            a[i] = 1;
            return;
        }
    }
}

Huffman::Encoder::Encoder(std::ofstream& stream): writer_(stream), first_file_(true) {}

Huffman::Encoder::~Encoder() {
    for (int i : codes_[ARCHIVE_END]) {
        writer_.write1bit(i);
    }
}

std::vector<int> Huffman::Encoder::buildCodesLengths(const std::string &file_name) {
    std::vector<int> cnt(ALPHABET);
    int len = file_name.size();
    for (int j = 0; j < len; ++j) {
        ++cnt[static_cast<unsigned char>(file_name[j])];
    }
    std::ifstream fin(file_name, std::fstream::binary);
    while (!fin.eof()) {
        ++cnt[static_cast<unsigned char>(fin.get())];
    }
    cnt[FILENAME_END] = 1;
    cnt[ONE_MORE_FILE] = 1;
    cnt[ARCHIVE_END] = 1;
//      std::multiset<std::pair<int, Node *>> st;
    BinHeap<std::pair<int, HuffmanTree *>> heap;
    for (size_t i = 0; i < ALPHABET; ++i) {
        if (cnt[i] > 0) {
            heap.push_back(std::make_pair(cnt[i], new HuffmanTree(i)));
        }
    }
    while (heap.size() >= 2) {

        auto left_child = heap.extract_min();
        auto right_child = heap.extract_min();
        size_t new_size = left_child.first + right_child.first;
        HuffmanTree *root = new HuffmanTree(left_child.second, right_child.second);
        delete left_child.second;
        delete right_child.second;
        heap.push_back(std::make_pair(new_size, root));
    }
    HuffmanTree root = *(heap.extract_min().second);
    std::vector<int> lengths = root.getLengths();
    return lengths;
}

void Huffman::Encoder::buildCanonicalHuffmanCodes(const std::string &file_name, const std::vector<int>& lengths) {
    codes_.assign(ALPHABET, {});
    std::vector<std::pair<int, int>> codes_helper;
    for (int i = 0; i < ALPHABET; ++i) {
        if (lengths[i] != -1) {
            codes_helper.emplace_back(lengths[i], i);
        }
    }
    sort(codes_helper.begin(), codes_helper.end());
    std::vector<int> last_code;
    for (auto[len, c] : codes_helper) {
        if (!last_code.empty()) {
            increment(last_code);
        }
        while (last_code.size() < len) {
            last_code.push_back(0);
        }
        codes_[c] = last_code;
    }
}


void Huffman::Encoder::ProcessFile(const std::string &file_name) {
    if (!first_file_) {
        for (int i : codes_[ONE_MORE_FILE]) {
            writer_.write1bit(i);
        }
    }
    first_file_ = false;
    auto lengths = buildCodesLengths(file_name);
    buildCanonicalHuffmanCodes(file_name, lengths);
    int symbols_count = 0;
    for (size_t i = 0; i < ALPHABET; ++i) {
        if (lengths[i] != -1) {
            ++symbols_count;
        }
    }
    int max_symbol_code_size = 0;
    for (int i = 0; i < ALPHABET; ++i) {
        if (lengths[i] > max_symbol_code_size) {
            max_symbol_code_size = lengths[i];
        }
    }
    writer_.write_bits(symbols_count, BLOCK_LENGTH);
    std::vector<int> count_with_length(max_symbol_code_size + 1);
    for (size_t i = 0; i < ALPHABET; ++i) {
        if (lengths[i] != -1) {
            ++count_with_length[codes_[i].size()];
        }
    }
    std::vector<std::pair<int, int>> codes_helper;
    for (int i = 0; i < ALPHABET; ++i) {
        if (lengths[i] != -1) {
            codes_helper.emplace_back(lengths[i], i);
        }
    }
    sort(codes_helper.begin(), codes_helper.end());
    for (auto[len, symbol] : codes_helper) {
        writer_.write_bits(symbol, BLOCK_LENGTH);
    }
    for (size_t i = 1; i <= max_symbol_code_size; ++i) {
        writer_.write_bits(count_with_length[i], BLOCK_LENGTH);
    }
    int name_len = file_name.size();
    for (int j = 0; j < name_len; ++j) {
        for (int i : codes_[static_cast<unsigned char>(file_name[j])]) {
            writer_.write1bit(i);
        }
    }
    for (int i : codes_[FILENAME_END]) {
        writer_.write1bit(i);
    }
    std::ifstream fin2(file_name, std::fstream::binary);
    while (!fin2.eof()) {
        unsigned char CCC = static_cast<unsigned char>(fin2.get());
        if (fin2.eof()) {
            break;
        }
        for (int i : codes_[CCC]) {
            writer_.write1bit(i);
        }
    }
}


Huffman::Decoder::Decoder(std::ifstream& stream): reader_(stream), has_next_file(true) {}

std::map<std::vector<int>, int> Huffman::Decoder::ReadCodes() {
    int symbols_count = reader_.read_bits(BLOCK_LENGTH);
    std::vector<int> alphabet;
    for (size_t i = 0; i < symbols_count; ++i) {
        alphabet.push_back(reader_.read_bits(BLOCK_LENGTH));
    }
    std::vector<int> count_codes_with_len(1);
    int now_symbols_count = 0;
    while (now_symbols_count < symbols_count) {
        int i = reader_.read_bits(BLOCK_LENGTH);
        count_codes_with_len.push_back(i);
        now_symbols_count += i;
    }
    std::map<std::vector<int>, int> codes;
    std::vector<std::pair<int, int>> codes_helper;
    int curr_length = 1;
    for (int i = 0; i < symbols_count; ++i) {
        while (count_codes_with_len[curr_length] == 0) {
            ++curr_length;
        }
        --count_codes_with_len[curr_length];
        codes_helper.emplace_back(curr_length, alphabet[i]);
    }

    std::vector<int> last_code;
    for (auto[len, c] : codes_helper) {
        if (!last_code.empty()) {
            increment(last_code);
        }
        while (last_code.size() < len) {
            last_code.push_back(0);
        }
        codes[last_code] = c;
    }
    return codes;
}

void Huffman::Decoder::ProcessFile() {
    auto codes = ReadCodes();
    std::string filename;
    std::vector<int> pref;
    for (int i = 0; true; ++i) {
        pref.push_back(reader_.read1bit());
        if (codes.find(pref) != codes.end()) {
            auto it = codes.find(pref);
            if (it->second == FILENAME_END) {
                break;
            }
            filename += static_cast<unsigned char>(it->second);
            pref.clear();
        }
    }
    std::fstream fout(filename, std::fstream::out | std::fstream::binary);
    pref.clear();
    for (int i = 0; true; ++i) {
        pref.push_back(reader_.read1bit());
        if (codes.find(pref) != codes.end()) {
            if (codes[pref] == ONE_MORE_FILE) {
                has_next_file = true;
                return;
            } else if (codes[pref] == ARCHIVE_END) {
                has_next_file = false;
                return;
            }
            fout.put(static_cast<unsigned char>(codes[pref]));
            pref.clear();
        }
    }
}

Huffman::HuffmanTree::HuffmanTree(): root_(new Node(-1)) {

}

Huffman::HuffmanTree::HuffmanTree(int symbol): root_(new Node(symbol)) {

}

Huffman::HuffmanTree::~HuffmanTree() {
    Delete(root_);
}

Huffman::HuffmanTree::HuffmanTree(Huffman::HuffmanTree *left, Huffman::HuffmanTree *right) {
    root_ = new Node(left->root_, right->root_);
    left->root_ = nullptr;
    right->root_ = nullptr;
}

void Huffman::HuffmanTree::Delete(Huffman::HuffmanTree::Node *node) {
    if (node != nullptr) {
        Delete(node->left_son);
        Delete(node->right_son);
        delete node;
    }
}

std::vector<int> Huffman::HuffmanTree::getLengths() {
    std::vector<int> lengths(ALPHABET, -1);
    dfs(root_, lengths, 0);
    return lengths;
}

void Huffman::HuffmanTree::dfs(Huffman::HuffmanTree::Node *root, std::vector<int>& lengths, int curr_length) {
    if (root == nullptr) {
        return;
    }
    if (root->left_son != nullptr) {
        dfs(root->left_son, lengths, curr_length + 1);
    }
    if (root->right_son != nullptr) {
        dfs(root->right_son, lengths, curr_length + 1);
    }
    if (root->left_son == nullptr && root->right_son == nullptr) {
        lengths[root->symbol] = curr_length;
    }
}


Huffman::HuffmanTree::Node::Node(int symbol) : symbol(symbol), left_son(nullptr), right_son(nullptr) {
}

Huffman::HuffmanTree::Node::Node(Huffman::HuffmanTree::Node *left_son, Huffman::HuffmanTree::Node *right_son): symbol(-1), left_son(left_son), right_son(right_son) {

}

Huffman::HuffmanTree::Node::Node(): symbol(-1), left_son(nullptr), right_son(nullptr) {

}
