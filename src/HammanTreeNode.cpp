/* 
 * File:   HammanTreeNode.cpp
 * Author: s
 * 
 * Created on July 15, 2015, 9:48 PM
 */

#include "HammanTreeNode.h"
namespace LobKo {

    HammanTreeNode::HammanTreeNode(uint32_t frequency) :
    frequency_(frequency) {
#ifndef NDEBUG
        characters = "AUX";
#endif
    }

    HammanTreeNode::~HammanTreeNode() {
#ifndef NDEBUG
        //std::cout << "~()" << characters << "\n";
#endif
    }

    spHammanTreeNode HammanTreeNode::get_node(uint32_t frequency) {
        return spHammanTreeNode(new HammanTreeNode(frequency));
    };

    spHammanTreeNode HammanTreeNode::get_node(uint32_t frequency, spHammanTreeNode left, spHammanTreeNode right) {
        spHammanTreeNode sp(new HammanTreeNode(frequency));
        sp->set_left_son(left);
        sp->set_right_son(right);

        return sp;
    };
}
