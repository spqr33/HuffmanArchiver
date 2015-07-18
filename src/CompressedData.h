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

    bool copmressed_data_copmarision(spCompressedData a, spCompressedData b);

    class CompressedData {
    public:
        explicit CompressedData(uint32_t page_number);
        virtual ~CompressedData();
        inline uint32_t page_number() const;
    private:
        uint32_t page_number_;
        spHammanData sp_hamman_data;

        CompressedData(const CompressedData& orig);
        const CompressedData& operator=(const CompressedData& rhs);

    };

    uint32_t CompressedData::page_number() const {
        return page_number_;
    }
}
#endif	/* COMPRESSEDDATA_H */

