/* 
 * File:   DecomressedPageWriter.h
 * Author: s
 *
 * Created on July 31, 2015, 8:50 PM
 */

#ifndef DECOMPRESSEDPAGEWRITER_H
#define	DECOMPRESSEDPAGEWRITER_H
#include <iostream>
#include "Decompression.h"

namespace LobKo {

    class DecompressedPageWriter {
    public:
        DecompressedPageWriter(uint64_t last_page_num, std::ofstream& file_out);
        virtual ~DecompressedPageWriter();

        void operator()(Decompression& decompression);

    private:
        uint64_t wait_page_num_;
        uint64_t last_page_num_;
        std::ofstream& file_out_;
    };
}
#endif	/* DECOMPRESSEDPAGEWRITER_H */

