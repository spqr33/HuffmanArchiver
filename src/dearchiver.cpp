/* 
 * File:   newmain.cpp
 * Author: s
 *
 * Created on July 29, 2015, 11:57 AM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include "CompressedData.h"
#include "CompressedFileReader.h"
#include "Decompression.h"
#include "Decompressor.h"
#include "DecompressedPageWriter.h"
#include "HammanData.h"
#include <mutex>
#include <thread>
#include <condition_variable>


using std::string;
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios_base;
using std::queue;
using namespace LobKo;
using std::mutex;
using std::thread;
using std::lock_guard;
using std::unique_lock;

/*
 * 
 */
int main(int argc, char** argv) {
    string compressed_file_name;
    string decompressed_file_name;
    const string archive_extention = ".processed";

    if ( argc < 2 ) {
        compressed_file_name = "test_file" + archive_extention;
    } else {
        compressed_file_name = argv[1];
    }

    std::size_t found = compressed_file_name.find(archive_extention);
    if ( found == std::string::npos ) {
        cerr << "There is no file with " + archive_extention + " extention\n";

        return EXIT_FAILURE;
    } else {
        decompressed_file_name = string(compressed_file_name, 0, found);

        cout << "Decompressed file name is " << decompressed_file_name << ':' << endl;
    }
    /////////////////////////////////////////////////////////////////////Opening
    ifstream compressed_file(compressed_file_name.c_str(), ios_base::binary);
    //ifstream compressed_file("/home/s/Projects/My/c++/Archiver/dist/Dearchive_Debug/GNU_gcc4.8.4-Linux-x86/test_file.processed", ios_base::binary);
    ofstream decompressed_file(decompressed_file_name.c_str(), ios_base::binary | ios_base::out | ios_base::trunc);
    if ( !compressed_file.is_open() ) {
        cerr << "Error while opening " << compressed_file_name << "\n";
        return EXIT_FAILURE;
    }
    if ( !decompressed_file.is_open() ) {

        cerr << "Error while opening " << decompressed_file_name << "\n";
        return EXIT_FAILURE;
    }
    //////////////////////////////////////////////////////////////////Decompress
    CompressedFileReader reader(compressed_file);
    Decompression decompression;

    cerr << "Starting Decompression\n";
    // thread r(reader, std::ref(compressed_data_q), std::ref(mutex_compressed_data_q), std::ref(reading_done), std::ref(mutex_reading_done));
    thread r(reader, std::ref(decompression));


    thread dc1(Decompressor(decompression), std::ref(decompression));
//    thread dc2(Decompressor(decompression), std::ref(decompression));
//    thread dc3(Decompressor(decompression), std::ref(decompression));
//    thread dc4(Decompressor(decompression), std::ref(decompression));
//    thread dc5(Decompressor(decompression), std::ref(decompression));
//    thread dc6(Decompressor(decompression), std::ref(decompression));
//    thread dc7(Decompressor(decompression), std::ref(decompression));
//    thread dc8(Decompressor(decompression), std::ref(decompression));
    {
        unique_lock<mutex> lk(decompression.mutex_last_page_num_);
        while (!decompression.last_page_num_calculated_) {
            decompression.cv_last_page_num_.wait(lk);
        }
    }
    thread w(DecompressedPageWriter(decompression.last_page_num_, decompressed_file), std::ref(decompression));

    r.join();
    dc1.join();
//    dc2.join();
//    dc3.join();
//    dc4.join();
//    dc5.join();
//    dc6.join();
//    dc7.join();
//    dc8.join();
    w.join();

    compressed_file.clear();
    decompressed_file.clear();
    compressed_file.close();
    decompressed_file.close();
    return 0;
}

