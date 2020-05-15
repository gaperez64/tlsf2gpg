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

#include <algorithm>
#include <queue>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include "cpphoafparser/consumer/hoa_consumer_print.hh"
#include "cpphoafparser/consumer/hoa_consumer_null.hh"
#include "cpphoafparser/consumer/hoa_intermediate_trace.hh"
#include "cpphoafparser/consumer/hoa_intermediate_resolve_aliases.hh"
#include "cpphoafparser/parser/hoa_parser.hh"

#include "cuddObj.hh"

#include "SimpleAutomaton.h"
#include "SimpleConsumer.h"
#include "SimpleArena.h"

/** An enum for the argument sections */
enum class Section {None, Files, Inputs, Outputs};

/** The version */
static const char* version = "0.1";

/** Print version to out */
static void printVersion(std::ostream& out) {
    out << "hoafs2gpg v" << version;
    out << " (C) Copyright 2019- Guillermo A. Perez" << std::endl;
    out << "Use this tool to translate HOAF files encoding deterministic";
    out << std::endl;
    out << "parity automata into generalized parity games played on graphs.";
    out << std::endl;
}

/** Print usage information, and optionally error message */
static unsigned int usage(const std::string& error = "") {
    if (error != "") {
        printVersion(std::cerr);
        std::cerr << "ERROR: Command-line arguments error: " << error << std::endl;
        std::cerr << "Use argument '--help' to get usage information." << std::endl;
        return 2;
    }

    printVersion(std::cout);
    std::cout << "Argument lists are separated by spaces delimited by the following:";
    std::cout << std::endl;
    std::cout << " --files    A non-empty list HOAF files with deterministic";
    std::cout << " parity automata" << std::endl;
    std::cout << " --inputs   A list of uncontrollable input atomic propositions";
    std::cout << std::endl;
    std::cout << " --outputs  A list of controllable output atomic propositions";
    std::cout << std::endl;

    return (error != "" ? 2 : 0);
}

