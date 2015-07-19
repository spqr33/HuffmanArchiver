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
#include <cassert>

namespace LobKo {

    Compressor::Compressor() {
    }

    Compressor::~Compressor() {
    }

    void Compressor::operator()(std::queue<spRawPage>& raw_pages_queue, std::mutex& mutex_reading_queue, bool& reading_done, std::mutex& mutex_reading_done,
            CompressedDataPriorityQueue& ready_for_write_data_priority_queue, std::mutex& mutex_ready_for_write_data_priority_queue) const {
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
#ifndef NDEBUG
                std::cout << "Thread " << std::this_thread::get_id() << " got page # " << sp_raw_page->page_number();
                std::cout << "Page size: " << sp_raw_page->size() << std::endl;
#endif
                mutex_reading_queue.unlock();

                spHammanData sp_hamman_data(new HammanData(*(sp_raw_page.get())));

                spCompressedData sp_compressed_page(new CompressedData(sp_raw_page->page_number()));
                sp_compressed_page->set_hamman_data(sp_hamman_data);
                sp_compressed_page->set_bytes_compressed(sp_raw_page->size());

                spBitBunch sp_bit_bunch = sp_hamman_data->generate_bit_bunch(*(sp_raw_page.get()));
                sp_compressed_page->set_bit_bunch(sp_bit_bunch);

#ifndef NDEBUG
                assert(sp_compressed_page->get_bit_bunch().get() && "Zero Pointer");
                std::cout << "Thread " << std::this_thread::get_id() << " compressed size: "
                        << sp_compressed_page->get_bit_bunch()->get_size_in_bytes() << std::endl;
#endif

                mutex_ready_for_write_data_priority_queue.lock();
                ready_for_write_data_priority_queue.push(sp_compressed_page);
                mutex_ready_for_write_data_priority_queue.unlock();

            }
        }
    }
}
