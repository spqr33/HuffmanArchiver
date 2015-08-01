/* 
 * File:   HammanData.cpp
 * Author: s
 * 
 * Created on July 15, 2015, 7:50 PM
 */

#include "HammanData.h"
#include <cassert>
#include <cstddef> //nullptr
#include <iostream>

namespace LobKo {

    HammanData::HammanData(RawPage& inputPage) :
    priority_q_(hamman_tree_node_copmarision),
    character_frequency_({0}) {
        build_character_frequency_array(inputPage.buffer(), inputPage.size());
        build();
        hamman_tree_root_.reset();
    }

    HammanData::HammanData(const std::array<uint32_t, 256>& character_frequency) :
    priority_q_(hamman_tree_node_copmarision) {
        character_frequency_ = character_frequency;
        build();
        hamman_tree_root_.reset();
    }

    HammanData::HammanData(std::pair<uint8_t*, size_t > p) :
    priority_q_(hamman_tree_node_copmarision),
    character_frequency_({0}) {
        build_character_frequency_array(p.first, p.second);
        build();
        delete [] p.first;
    }

    HammanData::~HammanData() {
    }

    void HammanData::build_character_frequency_array(uint8_t * buffer, const uint32_t size) {
        assert(buffer && "BUFFER is zero pointer");

        for ( uint32_t i = 0; i < size; ++i ) {
            ++character_frequency_[ buffer[i] ];
        }

        for ( uint32_t i = 0; i < 256; ++i ) {
            if ( character_frequency_[i] != 0 ) {
                std::cout << "character_frequency[" << i << "]" << character_frequency_[i] << '\n';
            }
        }
    };

    void HammanData::calc_bit_bunch(spHammanTreeNode node, const spBitBunch bits_sequence, BitBunch::add_behavior behavior) {
#ifndef NDEBUG
        std::cout << "Char: " << node->characters << "  ";
#endif
        BitBunch* p = new BitBunch(*(bits_sequence.get()));
        node->bits_sequence = spBitBunch(p);


        if ( behavior == BitBunch::ADD_ONE ) {
            node->bits_sequence->append_one_bit();
        } else if ( behavior == BitBunch::ADD_ZERO ) {
            node->bits_sequence->append_zero_bit();
        }
#ifndef NDEBUG
        LobKo::BitBunch_dump(*(node->bits_sequence.get()));
#endif

        if ( node->left_.get() != nullptr ) {
            calc_bit_bunch(node->left_, node->bits_sequence, BitBunch::ADD_ZERO);
        }
        if ( node->right_.get() != nullptr ) {
            calc_bit_bunch(node->right_, node->bits_sequence, BitBunch::ADD_ONE);
        }

    }

    bool hamman_tree_node_copmarision(spHammanTreeNode a, spHammanTreeNode b) {
        if ( a->frequency() >= b->frequency() ) {
            return true;
        }
        return false;
    }

    spBitBunch HammanData::generate_bit_bunch(RawPage& inputPage) const {
        //#ifndef NDEBUG
        //        std::cout << "Generate Bunch started" << std::endl;
        //#endif


        spBitBunch sp_bit_bunch(new BitBunch());
        BitBunch& bit_bunch = *(sp_bit_bunch.get());

        uint8_t* buffer = inputPage.buffer();
        uint8_t* buffer_end = buffer + inputPage.size();

        for (; buffer != buffer_end; ++buffer ) {
            BitBunch& bits_correspond_to_sybmol = *(character_to_node_map_[*buffer]->bits_sequence.get());

            bit_bunch += bits_correspond_to_sybmol;
        }
        return sp_bit_bunch;
    }

    void HammanData::build() {
        for ( uint32_t character = 0; character < 256; ++character ) {
            if ( character_frequency_[character] != 0 ) {
                character_to_node_map_[character] =
                        HammanTreeNode::get_node(character_frequency_[character],
                        static_cast<uint8_t> (character));
#ifndef NDEBUG
                character_to_node_map_[character]->characters = character;
#endif
                priority_q_.push(character_to_node_map_[character]);
            }
        }
#ifndef NDEBUG
        std::cout << "Priority_q_ size: " << priority_q_.size() << "\n";
#endif
        while (priority_q_.size() > 1) {
            spHammanTreeNode p1 = priority_q_.top();
            priority_q_.pop();
            spHammanTreeNode p2 = priority_q_.top();
            priority_q_.pop();
            spHammanTreeNode node = HammanTreeNode::get_node(p1->frequency() + p2->frequency(), p1, p2);
#ifndef NDEBUG
            std::cout << "First popped char: " << p1->characters << "\n";
            std::cout << "Second popped char: " << p2->characters << "\n";
            node->characters = "(" + p1->characters + " " + p2->characters + ")";
#endif
            priority_q_.push(node);
        }
#ifndef NDEBUG        
        std::cout << "Priority_q_ size: " << priority_q_.size() << "\n";
#endif
        if ( priority_q_.size() == 1 ) {
            hamman_tree_root_ = priority_q_.top();
            priority_q_.pop();
            calc_bit_bunch(hamman_tree_root_, spBitBunch(new BitBunch()), BitBunch::NONE);
        }
    }

    spHammanTreeNode HammanData::get_hamman_tree_root() const {
        return hamman_tree_root_;
    }
}
