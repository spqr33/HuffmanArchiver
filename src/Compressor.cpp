/* 
 * File:   Compressor.cpp
 * Author: s
 * 
 * Created on July 18, 2015, 12:50 PM
 */

#include <thread>
#include "Compressor.h"
#include "HammanData.h"
#include "CompressedData.h"

namespace LobKo {

    Compressor::Compressor() {
    }

    Compressor::~Compressor() {
    }

    void Compressor::operator()(std::queue<spRawPage>& raw_pages_queue, std::mutex& mutex_reading_queue, bool& reading_done, std::mutex& mutex_reading_done) const {
        //bool done;
        while (true) {
            mutex_reading_queue.lock();
            if ( raw_pages_queue.empty() ) {
                mutex_reading_done.lock();
                if ( reading_done == true ) {
                    mutex_reading_queue.unlock();
                    mutex_reading_done.unlock();
                    break;
                }

                mutex_reading_queue.unlock();
                mutex_reading_done.unlock();

                std::this_thread::yield();
                continue;
            } else {
                spRawPage sp_raw_page = raw_pages_queue.front();
                raw_pages_queue.pop();

                std::cout << "Thread " << std::this_thread::get_id() << " got page # " << sp_raw_page->page_number();
                std::cout << "Page size: " << sp_raw_page->size() << std::endl;
                mutex_reading_queue.unlock();

                spHammanData sp_hamman_data(new HammanData(*(sp_raw_page.get())));
            }
        }
    }
}
