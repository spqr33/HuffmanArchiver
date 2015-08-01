/* 
 * File:   CompressedFileReader.h
 * Author: s
 *
 * Created on July 29, 2015, 10:40 PM
 */

#ifndef COMPRESSEDFILEREADER_H
#define	COMPRESSEDFILEREADER_H

#include <fstream>
#include "Reader.h"
#include <utility>
#include <memory>
#include <queue>
#include <mutex>
#include "Decompression.h"
#include "HammanData.h"

using std::pair;
using std::shared_ptr;
namespace LobKo {

    class HammanData;
    typedef std::shared_ptr<HammanData> spHammanData;

    class CompressedFileReader : public Reader {
    public:
        explicit CompressedFileReader(std::ifstream& compressed_file);
        virtual ~CompressedFileReader();

        pair<uint8_t*, size_t> get_header();
        void operator()(Decompression& decompression);
    private:
        //CompressedFileReader(const CompressedFileReader& orig) = delete;
        CompressedFileReader& operator=(CompressedFileReader& rhs) = delete;
    };
}
#endif	/* COMPRESSEDFILEREADER_H */

