/* 
 * File:   DecomressedPageWriter.cpp
 * Author: s
 * 
 * Created on July 31, 2015, 8:50 PM
 */

#include "DecompressedPageWriter.h"
#include "Decompression.h"
#include <fstream>

namespace LobKo {

    DecompressedPageWriter::DecompressedPageWriter(uint64_t last_page_num, std::ofstream& file_out) :
    wait_page_num_(1),
    last_page_num_(last_page_num),
    file_out_(file_out) {
    }

    DecompressedPageWriter::~DecompressedPageWriter() {
    }

    void DecompressedPageWriter::operator()(Decompression& decompression) {
        while (true) {
            decompression.mutex_ready_for_write_data_priority_q_.lock();
            if ( !decompression.ready_for_write_data_priority_q_.empty() ) {
                if ( wait_page_num_ == decompression.ready_for_write_data_priority_q_.top()->page_number() ) {
                    spRawPage sp_raw_page(decompression.ready_for_write_data_priority_q_.top());
                    decompression.ready_for_write_data_priority_q_.pop();

                    std::cout << "Ready for write page# " << sp_raw_page->page_number() << " from " << last_page_num_ << std::endl;
                    decompression.mutex_ready_for_write_data_priority_q_.unlock();

                    //writing
                    uint8_t* buffer = sp_raw_page->buffer();
                    file_out_.write(reinterpret_cast<const char*> (buffer), sp_raw_page->size());
                    if ( wait_page_num_ == last_page_num_ ) {
                        decompression.mutex_writing_done_.lock();
                        decompression.writing_done_ = true;
                        decompression.mutex_writing_done_.unlock();

                        break;
                    }
                    ++wait_page_num_;
                }
            }
            decompression.mutex_ready_for_write_data_priority_q_.unlock();
        }
    }
}