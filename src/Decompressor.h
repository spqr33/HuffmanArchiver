/* 
 * File:   Decompressor.h
 * Author: s
 *
 * Created on July 31, 2015, 11:42 AM
 */

#ifndef DECOMPRESSOR_H
#define	DECOMPRESSOR_H

#include <queue>
#include <mutex>
#include <memory>
#include "Decompression.h"

namespace LobKo {
    class HammanData;
    typedef std::shared_ptr<HammanData> spHammanData;
    
    class Decompressor {
    public:
        Decompressor(Decompression& decompression);
        virtual ~Decompressor();
        
         void operator()(Decompression& decompression);
         spHammanData sp_hamman_data_;
         
    private:

    };
}
#endif	/* DECOMPRESSOR_H */

