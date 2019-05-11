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
    std::list<std::string> inputs;
    std::list<std::string> outputs;
    while (!arguments.empty()) {
        const std::string& arg = arguments.front();
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
                break;
            case Section::Outputs:
                outputs.push_back(arg);
                break;
            default:
                return usage("Unexpected value " + arg +
                             " before argument sections");
        }
    }
    if (files.size() == 0 || inputs.size() == 0 || outputs.size() == 0) {
        return usage("All three argument lists must be non-empty!");
    }
    // Print out some information about the arguments
    auto print = [](const std::string& s) { std::cout << " " << s; };
    std::cout << "Files:";
    std::for_each(files.begin(), files.end(), print);
    std::cout << std::endl << "Inputs:";
    std::for_each(inputs.begin(), inputs.end(), print);
    std::cout << std::endl << "Outputs:";
    std::for_each(outputs.begin(), outputs.end(), print);
    std::cout << std::endl;
//
//  std::string filename = arguments.front();
//  arguments.pop();
//  std::shared_ptr<std::ifstream> f_in;
//  if (filename != "-") {
//    f_in.reset(new std::ifstream(filename.c_str()));
//    if (!*f_in) {
//      std::cerr << "Error opening file " + filename << std::endl;
//      return 1;
//    }
//  }
//  std::istream& in = (filename == "-" ? std::cin : *f_in);
//
//  if (verbose) {
//    std::cerr << "Reading from " + (filename != "-" ? "file "+filename : "standard input") << std::endl;
//  }
//
//  HOAConsumer::ptr consumer;
//  if (command == "print") {
//    consumer.reset(new HOAConsumerPrint(std::cout));
//  } else if (command == "parse") {
//    consumer.reset(new HOAConsumerNull());
//  } else {
//    return usage("Unknown command: "+command);
//  }
//
//  if (resolve_aliases) {
//    consumer.reset(new HOAIntermediateResolveAliases(consumer));
//  }
//
//  if (trace) {
//    consumer.reset(new HOAIntermediateTrace(consumer));
//  }
//
//  try {
//    HOAParser::parse(in, consumer, validate);
//
//    if (command == "parse") {
//      std::cout << "Parsing OK" << std::endl;
//    }
//
//    return 0;
//  } catch (HOAParserException& e) {
//    std::cerr << e.what() << std::endl;
//  } catch (HOAConsumerException& e) {
//    std::cerr << "Exception: " << e.what() << std::endl;
//  }
  return 0;
}


