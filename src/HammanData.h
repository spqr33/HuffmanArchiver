/* 
 * File:   HammanData.h
 * Author: s
 *
 * Created on July 15, 2015, 7:50 PM
 */

#ifndef HAMMANDATA_H
#define	HAMMANDATA_H

#include "RawPage.h"
#include "HammanTreeNode.h"
#include "HammanTreeNodeCopmarision.h"
#include "BitBunch.h"
#include <array>
#include <queue>
#include <vector>
#include <memory>

namespace LobKo {
    class RawPage;
    class HammanTreeNode;
    //struct HammanTreeNodeCopmarision;
    typedef std::shared_ptr<HammanTreeNode> spHammanTreeNode;

    class HammanData {
    public:
        explicit HammanData(const RawPage& inputPage);
        virtual ~HammanData();
        void build_character_frequency_array(const uint8_t * buffer, const uint32_t size);
    private:
        std::array<uint32_t, 256> character_frequency_;
        std::array<spHammanTreeNode, 256> character_to_node_map_;

        //typedef std::priority_queue<spHammanTreeNode, std::vector<spHammanTreeNode>> pri_queue_;
        typedef std::priority_queue<spHammanTreeNode, std::vector<spHammanTreeNode>, HammanTreeNodeCopmarision> pri_queue_;
        pri_queue_ priority_q_;

        spHammanTreeNode hamman_tree_root_;

        //void calc_bit_bunch(spHammanTreeNode node, const spBitBunch bits_sequence, BitBunch::add_behavior behavior);
        HammanData(const HammanData& orig);
        const HammanData& operator=(const HammanData& rhs);

    };

}
#endif	/* HAMMANDATA_H */
