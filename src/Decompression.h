/* 
 * File:   Decompression.h
 * Author: s
 *
 * Created on July 30, 2015, 3:23 PM
 */

#ifndef DECOMPRESSION_H
#define	DECOMPRESSION_H

#include <mutex>
#include <memory>
#include <queue>
#include "HammanData.h"
#include "RawPage.h"
#include <functional>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <utility>

using std::mutex;
using std::queue;
using std::priority_queue;
using std::vector;
using std::function;
using std::condition_variable;


namespace LobKo {
    class CompressedData;
    typedef std::shared_ptr<CompressedData> spCompressedData;
    class HammanData;
    typedef std::shared_ptr<HammanData> spHammanData;
    class RawPage;
    typedef std::shared_ptr<RawPage> spRawPage;

    struct Decompression {
        Decompression();
        virtual ~Decompression();

        std::pair<uint8_t*, size_t> header_;
        bool header_calculated_;
        mutex mutex_header_;
        condition_variable cv_header_;

        //spHammanData sp_hamman_data_;

        bool reading_done_ = false;
        mutex mutex_reading_done_;
        bool writing_done_ = false;
        mutex mutex_writing_done_;
        queue<spCompressedData> compressed_data_q_;
        mutex mutex_compressed_data_q_;

        typedef priority_queue<spRawPage, vector<spRawPage>, function<bool(spRawPage, spRawPage) >> RawPagePriorityQueue;
        RawPagePriorityQueue ready_for_write_data_priority_q_;
        mutex mutex_ready_for_write_data_priority_q_;

        uint64_t last_page_num_;
        bool last_page_num_calculated_;
        mutex mutex_last_page_num_;
        condition_variable cv_last_page_num_;
    private:
        Decompression(const Decompression& orig) = delete;
        Decompression& operator=(const Decompression& rhs) = delete;
    };
}
#endif	/* DECOMPRESSION_H */

