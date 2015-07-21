/* 
 * File:   Compressor.h
 * Author: s
 *
 * Created on July 18, 2015, 12:50 PM
 */

#ifndef COMPRESSOR_H
#define	COMPRESSOR_H

#include "CompressedData.h"
#include "HammanData.h"
#include "RawPage.h"
#include <queue>
#include <mutex>
#include <memory>

namespace LobKo {
    class CompressedData;
    typedef std::shared_ptr<CompressedData> spCompressedData;
    class HammanData;
    typedef std::shared_ptr<HammanData> spHammanData;
    typedef std::priority_queue<spCompressedData, std::vector<spCompressedData>, std::function<bool(spCompressedData, spCompressedData) >> CompressedDataPriorityQueue;

    bool hamman_tree_node_copmarision(spHammanTreeNode a, spHammanTreeNode b);

    class Compressor {
    public:
        //Compressor();
        explicit Compressor(spHammanData& sp_hamman_data);
        virtual ~Compressor();
        void operator()(std::queue<spRawPage>& raw_pages_queue,
            std::mutex& mutex_reading_queue,
            bool& reading_done, std::mutex& mutex_reading_done,
            CompressedDataPriorityQueue& ready_for_write_data_priority_queue,
            std::mutex& mutex_ready_for_write_data_priority_queue,
            bool& writing_done, std::mutex& mutex_writing_done) const;
        //void operator()() const;
    private:
        //Compressor(const Compressor& orig);
        spHammanData& sp_hamman_data_;
        const Compressor& operator=(const Compressor& rhs);
    };
}
#endif	/* COMPRESSOR_H */

