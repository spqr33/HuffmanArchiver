/* 
 * File:   Writer.cpp
 * Author: s
 * 
 * Created on July 19, 2015, 4:31 PM
 */

#include "Writer.h"
#include <iostream>

namespace LobKo {

    Writer::Writer(CompressedDataPriorityQueue& ready_for_write_data_priority_queue, std::mutex& mutex_ready_for_write_data_priority_queue) :
    ready_for_write_data_priority_queue_(ready_for_write_data_priority_queue),
    mutex_ready_for_write_data_priority_queue_(mutex_ready_for_write_data_priority_queue) {
        ;
    }

    Writer::~Writer() {
        ;
    }

    void Writer::operator()() const {
        mutex_ready_for_write_data_priority_queue_.lock();
        spCompressedData sp_compressed_data(ready_for_write_data_priority_queue_.top());
        ready_for_write_data_priority_queue_.pop();
        mutex_ready_for_write_data_priority_queue_.unlock();
        
        std::cout<< "Ready for write page# " << sp_compressed_data->page_number() << std::endl;
        ;
    }
}