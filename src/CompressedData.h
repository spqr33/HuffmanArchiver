/* 
 * File:   CompressedData.h
 * Author: s
 *
 * Created on July 18, 2015, 12:36 PM
 */

#ifndef COMPRESSEDDATA_H
#define	COMPRESSEDDATA_H

#include <memory>
#include "HammanData.h"
#include "BitBunch.h"
#include "Compressor.h"

namespace LobKo {
    class HammanData;
    typedef std::shared_ptr<HammanData> spHammanData;
    class CompressedData;
    typedef std::shared_ptr<CompressedData> spCompressedData;
    class BitBunch;
    typedef std::shared_ptr<BitBunch> spBitBunch;

    bool compressed_data_copmarision(spCompressedData a, spCompressedData b);

    class CompressedData {
    public:
        explicit CompressedData(uint32_t page_number);
        virtual ~CompressedData();

        inline uint32_t page_number() const;
        inline void set_hamman_data(spHammanData new_hamman_data);
        inline void set_bit_bunch(spBitBunch new_bit_bunch);
        inline const spHammanData& get_hamman_data() const;
        inline const spBitBunch& get_bit_bunch() const;
        inline void set_bytes_compressed(uint32_t bytes_compressed);
        inline uint32_t get_bytes_compressed() const;
    private:
        const uint32_t page_number_;
        spHammanData sp_hamman_data_;
        spBitBunch sp_bit_bunch_;
        uint32_t bytes_compressed_;

        CompressedData(const CompressedData& orig);
        const CompressedData& operator=(const CompressedData& rhs);
    };

    uint32_t CompressedData::page_number() const {
        return page_number_;
    }

    void CompressedData::set_hamman_data(spHammanData new_hamman_data) {
        sp_hamman_data_ = new_hamman_data;
    }

    void CompressedData::set_bytes_compressed(uint32_t bytes_compressed) {
        bytes_compressed_ = bytes_compressed;
    }

    const spHammanData& CompressedData::get_hamman_data() const {
        return sp_hamman_data_;
    }

    const spBitBunch& CompressedData::get_bit_bunch() const {
        return sp_bit_bunch_;
    }

    void CompressedData::set_bit_bunch(spBitBunch new_bit_bunch) {
        sp_bit_bunch_ = new_bit_bunch;
    }

    uint32_t CompressedData::get_bytes_compressed() const {
        return bytes_compressed_;
    }
}
#endif	/* COMPRESSEDDATA_H */

