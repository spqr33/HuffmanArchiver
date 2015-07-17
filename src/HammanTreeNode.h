/* 
 * File:   HammanTreeNode.h
 * Author: s
 *
 * Created on July 15, 2015, 9:48 PM
 */

#ifndef HAMMANTREENODE_H
#define	HAMMANTREENODE_H

#include <memory>
#include "BitBunch.h"
#include "HammanData.h"
#include <string>

namespace LobKo {
    class HammanTreeNode;
    class HammanData;
    typedef std::shared_ptr<HammanTreeNode> spHammanTreeNode;
    

    class HammanTreeNode {
        friend class HammanData;
    public:
        HammanTreeNode(uint32_t frequency);
        virtual ~HammanTreeNode();

        static spHammanTreeNode get_node(uint32_t frequency);
        static spHammanTreeNode get_node(uint32_t frequency, spHammanTreeNode left, spHammanTreeNode right);
        inline uint32_t frequency() const;
        inline void set_left_son(spHammanTreeNode left);
        inline void set_right_son(spHammanTreeNode left);
#ifndef NDEBUG        
        std::string characters;
#endif
    private:
        uint32_t frequency_;
        spHammanTreeNode left_;
        spHammanTreeNode right_;
        spBitBunch bits_sequence;

        const HammanTreeNode& operator=(const HammanTreeNode& rhs);
        HammanTreeNode(const HammanTreeNode& orig);
    };

    uint32_t HammanTreeNode::frequency() const {
        return frequency_;
    };

    void HammanTreeNode::set_left_son(spHammanTreeNode left) {
        left_ = left;
    }

    void HammanTreeNode::set_right_son(spHammanTreeNode right) {
        right_ = right;
    }
}
#endif	/* HAMMANTREENODE_H */

