/* 
 * File:   Writer.cpp
 * Author: s
 * 
 * Created on July 19, 2015, 4:31 PM
 */

#include "Writer.h"
#include <iostream>

namespace LobKo {

    //    Writer::Writer(CompressedDataPriorityQueue& ready_for_write_data_priority_queue, std::mutex& mutex_ready_for_write_data_priority_queue) :
    //    ready_for_write_data_priority_queue_(ready_for_write_data_priority_queue),
    //    mutex_ready_for_write_data_priority_queue_(mutex_ready_for_write_data_priority_queue) {
    //        ;
    //    }

    Writer::Writer(uint64_t last_page_num) :
    wait_page_num_(1),
    last_page_num_(last_page_num) {
    }

    Writer::~Writer() {
    }

    void Writer::operator()(CompressedDataPriorityQueue& ready_for_write_data_priority_queue, std::mutex& mutex_ready_for_write_data_priority_queue,
            bool& writing_done, std::mutex& mutex_writing_done) {
        while (true) {
            mutex_ready_for_write_data_priority_queue.lock();
            if ( !ready_for_write_data_priority_queue.empty() ) {
                if ( wait_page_num_ == ready_for_write_data_priority_queue.top()-> page_number() ) {

                    spCompressedData sp_compressed_data(ready_for_write_data_priority_queue.top());
                    ready_for_write_data_priority_queue.pop();
                    std::cout << "Ready for write page# " << sp_compressed_data->page_number() << " from " << last_page_num_<< std::endl;
                    mutex_ready_for_write_data_priority_queue.unlock();

                    //writing data
                    if ( wait_page_num_ == last_page_num_ ) {
                        mutex_writing_done.lock();
                        writing_done = true;
                        mutex_writing_done.unlock();
                        
                        std::cout << "Compressor is exiting\n";
                        break;
                    }
                    ++wait_page_num_;
                }
            }
            mutex_ready_for_write_data_priority_queue.unlock();

        }
    }
}