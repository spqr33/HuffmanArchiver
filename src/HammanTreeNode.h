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
#include <cassert>


namespace LobKo {
    class HammanTreeNode;
    class HammanData;
    typedef std::shared_ptr<HammanTreeNode> spHammanTreeNode;

    class HammanTreeNode {
        friend class HammanData;
        friend struct HammanTreeNodeCopmarision;
    public:
        explicit HammanTreeNode(uint32_t frequency);
        virtual ~HammanTreeNode();

        static spHammanTreeNode get_node(uint32_t frequency);
        static spHammanTreeNode get_node(uint32_t frequency, uint8_t character);
        static spHammanTreeNode get_node(uint32_t frequency, spHammanTreeNode left, spHammanTreeNode right);
        inline uint32_t frequency() const;
        inline void set_left_son(spHammanTreeNode left);
        inline void set_right_son(spHammanTreeNode left);
        inline spHammanTreeNode get_left_son() const;
        inline spHammanTreeNode get_right_son() const;
        inline uint8_t get_character() const;
        inline void set_character(uint8_t new_character);
        inline bool is_leaf() const;
        inline void set_leaf(bool n);
//#ifndef NDEBUG        
        std::string characters_;
//#endif
    private:
        uint32_t frequency_;
        spHammanTreeNode left_;
        spHammanTreeNode right_;
        spBitBunch bits_sequence;
        uint8_t character_;
        bool is_leaf_;

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

    spHammanTreeNode HammanTreeNode::get_left_son() const {
        return left_;
    }

    spHammanTreeNode HammanTreeNode::get_right_son() const {
        return right_;
    }

    uint8_t HammanTreeNode::get_character() const {
        return character_;
    }

    void HammanTreeNode::set_character(uint8_t new_character) {
        character_ = new_character;
    }

    bool HammanTreeNode::is_leaf() const {
        return is_leaf_;
    };
    void HammanTreeNode::set_leaf(bool n) {
        is_leaf_ = n;
    }

}
#endif	/* HAMMANTREENODE_H */

