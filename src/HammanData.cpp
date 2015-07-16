/* 
 * File:   HammanData.cpp
 * Author: s
 * 
 * Created on July 15, 2015, 7:50 PM
 */

#include "HammanData.h"
#include <cassert>
#include <cstddef> //nullptr

namespace LobKo {

    HammanData::HammanData(const RawPage& inputPage) {
        character_frequency_ = {0};
        build_character_frequency_array(inputPage.buffer(), inputPage.size());
        //        character_to_node_map_.fill(HammanTreeNode::get_node());
        for ( uint32_t character = 0; character < 256; ++character ) {
            if ( character_frequency_[character] != 0 ) {
                character_to_node_map_[character] = HammanTreeNode::get_node(character_frequency_[character]);
                priority_q_.push(character_to_node_map_[character]);
            }
        }
        while (priority_q_.size() > 1) {
            spHammanTreeNode p1 = priority_q_.top();
            priority_q_.pop();
            spHammanTreeNode p2 = priority_q_.top();
            priority_q_.pop();

            priority_q_.push(HammanTreeNode::get_node(p1->frequency() + p2->frequency(), p2, p1));
        }
        if ( !priority_q_.empty() ) {
            hamman_tree_root_ = priority_q_.top();
            priority_q_.pop();
            //calc_bit_bunch(hamman_tree_root_, spBitBunch(new BitBunch()), BitBunch::NONE);
        }


    }

    HammanData::~HammanData() {
    }

    void HammanData::build_character_frequency_array(const uint8_t * buffer, const uint32_t size) {
        assert(buffer && "BUFFER is zero pointer");

        for ( uint32_t i = 0; i < size; ++i ) {
            ++character_frequency_[ buffer[i] ];
        }
    };

//    void HammanData::calc_bit_bunch(spHammanTreeNode node, const spBitBunch bits_sequence, BitBunch::add_behavior behavior) {
//        node->bits_sequence = bits_sequence;
//        if ( behavior == BitBunch::ADD_ONE ) {
//            node->bits_sequence->append_one_bit();
//        } else if ( behavior == BitBunch::ADD_ZERO ) {
//            node->bits_sequence->append_zero_bit();
//        }
//        if ( node->left_.get() != nullptr ) {
//            calc_bit_bunch(node->left_, node->bits_sequence, BitBunch::ADD_ZERO);
//        }
//        if ( node->right_.get() != nullptr ) {
//            calc_bit_bunch(node->right_, node->bits_sequence, BitBunch::ADD_ONE);
//        }
//
//    }
}