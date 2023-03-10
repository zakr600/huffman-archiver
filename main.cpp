#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "BufferedReader.h"
#include "BufferedWriter.h"
#include "BinHeap.h"
#include "Huffman.h"

const int FILENAME_END = 256;
const int ONE_MORE_FILE = 257;
const int ARCHIVE_END = 258;
const int BLOCK_LENGTH = 9;

void increment(std::vector<int> &a);

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        std::cout << "Usage:\n"
                     "\n"
                     "archiver -c archive_name file1 [file2 ...] - zip files fil1, "
                     "file2, ... and save the result to archive_name file.\n"
                     "\n"
                     "archiver -d archive_name - unzip files from the archive_name "
                     "archive and put them in the current directory. File names "
                     "must be preserved when zipping and unzipping.\n"
                     "\n"
                     "archiver -h - display help on using the program.\n";
        return 0;
    } else if (argc >= 4 && strcmp(argv[1], "-c") == 0) {
        std::ofstream fout(argv[2], std::fstream::binary);
        auto encoder = Huffman::Encoder(fout);

        for (int file_number = 3; file_number < argc; ++file_number) {
            encoder.ProcessFile(argv[file_number]);
        }
        return 0;
    } else if (argc == 3 && strcmp(argv[1], "-d") == 0) {
        char *archive = argv[2];
        std::ifstream fin(archive, std::fstream::binary);
        auto decoder = Huffman::Decoder(fin);
        while (decoder.has_next_file) {
            decoder.ProcessFile();
        }
        return 0;
    }
    std::cout
            << "Invalid command line arguments. Run archiver -h for more info\n";
    return 0;
}