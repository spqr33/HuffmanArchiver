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
        Writer(CompressedDataPriorityQueue& ready_for_write_data_priority_queue, std::mutex& mutex_ready_for_write_data_priority_queue);
        virtual ~Writer();
        void operator()() const;
    private:
//        Writer(const Writer& orig);
        CompressedDataPriorityQueue& ready_for_write_data_priority_queue_;
        std::mutex& mutex_ready_for_write_data_priority_queue_;

    };
}
#endif	/* WRITER_H */

