/* 
 * File:   CompressedFileReader.cpp
 * Author: s
 * 
 * Created on July 29, 2015, 10:40 PM
 */

#include "CompressedFileReader.h"
#include "Decompression.h"
#include "CompressedData.h"
#include <cassert>
#include <queue>
#include <mutex>
#include <iostream>
#include <thread>
namespace LobKo {

    CompressedFileReader::CompressedFileReader(std::ifstream& compressed_file) :
    Reader(compressed_file) {
        ;
    }

    CompressedFileReader::~CompressedFileReader() {
        ;
    }

    pair<uint8_t*, size_t> CompressedFileReader::get_header() {
        assert(file_in_.good()&& "CompressedFileReader::get_header(), file_in_.good()");
        uint16_t header_size;
        file_in_.read(reinterpret_cast<char*> (&header_size), sizeof (header_size));
        assert(header_size > 0 && "CompressedFileReader::get_header(), header_size > 0");
        std::cout << "\t\theader_size: " << header_size << std::endl;

        uint8_t* buffer = new uint8_t[header_size];
        file_in_.read(reinterpret_cast<char*> (buffer), header_size);
        assert(file_in_.gcount() == header_size && "CompressedFileReader::get_header(), file_in_.gcount() == header_size");

        return std::make_pair(buffer, header_size);
    }

    void CompressedFileReader::operator()(Decompression& decompression) {
        pair<uint8_t*, size_t> header = get_header();
        decompression.header_ = header;
        {
            std::lock_guard<mutex> lk(decompression.mutex_header_);
            decompression.header_calculated_ = true;
            decompression.cv_header_.notify_all();
        }

        //decompression.sp_hamman_data_ = spHammanData(new HammanData(header));

        //assert(decompression.sp_hamman_data_.get() && "CompressedFileReader::operator()");
        file_in_.read(reinterpret_cast<char*> (&decompression.last_page_num_), sizeof (decompression.last_page_num_));
        {
            std::lock_guard<std::mutex> lk(decompression.mutex_last_page_num_);
            decompression.last_page_num_calculated_ = true;
            decompression.cv_last_page_num_.notify_all();
        }

        bool can_read = true;
        file_in_.clear();
        std::cout << "[CompressedFileReader] Filesize: " << file_size_ << " file_in_.tellg(): " << file_in_.tellg() << std::endl;
        do {
            if ( file_in_.eof() ) {
                assert(1);
            }

            can_read = true;

            decompression.mutex_compressed_data_q_.lock();
            if ( decompression.compressed_data_q_.size() >= 40 ) {
                //std::cout << "[CompressedFileReader] compressed_data_q_.size(): " << decompression.compressed_data_q_.size() << std::endl;
                can_read = false;
                std::this_thread::yield();
            }
            decompression.mutex_compressed_data_q_.unlock();

            if ( can_read == true ) {
                uint32_t compressed_block_size = 0;
                uint32_t bytes_compressed = 0;

                file_in_.read(reinterpret_cast<char*> (&compressed_block_size),
                        sizeof (compressed_block_size));
                assert(file_in_.good() && "file_in_.good()\n");

                file_in_.read(reinterpret_cast<char*> (&bytes_compressed),
                        sizeof (bytes_compressed));
                assert(file_in_.good() && "file_in_.good()2\n");

                spBitBunch sp_bit_bunch(new BitBunch(compressed_block_size * BitBunch::BITS_PER_BYTE));
                uint8_t * buffer = const_cast<uint8_t *> (sp_bit_bunch->buffer());
                file_in_.read(reinterpret_cast<char*> (buffer), compressed_block_size);
                assert(file_in_.good() && "file_in_.good()3\n");

                //reading_counter_ is initialized by 0;
                spCompressedData sp_compressed_data(new CompressedData(reading_counter_ + 1));
                sp_compressed_data->set_bytes_compressed(bytes_compressed);
                sp_compressed_data->set_bit_bunch(sp_bit_bunch);

                ++reading_counter_;

                decompression.mutex_compressed_data_q_.lock();
                decompression.compressed_data_q_.push(sp_compressed_data);
                decompression.mutex_compressed_data_q_.unlock();

                if ( file_in_.eof() ) {
                    assert(1);
                }
            }
            //} while (file_in_.good());
        } while (file_size_ != file_in_.tellg());

        if ( file_in_.bad() ) {
#ifndef NDEBUG
            std::cout << "[CompressedFileReader] I/O error while reading, reading_counter_ == " << reading_counter_ << "\n";
#endif
            //TODO clean up
            exit(EXIT_FAILURE);
        } else if ( file_size_ == file_in_.tellg() ) {
#ifndef NDEBUG
            std::cout << "[CompressedFileReader] End of file reached successfully\n";
#endif
            std::lock_guard<std::mutex> lock(decompression.mutex_reading_done_);
            decompression.last_page_num_ = reading_counter_;
            decompression.reading_done_ = true;
        }
    }


}
