/* 
 * File:   Writer.cpp
 * Author: s
 * 
 * Created on July 19, 2015, 4:31 PM
 */

#include "Writer.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <iostream>

namespace LobKo {

    Writer::Writer(uint64_t last_page_num, std::ofstream& file_out, std::array<uint32_t, 256>& frequency_array) :
    wait_page_num_(1),
    last_page_num_(last_page_num),
    file_out_(file_out),
    frequency_array_(frequency_array) {
        ;
    }

    Writer::Buff::Buff(uint32_t buffer_size) :
    buffer_size_(buffer_size),
    water_mark_(start_),
    start_(new uint8_t[buffer_size]) {
        ;
    }

    Writer::Buff::~Buff() {
        delete [] start_;
    }

    Writer::~Writer() {
    }

    void Writer::operator()(CompressedDataPriorityQueue& ready_for_write_data_priority_queue, std::mutex& mutex_ready_for_write_data_priority_queue,
            bool& writing_done, std::mutex& mutex_writing_done) {

        {//Write once serialized frequency array
            std::shared_ptr<Writer::Buff> sp_buff = serialization_frequency_arr_to_buffer();
            uint16_t size = sp_buff->water_mark_ - sp_buff->start_;

            std::cout << "Buffer size: " << size << std::endl; 
            
            assert(size > 0 && "void Writer::operator(): sp_buff->water_mark_ - sp_buff->start_");
            assert(file_out_.good()&& "void Writer::operator() file_out_.good()");
            file_out_.write(reinterpret_cast<const char*> (&size), sizeof (size));
            file_out_.write(reinterpret_cast<const char*> (sp_buff->start_), size);
            file_out_.write(reinterpret_cast<char*>(&last_page_num_),sizeof(last_page_num_));
        }
        uint64_t compressed_size = 0;
        while (true) {
            mutex_ready_for_write_data_priority_queue.lock();
            if ( !ready_for_write_data_priority_queue.empty() ) {
                if ( wait_page_num_ == ready_for_write_data_priority_queue.top()-> page_number() ) {

                    spCompressedData sp_compressed_data(ready_for_write_data_priority_queue.top());
                    ready_for_write_data_priority_queue.pop();
                    std::cout << "Ready for write page# " << sp_compressed_data->page_number() << " from " << last_page_num_ << std::endl;
                    mutex_ready_for_write_data_priority_queue.unlock();

                    //writing data
                    uint32_t compressed_block_size = sp_compressed_data->get_bit_bunch()->get_size_in_bytes();
                    uint32_t bytes_compressed = sp_compressed_data->get_bytes_compressed();
                    compressed_size += sp_compressed_data->get_bit_bunch()->get_size_in_bytes();

                    std::cout << "bytes_compressed: " << bytes_compressed << std::endl;


                    file_out_.write(reinterpret_cast<const char *> (&compressed_block_size), sizeof (compressed_block_size));
                    file_out_.write(reinterpret_cast<const char *> (&bytes_compressed), sizeof (bytes_compressed));

                    file_out_.write(reinterpret_cast<const char *> (sp_compressed_data->get_bit_bunch()->buffer()), compressed_block_size);
                    //end of writing data
                    if ( wait_page_num_ == last_page_num_ ) {
                        mutex_writing_done.lock();
                        writing_done = true;
                        mutex_writing_done.unlock();

                        std::cout << "Writer is exiting, compressed_size=" << compressed_size << "Bytes\n";
                        break;
                    }
                    ++wait_page_num_;
                }
            }
            mutex_ready_for_write_data_priority_queue.unlock();

        }
    }

    std::shared_ptr<Writer::Buff> Writer::serialization_frequency_arr_to_buffer() {
        uint32_t buff_size;
        uint32_t freq_array_size = frequency_array_.size();

        buff_size = freq_array_size * (sizeof (character) + sizeof (frequency));
        Buff* buffer = new Buff(buff_size);

        for ( uint32_t i = 0; i < freq_array_size; ++i ) {
            if ( frequency_array_[i] != 0 ) {
                *buffer->water_mark_++ = static_cast<uint8_t> (i);
                *reinterpret_cast<frequency*> (buffer->water_mark_) = frequency_array_[i];
                buffer->water_mark_ += sizeof (frequency);
            }
        }

        return std::shared_ptr<Writer::Buff>(buffer);
    }
}