# CXXFLAGS for debugging:
# CXXFLAGS=-g -O1 -Wall

# CXXFLAGS for production
CXXFLAGS=-O3 -Wall

hoafs2gpg: src/hoafs2gpg.cpp include/cpphoafparser/*/*.hh
	$(CXX) $(CXXFLAGS) -I include --std=c++11 -o $@ $<
