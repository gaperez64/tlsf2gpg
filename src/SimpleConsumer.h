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

#ifndef TLSF2GPG_SIMPLECONSUMER
#define TLSF2GPG_SIMPLECONSUMER

#include <map>
#include <list>
#include <vector>

#include "cpphoafparser/consumer/hoa_consumer.hh"

#include "cuddObj.hh"

#include "SimpleAutomaton.h"

class SimpleConsumer : public cpphoafparser::HOAConsumer {
private:
    const Cudd& mgr;
    const std::map<std::string, unsigned int>& global_signals;
    SimpleAutomaton& automaton;
    std::vector<unsigned int> signals;
    BDD labelBDD(label_expr::ptr labelExpr);

public:
    SimpleConsumer(Cudd& mgr, std::map<std::string,
                   unsigned int>& g_sig,
                   SimpleAutomaton& a) : mgr(mgr),
                                         global_signals(g_sig),
                                         automaton(a) {}
    virtual void addStartStates(const int_list& stateConjunction) override;
    virtual void setAPs(const std::vector<std::string>& aps) override;
    virtual void addState(unsigned int id,
                          std::shared_ptr<std::string> info,
                          label_expr::ptr labelExpr,
                          std::shared_ptr<int_list> accSignature) override;
    virtual void addEdgeWithLabel(unsigned int stateId,
                                  label_expr::ptr labelExpr,
                                  const int_list& conjSuccessors,
                                  std::shared_ptr<int_list> accSignature) override;
    // The methods below do nothing or just check that they are not called
    virtual void setNumberOfStates(unsigned int numberOfStates) override {}
    virtual void addEdgeImplicit(unsigned int stateId,
                                 const int_list& conjSuccessors,
                                 std::shared_ptr<int_list> accSignature) override {
        // there should be no implicit edges
        assert(false);
    }
    virtual bool parserResolvesAliases() override {
        return false;
    }
    virtual void notifyHeaderStart(const std::string& version) override {}
    virtual void addAlias(const std::string& name,
                          label_expr::ptr labelExpr) override {
        // there should be no aliases
        assert(false);
    }
    virtual void setAcceptanceCondition(unsigned int numberOfSets,
                                        acceptance_expr::ptr accExpr) override {}
    virtual void provideAcceptanceName(const std::string& name,
        const std::vector<cpphoafparser::IntOrString>& extraInfo) override {}
    virtual void setName(const std::string& name) override {}
    virtual void setTool(const std::string& name,
                         std::shared_ptr<std::string> version) override {}
    virtual void addProperties(const std::vector<std::string>& properties)
        override {}
    virtual void addMiscHeader(const std::string& name,
        const std::vector<cpphoafparser::IntOrString>& content) override {}
    virtual void notifyBodyStart() override {}
    virtual void notifyEndOfState(unsigned int stateId) override {}
    virtual void notifyEnd() override {}
    virtual void notifyAbort() override {}
    virtual void notifyWarning(const std::string& warning) override {}
};

#endif
