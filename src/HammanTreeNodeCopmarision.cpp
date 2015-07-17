/* 
 * File:   HammanTreeNodeCopmarision.cpp
 * Author: s
 * 
 * Created on July 16, 2015, 7:47 PM
 */

#include "HammanTreeNodeCopmarision.h"
namespace LobKo {

    bool HammanTreeNodeCopmarision::operator()(std::shared_ptr<HammanTreeNode> a, std::shared_ptr<HammanTreeNode> b) {
        if ( a->frequency() > b->frequency() ) {
            return true;
        }
        return false;
    }
    
}