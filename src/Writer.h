/* 
 * File:   Writer.h
 * Author: s
 *
 * Created on July 19, 2015, 4:31 PM
 */

#ifndef WRITER_H
#define	WRITER_H
#include <vector>
#include <functional>
#include <queue>
#include "CompressedData.h"
#include <mutex>

namespace LobKo {

    typedef std::priority_queue<spCompressedData, std::vector<spCompressedData>, std::function<bool(spCompressedData, spCompressedData) >> CompressedDataPriorityQueue;

    class Writer {
    public:
        Writer(uint64_t last_page_num);
        //Writer(CompressedDataPriorityQueue& ready_for_write_data_priority_queue, std::mutex& mutex_ready_for_write_data_priority_queue);
        virtual ~Writer();
        void operator()(CompressedDataPriorityQueue& ready_for_write_data_priority_queue, std::mutex& mutex_ready_for_write_data_priority_queue,
        bool& writing_done, std::mutex& mutex_writing_done);
    private:
        uint64_t wait_page_num_;
        uint64_t last_page_num_;
        //        Writer(const Writer& orig);
        //CompressedDataPriorityQueue& ready_for_write_data_priority_queue_;
        //std::mutex& mutex_ready_for_write_data_priority_queue_;

    };
}
#endif	/* WRITER_H */

