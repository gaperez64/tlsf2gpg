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

#include <iostream>
#include <queue>
#include <algorithm>

#include "SimpleArena.h"

void SimpleArena::print() {
    std::cout << "parity " << this->numVertices() - 1 << ";" << std::endl;
    for (int i = 0; i < this->numVertices(); i++) {
        std::cout << i << " ";
        auto first_priority = this->priorities[i].begin();
        assert(first_priority != this->priorities[i].end());
        std::cout << first_priority;
        // for the rest we pad with commas
        std::for_each(first_priority, this->priorities[i].end(),
                      [] (unsigned int& p) { std::cout << "," << p;});
        std::cout << " " << (int)this->protagonist_vertex[i] << " ";
        auto first_successor = this->successors[i].begin();
        assert(first_succesor != this->successors[i].end());
        std::cout << first_successor;
        // again we pad with commas
        std::for_each(this->successors[i].begin(), this->successors[i].end(),
                      [] (unsigned int& s) { std::cout << "," << s;});
        std::cout << " \"" << i << "\"" << std::endl;
    }
}

bool SimpleArena::isComplete() {
    for (auto succ = successors.begin(); succ != successors.end(); succ++)
        if (succ->size() == 0)
            return false;
    return true;
}

bool SimpleArena::isReachable() {
    bool visited[this->numVertices()];
    for (unsigned int i = 1; i < this->numVertices(); i++)
        visited[i] = false;
    std::queue<unsigned int> to_visit;
    to_visit.push(0);
    visited[0] = true;
    while (to_visit.size() > 0) {
        unsigned int vertex = to_visit.front();
        to_visit.pop();
        for (auto next_vertex = successors[vertex].begin();
                  next_vertex != successors[vertex].end(); next_vertex++) {
            if (!visited[*next_vertex]) {
                to_visit.push(*next_vertex);
                visited[*next_vertex] = true;
            }
        }
    }
    for (unsigned int i = 0; i < this->numVertices(); i++)
        if (!visited[i])
            return false;
    return true;
}
