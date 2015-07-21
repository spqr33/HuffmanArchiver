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
#include <array>
#include <utility>

namespace LobKo {

    typedef std::priority_queue<spCompressedData, std::vector<spCompressedData>, std::function<bool(spCompressedData, spCompressedData) >> CompressedDataPriorityQueue;

    class Writer {
    public:
        Writer(uint64_t last_page_num, std::ofstream& file_out, std::array<uint32_t, 256>& frequency_array);
        virtual ~Writer();
        
        void operator()(CompressedDataPriorityQueue& ready_for_write_data_priority_queue, std::mutex& mutex_ready_for_write_data_priority_queue,
                bool& writing_done, std::mutex& mutex_writing_done);
    private:

        uint64_t wait_page_num_;
        uint64_t last_page_num_;
        std::ofstream& file_out_;
        const std::array<uint32_t, 256>& frequency_array_;

        typedef uint8_t character;
        typedef uint32_t frequency;

        struct Buff {
            Buff(uint32_t buffer_size);
            ~Buff();
            uint8_t * const start_;
            uint32_t buffer_size_;
            uint8_t* water_mark_;
        private:
            Buff(const Buff&);
            const Buff& operator=(const Buff&);
        };
        std::shared_ptr<Buff> serialization_frequency_arr_to_buffer();

    };
}
#endif	/* WRITER_H */

