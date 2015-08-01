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

    class Decompressor {
    public:
        Decompressor();
        virtual ~Decompressor();
        
         void operator()(Decompression& decompression);
    private:

    };
}
#endif	/* DECOMPRESSOR_H */

