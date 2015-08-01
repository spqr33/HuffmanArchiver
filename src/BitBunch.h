/* 
 * File:   BitBunch.h
 * Author: s
 *
 * Created on July 6, 2015, 10:06 PM
 */

#ifndef BITBUNCH_H
#define	BITBUNCH_H

#include <cstdint>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <memory>

namespace LobKo {
    class BitBunch;
    typedef std::shared_ptr<BitBunch> spBitBunch;

    class BitBunch {
        friend void BitBunch_dump(const BitBunch& bit_bunch);
    public:
        /**
         * can throw std::out_of_range("More than the limit bits requested")
         * if bitsQuantity > max_bit_can_hold()
         * can throw std::bad_alloc()
         * if memory allocation failed
         * @param bitsQuantity
         */
        explicit BitBunch(uint32_t bitsQuantity = 32);
        BitBunch(uint8_t* buffer, uint32_t buffer_size);
        BitBunch(const BitBunch& orig);

        ~BitBunch();
        /**
         * can throw std::out_of_range("More than the limit bits requested");
         *  if (size_ == max_bit_can_hold()) is. 
         */
        inline void append_zero_bit();
        inline void append_one_bit();
        inline uint32_t max_bit_can_hold() const;
        inline uint32_t capacity() const; // in bits
        inline uint32_t get_size_in_bytes() const;
        inline const uint8_t* buffer() const;
        const BitBunch& operator+=(const BitBunch& rhs);

        enum add_behavior {
            NONE, ADD_ZERO, ADD_ONE
        };
        
        enum fill_behavior {
            FILL_ZERO, DONT_FILL_ZERO
        };
        uint8_t* buff_allocate(const uint32_t size_in_bytes, fill_behavior fill = FILL_ZERO);
        enum bit_state {
            ZERO, ONE
        };
        bit_state operator[](uint32_t position) const;
        static const uint32_t MAX_BYTES_COUNT;
    protected:

        /**
         * can throw std::out_of_range("More than the limit bits requested");
         */
        void buff_free_memory();
        void buff_copy(uint8_t* dest, uint32_t dest_size) const;

        //inline void set_bit();

        uint32_t usage_count_;

        uint32_t size_; //in bits

        uint8_t* buffer_;
        uint8_t* current_byte_;
        uint8_t current_bit_;
        uint32_t allocated_bytes_;
        
        static const uint8_t BITS_PER_BYTE = 8;
        static const int BUFFER_GROW_FACTOR = 2;
    private:
        void buffer_zeroing(uint8_t* buffer, uint32_t buffer_size);

        BitBunch(BitBunch&& orig);
        const BitBunch& operator=(const BitBunch& rhs);
        const BitBunch& operator=(BitBunch&& rhs);

    };

    void BitBunch_dump(const BitBunch& bit_bunch);
};

uint32_t LobKo::BitBunch::max_bit_can_hold() const {
    return MAX_BYTES_COUNT * BITS_PER_BYTE;
};

void LobKo::BitBunch::append_zero_bit() {
    if (size_ == capacity()) {
        if (size_ == max_bit_can_hold()) {
            throw std::out_of_range("append_zero_bit(), More than the limit bits requested");
        }
        uint8_t* new_buffer;

        // TODO change  FILL_ZERO to DONT_FILL_ZERO and check behavior
        new_buffer = buff_allocate(allocated_bytes_ * BUFFER_GROW_FACTOR, FILL_ZERO);
        buff_copy(new_buffer, allocated_bytes_ * BUFFER_GROW_FACTOR);
        buff_free_memory();
        buffer_ = new_buffer;

        current_byte_ = new_buffer + allocated_bytes_;

        allocated_bytes_ *= BUFFER_GROW_FACTOR;
    };

    *current_byte_ &= ~(((uint8_t) 1) << current_bit_);

    if (current_bit_ == 0) {
        current_bit_ = BITS_PER_BYTE - 1;
        ++current_byte_;
    } else {
        --current_bit_;
    }

    ++size_;
}

void LobKo::BitBunch::append_one_bit() {
    if (size_ == capacity()) {
        if (size_ == max_bit_can_hold()) {
            throw std::out_of_range("append_one_bit(), More than the limit bits requested");
        }
        uint8_t* new_buffer;

        // TODO change  FILL_ZERO to DONT_FILL_ZERO and check behavior
        new_buffer = buff_allocate(allocated_bytes_ * BUFFER_GROW_FACTOR, FILL_ZERO);
        buff_copy(new_buffer, allocated_bytes_ * BUFFER_GROW_FACTOR);
        buff_free_memory();
        buffer_ = new_buffer;

        current_byte_ = new_buffer + allocated_bytes_;

        allocated_bytes_ *= BUFFER_GROW_FACTOR;
    };

    *current_byte_ |= ((uint8_t) 1) << current_bit_;

    if (current_bit_ == 0) {
        current_bit_ = BITS_PER_BYTE - 1;
        ++current_byte_;
    } else {
        --current_bit_;
    }

    ++size_;

}

uint32_t LobKo::BitBunch::capacity() const {// in bits
    return allocated_bytes_ * BITS_PER_BYTE;
};

uint32_t LobKo::BitBunch::get_size_in_bytes() const {
    //uint32_t res = current_byte_ - buffer_ + 1;


    if (size_ % BITS_PER_BYTE == 0) {
        return current_byte_ - buffer_;
    } else {
        return current_byte_ - buffer_ + 1;
    }

}

const uint8_t* LobKo::BitBunch::buffer() const {
    return buffer_;
}
#endif	/* BITBUNCH_H */

