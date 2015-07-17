/* 
 * File:   HammanTreeNodeCopmarision.h
 * Author: s
 *
 * Created on July 16, 2015, 7:47 PM
 */

#ifndef HAMMANTREENODECOPMARISION_H
#define	HAMMANTREENODECOPMARISION_H
#include <memory>
#include "HammanTreeNode.h"

namespace LobKo {
    class HammanTreeNode;
    //typedef std::shared_ptr<HammanTreeNode> spHammanTreeNode;

    struct HammanTreeNodeCopmarision {

        bool operator()(std::shared_ptr<HammanTreeNode> a, std::shared_ptr<HammanTreeNode> b);
//        {
//            if (a->frequency() > b->frequency()) {
//                return true;
//            }
//            return false;
//        }
    };
}
#endif	/* HAMMANTREENODECOPMARISION_H */

