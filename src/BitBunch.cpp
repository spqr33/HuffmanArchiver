/* 
 * File:   BitBunch.cpp
 * Author: s
 * 
 * Created on July 6, 2015, 10:06 PM
 */

#include "BitBunch.h"
#include <limits>
#include <cstddef>
#include <stdexcept>
#include <bitset>
#include <sstream>

const uint32_t LobKo::BitBunch::MAX_BYTES_COUNT = 1024 * 1024 * 8;
//const uint16_t LobKo::BitBunch::MAX_BYTES_COUNT = std::numeric_limits<uint16_t>::max();

namespace LobKo {

    BitBunch::BitBunch(uint32_t bitsQuantity) :
    usage_count_(1),
    current_bit_(BITS_PER_BYTE - 1),
    size_(0) {
        if ( bitsQuantity > max_bit_can_hold() ) {
            std::stringstream s;
            s << "BitBunch(), More than the limit bits requested, bitsQuantity: " << bitsQuantity;
            throw std::out_of_range(s.str());
        }

        // TODO if bitsQuantity == 0 ????

        allocated_bytes_ = bitsQuantity / BITS_PER_BYTE;
        if ( bitsQuantity % BITS_PER_BYTE != 0 ) {
            ++allocated_bytes_;
        };

        buffer_ = buff_allocate(allocated_bytes_);
        current_byte_ = buffer_;
    };

    BitBunch::BitBunch(uint8_t* buffer, uint32_t buffer_size) :
    usage_count_(1),
    current_bit_(BITS_PER_BYTE - 1),
    size_(0) {
        if ( (buffer_size * BITS_PER_BYTE) > max_bit_can_hold() ) {
            throw std::out_of_range("BitBunch(), More than the limit bits requested");
        }

        allocated_bytes_ = buffer_size;
        buffer_ = buffer;
        current_byte_ = buffer_;
    }

    BitBunch::~BitBunch() {
        buff_free_memory();
    };

    uint8_t* BitBunch::buff_allocate(const uint32_t size_in_bytes, fill_behavior fill) {
        if ( size_in_bytes > MAX_BYTES_COUNT ) {
            throw std::out_of_range("buff_allocate(), More than the limit bits requested");
        }

        uint8_t* new_buffer = new uint8_t[size_in_bytes];

        if ( fill == FILL_ZERO ) {
            buffer_zeroing(new_buffer, size_in_bytes);
        }
        return new_buffer;
    };

    void BitBunch::buff_free_memory() {
        delete [] buffer_;
    }

    void BitBunch::buffer_zeroing(uint8_t* buffer, uint32_t buffer_size) {
        // TODO we could fill buff by uint32_t
        // or bzero() || memset()
        for ( uint32_t i = 0; i < buffer_size; ++i ) {
            buffer[i] = 0;
        };
    }

    void BitBunch::buff_copy(uint8_t* dest, uint32_t dest_size) const {
#ifndef NDEBUG
        std::cout << "Buffer copied, size: " << this->size_ << "capacity: " << capacity() << "\n";
#endif
        int n = (allocated_bytes_ <= dest_size) ? allocated_bytes_ : dest_size;

        for ( uint32_t i = 0; i < n; ++i ) {
            dest[i] = buffer_[i];
        };
    }

    void BitBunch_dump(const BitBunch& bit_bunch) {
        for ( uint32_t i = 0; i < bit_bunch.get_size_in_bytes(); ++i ) {
            //bitset b = std::bitset(bit_bunch[i]);
            std::cout << std::bitset<8>(bit_bunch.buffer_[i]) << std::endl;
        }
        std::cout << "Size: " << bit_bunch.size_ << std::endl << "__________\n";
    }

    //    const BitBunch& BitBunch::operator+=(const BitBunch& rhs) {
    //        //return *this;
    //    }

    BitBunch::BitBunch(const BitBunch& orig) {
        //usage_count_ = 1;
        allocated_bytes_ = orig.allocated_bytes_;
        size_ = orig.size_;
        buffer_ = buff_allocate(allocated_bytes_, DONT_FILL_ZERO);
        orig.buff_copy(buffer_, allocated_bytes_);
        uint32_t n = orig.current_byte_ - orig.buffer_;
        current_byte_ = buffer_ + n;
        current_bit_ = orig.current_bit_;

    };

    const BitBunch& BitBunch::operator=(const BitBunch& rhs) {
        if ( this == &rhs ) {
            return *this;
        }
        uint8_t* new_buffer = buff_allocate(rhs.allocated_bytes_, DONT_FILL_ZERO);
        rhs.buff_copy(new_buffer, rhs.allocated_bytes_);
        allocated_bytes_ = rhs.allocated_bytes_;
        size_ = rhs.size_;
        uint32_t n = rhs.current_byte_ - rhs.buffer_;
        current_byte_ = new_buffer + n;
        current_bit_ = rhs.current_bit_;
        buff_free_memory();
        buffer_ = new_buffer;
    }

    const BitBunch& BitBunch::operator+=(const BitBunch& rhs) {
        //#ifndef NDEBUG
        //        std::cout << "BitBunch::operator+=()" << std::endl;
        //#endif

        if ( (size_ + rhs.size_) > capacity() ) {
            if ( (size_ + rhs.size_) > max_bit_can_hold() ) {
                assert(size_ + rhs.size_ < max_bit_can_hold() && size_ && " " && rhs.size_);
                throw std::out_of_range("operator+=(),More than the limit bits requested");
            }
            uint8_t* new_buffer;
            uint8_t* old_current_byte = current_byte_;
            // TODO change  FILL_ZERO to DONT_FILL_ZERO and check behavior
            uint32_t need_bytes = (size_ + rhs.size_) / BITS_PER_BYTE;
            if ( (size_ + rhs.size_) % BITS_PER_BYTE != 0 ) {
                ++need_bytes;
            };

            if ( need_bytes < (allocated_bytes_ * BUFFER_GROW_FACTOR) ) {
                need_bytes = allocated_bytes_ * BUFFER_GROW_FACTOR;
            }
            new_buffer = buff_allocate(need_bytes, FILL_ZERO);
            buff_copy(new_buffer, need_bytes);

            current_byte_ = new_buffer + (old_current_byte - buffer_);
            buff_free_memory();
            buffer_ = new_buffer;

            allocated_bytes_ = need_bytes;
            //current_bit_ --- left the same.
        };
        for ( uint32_t i = 0; i < rhs.size_; ++i ) {
            if ( rhs[i] == ONE ) {
                append_one_bit();
            } else {
                append_zero_bit();
            }
        }
    }
}



