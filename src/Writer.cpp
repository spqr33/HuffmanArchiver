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

    Writer::Writer() {
        ;
    }

    Writer::~Writer() {
        ;
    }

    void Writer::operator()(CompressedDataPriorityQueue& ready_for_write_data_priority_queue, std::mutex& mutex_ready_for_write_data_priority_queue) const {
        while (true) {
            mutex_ready_for_write_data_priority_queue.lock();
            if ( !ready_for_write_data_priority_queue.empty() ) {
                spCompressedData sp_compressed_data(ready_for_write_data_priority_queue.top());

                ready_for_write_data_priority_queue.pop();

                std::cout << "Ready for write page# " << sp_compressed_data->page_number() << std::endl;
                mutex_ready_for_write_data_priority_queue.unlock();
                //writing data


            }
            mutex_ready_for_write_data_priority_queue.unlock();

        }
    }
}