int main(int argc, char* argv[]) {
    std::queue<std::string> arguments;
    for (int i=1; i < argc; i++)
        arguments.push(argv[i]);

    if (arguments.size() == 0)
        return usage();

    Section sec = Section::None;
    std::list<std::string> files;
    std::vector<std::string> inputs;
    std::list<std::string> outputs;
    unsigned int sindex = 0;
    std::map<std::string, unsigned int> signal_table;
    while (!arguments.empty()) {
        std::string arg(arguments.front());
        arguments.pop();
        if (arg.compare(0, 2, "--") == 0) {
            if (arg == "--help") {
                return usage();
            } else if (arg == "--version") {
                printVersion(std::cout);
                return 0;
            } else if (arg == "--files") {
                sec = Section::Files;
            } else if (arg == "--inputs") {
                sec = Section::Inputs;
            } else if (arg == "--outputs") {
                sec = Section::Outputs;
            } else {
                return usage("Unknown argument " + arg);
            }
            continue;
        }
        // not an argument, are we inside an argument section?
        switch (sec) {
            case Section::Files:
                files.push_back(arg);
                break;
            case Section::Inputs:
                inputs.push_back(arg);
                signal_table[arg] = sindex++;
                break;
            case Section::Outputs:
                outputs.push_back(arg);
                signal_table[arg] = sindex++;
                break;
            default:
                return usage("Unexpected value " + arg +
                             " before argument sections");
        }
    }
    if (files.size() == 0 || inputs.size() == 0 || outputs.size() == 0) {
        return usage("All three argument lists must be non-empty!");
    }

    // Read all files into the automaton data structure
    Cudd mgr(0, 0);
    mgr.AutodynEnable(CUDD_REORDER_SIFT);
    std::vector<SimpleAutomaton> parity_automata;
    for (auto hoafauto = files.begin(); hoafauto != files.end(); hoafauto++) {
        std::shared_ptr<std::ifstream> in;
        in.reset(new std::ifstream(hoafauto->c_str()));
        if (!*in) {
            std::cerr << "Error opening file " + *hoafauto << std::endl;
            return 1;
        }

        cpphoafparser::HOAConsumer::ptr consumer;
        SimpleAutomaton a;
        consumer.reset(new SimpleConsumer(mgr, signal_table, a));

        try {
            cpphoafparser::HOAParser::parse(*in, consumer, true);
        } catch (cpphoafparser::HOAParserException& e) {
            std::cerr << e.what() << std::endl;
        } catch (cpphoafparser::HOAConsumerException& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }

        parity_automata.push_back(a);
    }

    // Create a product automaton by exploring the product
    std::queue<std::vector<unsigned int> > to_visit;
    std::vector<unsigned int> product_start;
    for (auto pa = parity_automata.begin(); pa != parity_automata.end(); pa++)
        product_start.push_back(pa->start);
    to_visit.push(product_start);

    // Start creating the product automaton
    SimpleAutomaton product;
    std::map<std::vector<unsigned int>, unsigned int> prod_indexes;
    prod_indexes[product_start] = 0;
    product.start = 0;
    /* Invariant: every element in to_visit has an entry in prod_indexes,
     * and its corresponding priorities and successors are not yet there
     */
    while (to_visit.size() > 0) {
        std::vector<unsigned int> state = to_visit.front();
        to_visit.pop();

        // create a list of successors and a list of priorities
        product.priorities.push_back({});
        product.successors.push_back({});

        // we load all priorities of the state
        for (int i = 0; i < state.size(); i++)
            product.priorities[prod_indexes[state]].push_back(
                parity_automata[i].priorities[state[i]].front());
        
        // we now explore all transitions in the product
        std::list<std::pair<BDD, unsigned int> >::iterator indices[state.size()];
        for (int i = 0; i < state.size(); i++)
            indices[i] = parity_automata[i].successors[state[i]].begin();
        while (indices[0] != parity_automata[0].successors[state[0]].end()) {
            // process the new transition if the BDDs allow it
            BDD prod_label = mgr.bddOne();
            for (int i = 0; i < state.size(); i++)
                prod_label &= indices[i]->first;
            if (prod_label != mgr.bddZero()) {
                // create the successor, check if it is in the index map, 
                // add the transition, and add it to the queue (if not in map)
                std::vector<unsigned int> succ;
                for (int i = 0; i < state.size(); i++)
                    succ.push_back(indices[i]->second);
                if (prod_indexes.find(succ) == prod_indexes.end()) {
                    to_visit.push(succ);
                    prod_indexes[succ] = prod_indexes.size();
                }
                product.successors[prod_indexes[state]].push_back(
                    std::make_pair(prod_label, prod_indexes[succ]));
            }
            // increase the indices in a "binary counter" fashion
            for (int i = state.size() - 1; i >= 0; i--) {
                indices[i]++;
                if (indices[i] != parity_automata[i].successors[state[i]].end()) {
                    break;
                } else if (i > 0) {  // the first digit is never reset
                    indices[i] = parity_automata[i].successors[state[i]].begin();
                }
            }
        }
    }
    assert(product.isComplete());

    // Start creating the generalized parity game
    // Step 1. generate all valuations of inputs
    std::list<BDD> input_vals;

    // we step through input valuations in a "binary counter" fashion
    // so we need a counter to remember the current valuation
    bool val[inputs.size()];
    for (int i = 0; i < inputs.size(); i++)
        val[i] = false;
        
    bool done = false;
    while (!done) {
        BDD valBDD = mgr.bddOne();
        for (int i = 0; i < inputs.size(); i++) {
            BDD varBDD = mgr.bddVar(signal_table[inputs[i]]);
            if (!val[i])
                varBDD = ~varBDD;
            valBDD &= varBDD;
        }
        input_vals.push_back(valBDD);
        // increase the counter
        for (int i = inputs.size() - 1; i >= 0; i--) {
            if (!val[i]) {
                val[i] = true;
                break;
            } else if (i > 0) { // the first digit is never reset
                val[i] = false;
                // and do not break, we need to do carries
            } else {
                done = true;
                break;
            }
        }
    }

    // Step 2. for all states in the product and all valuations, create
    // vertices of both players
    unsigned int nindex = 0;
    std::map<unsigned int, unsigned int> state2vertex;
    SimpleArena game;
    for (unsigned int state = 0; state < product.numStates(); state++) {
        if (state2vertex.find(state) == state2vertex.end()) {
            state2vertex[state] = nindex++;
            game.protagonist_vertex.push_back(true);
            game.successors.push_back({});
            game.priorities.push_back(product.priorities[state]);
        }
        // we step through all input valuations now
        for (auto val = input_vals.begin(); val != input_vals.end(); val++) {
            unsigned int nature_vertex = nindex++;
            game.protagonist_vertex.push_back(false);
            game.successors.push_back({});
            // the priorities of the new vertex are copied from the 
            // protagonist vertex
            game.priorities.push_back(product.priorities[state]);
            // connect from state to here
            game.successors[state2vertex[state]].push_back(nature_vertex);
            // connect from here to all successors of state whose transition
            // BDD is compatible with the valuation
            bool atleastone = false;
            for (auto succ = product.successors[state].begin();
                      succ != product.successors[state].end();
                      succ++) {
                // ignore this if the transition is not compatible
                if ((succ->first & *val) == mgr.bddZero())
                    continue;
                // otherwise recover info on the successor
                atleastone = true;
                unsigned int next_state = succ->second;
                if (state2vertex.find(next_state) == state2vertex.end()) {
                    state2vertex[next_state] = nindex++;
                    game.protagonist_vertex.push_back(true);
                    game.successors.push_back({});
                    game.priorities.push_back(product.priorities[next_state]);
                }
                // connect from the intermediate vertex to next state
                game.successors[nature_vertex].push_back(state2vertex[next_state]);
            }
            assert(atleastone);
        }
    }
    assert(game.isComplete());
    assert(game.isReachable());
    game.print();
    return 0;
}


