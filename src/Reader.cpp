/* 
 * File:   Reader.cpp
 * Author: s
 * 
 * Created on July 19, 2015, 11:57 PM
 */

#include "Reader.h"
#include <cstdlib> //exit()
#include <iostream>

namespace LobKo {

    Reader::Reader(std::ifstream& file_in) :
    file_in_(file_in),
    reading_counter_(0) {
        file_in_.seekg(0, file_in_.end);
        file_size_ = file_in_.tellg();
        file_in_.seekg(0, file_in_.beg);
    }

    Reader::~Reader() {
    }

    void Reader::operator()(std::queue<spRawPage>& raw_pages_queue, std::mutex& mutex_reading_queue, bool& reading_done, std::mutex& mutex_reading_done, uint64_t& last_page_num) const {
        uint32_t actual_read_bytes;
        bool can_read = true;

        do {
            can_read = true;

            mutex_reading_queue.lock();
            if ( raw_pages_queue.size() >= 40 ) {
                can_read = false;
            }
            mutex_reading_queue.unlock();

            if ( can_read == true ) {
                uint8_t* buffer = new uint8_t[RawPage::MAX_RAW_PAGE_SIZE];
                file_in_.read((char*) buffer, RawPage::MAX_RAW_PAGE_SIZE);
                ++reading_counter_;

                actual_read_bytes = file_in_.gcount();
                spRawPage sp_raw_page(new RawPage(buffer, actual_read_bytes, reading_counter_));

                mutex_reading_queue.lock();
                raw_pages_queue.push(sp_raw_page);
                //#ifndef NDEBUG
                std::cout << "READING COUNTER == " << reading_counter_ <<
                        " READING QUEUE SIZE == " << raw_pages_queue.size() << std::endl;
                //#endif
                mutex_reading_queue.unlock();
            }

        } while (file_in_.good());

        if ( file_in_.bad() ) {
#ifndef NDEBUG
            std::cout << "I/O error while reading\n";
#endif
            //TODO clean up
            exit(EXIT_FAILURE);
        } else if ( file_in_.eof() ) {
#ifndef NDEBUG
            std::cout << "End of file reached successfully\n";
#endif
            std::lock_guard<std::mutex> lock(mutex_reading_done);
            last_page_num = reading_counter_;
            reading_done = true;
        }
    }

    void Reader::reset_reading_counter() {
        reading_counter_ = 0;
    }
}