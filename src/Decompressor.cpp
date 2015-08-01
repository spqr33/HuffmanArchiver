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
#include <thread>

namespace LobKo {

    Decompressor::Decompressor() {
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
                spCompressedData sp_compressed_data;
                sp_compressed_data = decompression.compressed_data_q_.front();

                decompression.compressed_data_q_.pop();
                decompression.mutex_compressed_data_q_.unlock();

                uint32_t bytes_compressed = sp_compressed_data->get_bytes_compressed();
                spRawPage sp_raw_page(new RawPage(bytes_compressed,
                        sp_compressed_data->page_number()));

                uint8_t* decompressed_buffer = sp_raw_page->buffer();

                spHammanTreeNode sp_root = sp_compressed_data->get_hamman_data()->get_hamman_tree_root();
                spHammanTreeNode sp_current_node;
                BitBunch& bit_bunch = *sp_compressed_data->get_bit_bunch().get();
                for ( uint32_t i = 0; i < bytes_compressed; ++i ) {
                    sp_current_node = sp_root;

                    uint32_t current_bit = 0;
                    while (sp_current_node.get() != nullptr) {
                        if ( bit_bunch[current_bit] == BitBunch::bit_state::ZERO ) {
                            sp_current_node = sp_current_node->get_left_son();
                        } else {
                            sp_current_node = sp_current_node->get_right_son();
                        }
                        ++current_bit;
                    }
                    *(decompressed_buffer + i) = sp_current_node->get_character();
                }
                
                decompression.mutex_ready_for_write_data_priority_q_.lock();
                decompression.ready_for_write_data_priority_q_.push(sp_raw_page);
                decompression.mutex_ready_for_write_data_priority_q_.unlock();
            }
        }
    }
}