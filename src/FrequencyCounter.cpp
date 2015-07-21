/* 
 * File:   FrequencyCounter.cpp
 * Author: s
 * 
 * Created on July 20, 2015, 1:08 PM
 */

#include "FrequencyCounter.h"
#include "RawPage.h"
#include <iostream>
#include <thread>

namespace LobKo {

    FrequencyCounter::FrequencyCounter(std::array< uint32_t, 256>& result_character_frequency, std::mutex& mutex_result_character_frequency) :
    sp_array_(new FrequencyCounter::Array()),
    result_character_frequency_(result_character_frequency),
    mutex_result_character_frequency_(mutex_result_character_frequency) {
#ifndef NDEBUG
        std::cout << "FrequencyCounter() was created" << std::endl;
#endif
    }

    FrequencyCounter::Array::Array() :
    character_frequency_({0}) {
    }

    FrequencyCounter::~FrequencyCounter() {
#ifndef NDEBUG
        std::cout << "FrequencyCounter(const FrequencyCounter&) was called" << std::endl;
#endif
    }

    const FrequencyCounter::Array& FrequencyCounter::Array::operator+=(const FrequencyCounter::Array& rhs) {
        uint32_t size = character_frequency_.size();

        for ( uint32_t i = 0; i < size; ++i ) {
            character_frequency_[i] += rhs.character_frequency_[i];
        }
        return *this;
    }

    void FrequencyCounter::operator()(std::queue<spRawPage>& raw_pages_queue, std::mutex& mutex_reading_queue, bool& reading_done, std::mutex& mutex_reading_done) {
        std::unique_lock<std::mutex> lock_reading_queue(mutex_reading_queue, std::defer_lock);
        std::unique_lock<std::mutex> lock_reading_done(mutex_reading_done, std::defer_lock);

        while (true) {
            lock_reading_queue.lock();
            if ( raw_pages_queue.empty() ) {
                lock_reading_done.lock();
                if ( reading_done == true ) {
                    uint32_t size = sp_array_->character_frequency_.size();
                    std::array<uint32_t, 256>& character_frequency = sp_array_->character_frequency_;

                    mutex_result_character_frequency_.lock();
                    for ( uint32_t i = 0; i < size; ++i ) {
                        result_character_frequency_[i] += character_frequency[i];
                    }
                    mutex_result_character_frequency_.unlock();
                    break;
                }

                lock_reading_queue.unlock();
                lock_reading_done.unlock();
            } else {
                spRawPage sp_raw_page = raw_pages_queue.front();
                raw_pages_queue.pop();

#ifndef NDEBUG
                std::cout << "FrequencyCounter thread " << std::this_thread::get_id() << " got page # " << sp_raw_page->page_number();
                std::cout << "Page size: " << sp_raw_page->size() << std::endl;
#endif
                lock_reading_queue.unlock();

                const uint8_t * p_raw_page_buffer = sp_raw_page->buffer();
                const uint8_t * const p_raw_page_buffer_end = p_raw_page_buffer + sp_raw_page->size();

                std::array<uint32_t, 256>& character_frequency = sp_array_->character_frequency_;
                for (; p_raw_page_buffer != p_raw_page_buffer_end; ++p_raw_page_buffer ) {

                    ++(character_frequency[*p_raw_page_buffer]);
                }
            }
        }
    }
}