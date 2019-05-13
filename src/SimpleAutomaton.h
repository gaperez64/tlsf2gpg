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

#ifndef TLSF2GPG_SIMPLEAUTOMATON
#define TLSF2GPG_SIMPLEAUTOMATON

#include <list>
#include <vector>

#include "cuddObj.hh"

class SimpleAutomaton {
public:
    unsigned int start;
    std::vector<std::list<unsigned int> > priorities;
    std::vector<std::list<std::pair<BDD, unsigned int> > > successors;
    unsigned int numStates() {
        assert(priorities.size() == successors.size());
        return priorities.size();
    }
    bool isComplete();
};

#endif
