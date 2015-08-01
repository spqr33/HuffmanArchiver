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

        uint8_t* buffer = new uint8_t[header_size];
        file_in_.read(reinterpret_cast<char*> (buffer), header_size);
        assert(file_in_.gcount() == header_size && "CompressedFileReader::get_header(), file_in_.gcount() == header_size");

        return std::make_pair(buffer, header_size);
    }

    void CompressedFileReader::operator()(Decompression& decompression) {
        decompression.sp_hamman_data_ = spHammanData(new HammanData(get_header()));

        assert(decompression.sp_hamman_data_.get() && "CompressedFileReader::operator()");

        bool can_read = true;
        do {
            can_read = true;

            decompression.mutex_compressed_data_q_.lock();
            if ( decompression.compressed_data_q_.size() >= 40 ) {
                can_read = false;
            }
            decompression.mutex_compressed_data_q_.unlock();

            if ( can_read == true ) {
                uint32_t compressed_block_size;
                uint32_t bytes_compressed;

                file_in_.read(reinterpret_cast<char*> (&compressed_block_size),
                        sizeof (compressed_block_size));
                file_in_.read(reinterpret_cast<char*> (&bytes_compressed),
                        sizeof (bytes_compressed));

                //reading_counter_ is initialized by 0;
                spCompressedData sp_compressed_data(new CompressedData(reading_counter_ + 1));
                sp_compressed_data->set_hamman_data(decompression.sp_hamman_data_);
                sp_compressed_data->set_bytes_compressed(bytes_compressed);

                spBitBunch sp_bit_bunch(new BitBunch(bytes_compressed * BitBunch::MAX_BYTES_COUNT));
                sp_compressed_data->set_bit_bunch(sp_bit_bunch);
                uint8_t * buffer = const_cast<uint8_t *> (sp_compressed_data->get_bit_bunch()->buffer());
                file_in_.read(reinterpret_cast<char*> (buffer), bytes_compressed);
                ++reading_counter_;

                decompression.mutex_compressed_data_q_.lock();
                decompression.compressed_data_q_.push(sp_compressed_data);
                decompression.mutex_compressed_data_q_.unlock();
            }
        } while (file_in_.good());

        if ( file_in_.bad() ) {
#ifndef NDEBUG
            std::cout << "I/O error while reading\n";
#endif
            //TODO clean up
            exit(EXIT_FAILURE);
        } else if ( file_in_.eof() ) {
#ifndef NDEBUG
            std::cout << "End of file reached successfully\n";
#endif
            std::lock_guard<std::mutex> lock(decompression.mutex_reading_done_);
            decompression.last_page_num_ = reading_counter_;
            decompression.reading_done_ = true;
        }
    }


}
