//==============================================================================
//
//  Copyright (c) 2019-
//  Authors:
//  * Guillermo A. Perez <guillermoalberto.perez@uantwerpen.be>
//
//  Based on the cpphoaf parser by Joachim Klein and David Mueller
//
//------------------------------------------------------------------------------
//
//  This file uses the cpphoafparser library,
//      http://automata.tools/hoa/cpphoafparser/
//
//  The cpphoafparser library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  The cpphoafparser library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
//==============================================================================

#include <map>
#include <list>
#include <vector>

#include "cpphoafparser/ast/boolean_expression.hh"
#include "cpphoafparser/consumer/hoa_consumer.hh"

#include "cuddObj.hh"

#include "SimpleAutomaton.h"
#include "SimpleConsumer.h"

void SimpleConsumer::addStartStates(const int_list& stateConjunction) {
    bool first = true;
    for (unsigned int state : stateConjunction) {
        assert(first);
        automaton.start = state;
        first = false;
    }
}

void SimpleConsumer::setAPs(const std::vector<std::string>& aps) {
    for (const std::string& ap : aps) {
        auto sindex = global_signals.find(ap);
        assert(sindex != global_signals.end());
        signals.push_back(sindex->second);
    }
}

void SimpleConsumer::addState(unsigned int id,
                     std::shared_ptr<std::string> info,
                     label_expr::ptr labelExpr,
                     std::shared_ptr<int_list> accSignature) {
    assert(accSignature);
    assert(labelExpr ? false : true);  // the pointer should be null
    // we create the list of successors
    automaton.successors.push_back({});
    // and we also recover the priority
    bool first = true;
    for (unsigned int acc : *accSignature) {
        assert(first && automaton.priorities.size() == id);
        automaton.priorities.push_back({acc});
        first = false;
    }
}

BDD SimpleConsumer::labelBDD(label_expr::ptr labelExpr) {
    // for binary operators, just operate on getLeft(), getRight()
    switch (labelExpr->getType()) {
        case label_expr::EXP_AND:
            return this->labelBDD(labelExpr->getLeft()) &
                this->labelBDD(labelExpr->getRight());
        case label_expr::EXP_OR:
            return this->labelBDD(labelExpr->getLeft()) |
                this->labelBDD(labelExpr->getRight());
        case label_expr::EXP_NOT:  // right child is empty
            return ~this->labelBDD(labelExpr->getLeft());
        case label_expr::EXP_TRUE:
            return this->mgr.bddOne();
        case label_expr::EXP_FALSE:
            return this->mgr.bddZero();
        case label_expr::EXP_ATOM:  // getAtom()
            return this->mgr.bddVar(
                this->signals[labelExpr->getAtom().getAPIndex()]);
    }
}

void SimpleConsumer::addEdgeWithLabel(unsigned int stateId,
                                      label_expr::ptr labelExpr,
                                      const int_list& conjSuccessors,
                                      std::shared_ptr<int_list> accSignature) {
    assert(labelExpr ? true : false);  // the pointer should not be null

    bool first = true;
    for (unsigned int succ : conjSuccessors) {
        assert(first);
        automaton.successors[stateId].push_back(std::make_pair(
            this->labelBDD(labelExpr), succ));
        first = false;
    }
    
    assert(!accSignature);
}
