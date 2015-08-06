/* 
 * File:   Decompressor.cpp
 * Author: s
 * 
 * Created on July 31, 2015, 11:42 AM
 */

#include "Decompressor.h"
#include "CompressedData.h"
#include "HammanTreeNode.h"
#include "Decompression.h"
#include "HammanData.h"
#include <thread>
#include <iostream>

namespace LobKo {

    Decompressor::Decompressor(Decompression& decompression) {
        std::unique_lock<std::mutex> lk(decompression.mutex_header_);
        while (!decompression.header_calculated_) {
            decompression.cv_header_.wait(lk);
        }

        sp_hamman_data_ = spHammanData(new HammanData(decompression.header_));
    }

    Decompressor::~Decompressor() {
    }

    void Decompressor::operator()(Decompression& decompression) {
        while (true) {
            decompression.mutex_compressed_data_q_.lock();
            if ( decompression.compressed_data_q_.empty() ) {
                
                decompression.mutex_reading_done_.lock();
                if ( decompression.reading_done_ == true ) {
                    decompression.mutex_compressed_data_q_.unlock();
                    decompression.mutex_reading_done_.unlock();

                    break;
                }
                decompression.mutex_compressed_data_q_.unlock();
                decompression.mutex_reading_done_.unlock();

                std::this_thread::yield();
                continue;
            } else {
                std::cout << "[Decompressor] Thread # " << std::this_thread::get_id() << " Decompression started" << std::endl;
                std::cout << "[Decompressor] Thread # " << std::this_thread::get_id() << " Compressed Q: " << decompression.compressed_data_q_.size() << std::endl;
                spCompressedData sp_compressed_data = decompression.compressed_data_q_.front();

                decompression.compressed_data_q_.pop();
                decompression.mutex_compressed_data_q_.unlock();

                uint32_t bytes_compressed = sp_compressed_data->get_bytes_compressed();
                spRawPage sp_raw_page(new RawPage(bytes_compressed,
                        sp_compressed_data->page_number()));

                uint8_t* decompressed_buffer = sp_raw_page->buffer();

                spHammanTreeNode sp_root = sp_hamman_data_->get_hamman_tree_root();
                HammanTreeNode* root = sp_root.get();
                BitBunch& bit_bunch = *sp_compressed_data->get_bit_bunch().get();

                std::cout << "[Decompressor] Thread # " << std::this_thread::get_id() << " Bytes_compressed: " << bytes_compressed << std::endl;
                uint32_t current_bit = 0;
                HammanTreeNode* current_node;
                for ( uint32_t i = 0; i < bytes_compressed; ++i ) {
                    current_node = root;

                    while (!current_node->is_leaf()) {
                        //while (!sp_current_node.get()->is_leaf()) {
                        if ( bit_bunch[current_bit] == BitBunch::bit_state::ZERO ) {
                            //std::cout << " We are going to the left\n";
                            //sp_current_node = sp_current_node->get_left_son();
                            current_node = current_node->get_left_son().get();
                        } else {
                            //sp_current_node = sp_current_node->get_right_son();
                            current_node = current_node->get_right_son().get();
                            //std::cout << " We are going to the right\n";
                        }
                        ++current_bit;
                    }
                    *(decompressed_buffer + i) = current_node->get_character();
                }
                std::cout << "[Decompressor] Thread # " << std::this_thread::get_id() << "Page# " << sp_raw_page->page_number() << "is ready for writing"<< std::endl;

                decompression.mutex_ready_for_write_data_priority_q_.lock();
                decompression.ready_for_write_data_priority_q_.push(sp_raw_page);
                decompression.mutex_ready_for_write_data_priority_q_.unlock();
            }
        }
    }
}