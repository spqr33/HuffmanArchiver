/* 
 * File:   main.cpp
 * Author: s
 *
 * Created on July 6, 2015, 10:04 PM
 */

#include <cstdlib>
#include "BitBunch.h"
#include "RawPage.h"
#include "HammanData.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>
#include <functional> //std::ref
#include "Compressor.h"
#include "CompressedData.h"
#include "Writer.h"

using std::queue;
using std::string;
using std::ifstream;
using std::ofstream;
using std::mutex;
using std::thread;
using std::cerr;
using std::cout;
using std::ios_base;
using std::vector;
using std::ref;
using namespace LobKo;

/*
 * 
 */
int main(int argc, char** argv) {
    {
        string input_file_name;
        string output_file_name;
        if ( argc < 2 ) {
            input_file_name = "test_file";
        } else {
            input_file_name = argv[1];
        }
        output_file_name = input_file_name + ".processed";
        ifstream in(input_file_name.c_str(), ios_base::binary);
        ofstream out(output_file_name.c_str(), ios_base::binary | ios_base::out | ios_base::trunc);
        if ( !in.is_open() || !out.is_open() ) {
            cerr << "Error while opening file\n";
            return 1;
        }

        uint32_t real_cores = thread::hardware_concurrency();
        uint32_t reading_counter = 0;
        uint32_t actual_read_bytes;
        queue<spRawPage> raw_pages_queue;
        mutex mutex_reading_queue;

        bool reading_done = false;
        mutex mutex_reading_done;

        typedef std::priority_queue<spCompressedData, std::vector<spCompressedData>, std::function<bool(spCompressedData, spCompressedData) >> CompressedDataPriorityQueue;
        CompressedDataPriorityQueue ready_for_write_data_priority_queue(compressed_data_copmarision);
        mutex mutex_ready_for_write_data_priority_queue;

        //mutex_reading_queue.lock();
        std::function<void(queue<spRawPage>&, mutex&, bool&, mutex&, CompressedDataPriorityQueue&, mutex&) > worker = Compressor();
        thread w1(worker, std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue));
        w1.detach();

        thread w2(Compressor(), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue));
        w2.detach();
        thread w3(Compressor(), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue));
        w3.detach();
        thread w4(Compressor(), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue));
        w4.detach();

        //thread writer(Writer(std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue)));
        //writer.detach();

        do {
            uint8_t* buffer = new uint8_t[RawPage::MAX_RAW_PAGE_SIZE];

            in.read((char*) buffer, RawPage::MAX_RAW_PAGE_SIZE);
            ++reading_counter;

            actual_read_bytes = in.gcount();
            spRawPage sp_raw_page(new RawPage(buffer, actual_read_bytes, reading_counter));

            mutex_reading_queue.lock();
            raw_pages_queue.push(sp_raw_page);
            mutex_reading_queue.unlock();

            //out.write((char*) sp_raw_page->buffer(), sp_raw_page->size());


        } while (in.good());

        if ( in.bad() ) {
            std::cout << "I/O error while reading\n";
            return 1;
        } else if ( in.eof() ) {
            std::cout << "End of file reached successfully\n";
            std::lock_guard<std::mutex> lock(mutex_reading_done);
            reading_done = true;
        }

        std::cin.get();
        std::cout << "Main is exiting\n";
        //HammanData hd(page1);
    }
    return 0;
}

