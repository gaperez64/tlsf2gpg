CXX=g++
CXXFLAGS=-std=c++11 -O3
HEADERS=src/SimpleAutomaton.h src/SimpleConsumer.h src/SimpleArena.h
SOURCES=src/SimpleConsumer.cpp src/hoafs2gpg.cpp \
	src/SimpleArena.cpp src/SimpleAutomaton.cpp
CUDD_PATH=cudd-2.5.1
CUDD_INCLUDE=$(CUDD_PATH)/include
CUDD_HDRS=$(CUDD_INCLUDE)/*.h
CUDD_LIBS=$(CUDD_PATH)/obj/libobj.a \
	  $(CUDD_PATH)/cudd/libcudd.a \
	  $(CUDD_PATH)/mtr/libmtr.a \
	  $(CUDD_PATH)/st/libst.a \
	  $(CUDD_PATH)/util/libutil.a \
	  $(CUDD_PATH)/epd/libepd.a
CPPHOAFP_INCLUDE=include
CPPHOAFP_HDRS=$(CPPHOAFP_INCLUDE)/cpphoafparser/*/*.hh

hoafs2gpg: $(HEADERS) $(SOURCES) $(CUDD_HDRS) $(CUDD_LIBS) $(CPPHOAFP_HDRS)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(CUDD_LIBS) -o hoafs2gpg \
		-I $(CUDD_INCLUDE) \
		-I $(CPPHOAFP_INCLUDE)
