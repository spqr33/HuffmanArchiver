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
#include <array>
#include <functional> //std::ref
#include "Compressor.h"
#include "CompressedData.h"
#include "Writer.h"
#include "FrequencyCounter.h"
#include "Reader.h"

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
using std::array;
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
        if ( !in.is_open() ) {

            cerr << "Error while opening " << input_file_name << "\n";
            return 1;
        }
        if ( !out.is_open() ) {

            cerr << "Error while opening " << output_file_name << "\n";
            return 1;
        }

        uint32_t real_cores = thread::hardware_concurrency();
        //uint32_t reading_counter = 0;
        //uint32_t actual_read_bytes;
        uint64_t last_page_num = 0;

        queue<spRawPage> raw_pages_queue;
        mutex mutex_reading_queue;

        bool reading_done = false;
        mutex mutex_reading_done;

        array<uint32_t, 256> result_character_frequency({0});
        Reader reader(in);
        {
            mutex mutex_result_character_frequency;
            thread r(reader, std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done), std::ref(last_page_num));
            thread fc1(FrequencyCounter(result_character_frequency, mutex_result_character_frequency), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done));
            thread fc2(FrequencyCounter(result_character_frequency, mutex_result_character_frequency), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done));
            thread fc3(FrequencyCounter(result_character_frequency, mutex_result_character_frequency), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done));
            thread fc4(FrequencyCounter(result_character_frequency, mutex_result_character_frequency), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done));

            r.join();
            fc1.join();
            fc2.join();
            fc3.join();
            fc4.join();

            uint64_t summ = 0;
            for ( uint32_t ch = 0; ch < 256; ++ch ) {
                //std::cout << "Char: " << ch << " frequency: " << result_character_frequency[ch] << std::endl;
                summ += result_character_frequency[ch];
            }
            std::cout << "Summ: " << summ << std::endl;
            std::cout << "FileSize: " << reader.get_file_size() << std::endl;
        }

        spHammanData sp_hamman_data(new HammanData(result_character_frequency));

        /////////////////////////////////////////////////////////////Compressing
        typedef std::priority_queue<spCompressedData, std::vector<spCompressedData>, std::function<bool(spCompressedData, spCompressedData) >> CompressedDataPriorityQueue;
        CompressedDataPriorityQueue ready_for_write_data_priority_queue(compressed_data_copmarision);
        mutex mutex_ready_for_write_data_priority_queue;

        bool writing_done = false;
        mutex mutex_writing_done;

        {
            in.clear(); // in.failbit was set on last reading.
            reader.reset_reading_counter();
            reader.reset_seek();
            std::cout << "-=-=-=-=-=-= Main, reset seek" << in.tellg() << std::endl;
            reading_done = false;

            //Reader reader2(in);
            thread r(reader, std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done), std::ref(last_page_num));

            std::function<void(queue<spRawPage>&, mutex&, bool&, mutex&, CompressedDataPriorityQueue&, mutex&, bool&, mutex&) > worker = Compressor(sp_hamman_data);
            thread w1(worker, std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                    std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue), std::ref(writing_done), std::ref(mutex_writing_done));

            thread w2{Compressor(sp_hamman_data), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue), std::ref(writing_done), std::ref(mutex_writing_done)};
            thread w3{Compressor(sp_hamman_data), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue), std::ref(writing_done), std::ref(mutex_writing_done)};
            thread w4{Compressor(sp_hamman_data), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue), std::ref(writing_done), std::ref(mutex_writing_done)};
            thread w5{Compressor(sp_hamman_data), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue), std::ref(writing_done), std::ref(mutex_writing_done)};
            thread w6(Compressor(sp_hamman_data), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                    std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue), std::ref(writing_done), std::ref(mutex_writing_done));
            thread w7(Compressor(sp_hamman_data), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                    std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue), std::ref(writing_done), std::ref(mutex_writing_done));
            thread w8(Compressor(sp_hamman_data), std::ref(raw_pages_queue), std::ref(mutex_reading_queue), std::ref(reading_done), std::ref(mutex_reading_done),
                    std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue), std::ref(writing_done), std::ref(mutex_writing_done));


            thread writer(Writer(last_page_num, out, result_character_frequency), std::ref(ready_for_write_data_priority_queue), std::ref(mutex_ready_for_write_data_priority_queue),
                    std::ref(writing_done), std::ref(mutex_writing_done));

            r.join();
            w1.join();
            w2.join();
            w3.join();
            w4.join();
            w5.join();
            w6.join();
            w7.join();
            w8.join();
            writer.join();
        }

        std::cin.get();
        std::cout << "Main is exiting\n";
        //HammanData hd(page1);
    }
    return 0;
}
