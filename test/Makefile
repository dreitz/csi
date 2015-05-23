LIB = -lm -lboost_unit_test_framework-mt ../libcsi.so
SRC := $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)
EXE = $(SRC:%.cpp=%)
CPPFLAGS +=  -I ../ -ggdb -g3
DEPS = $(OBJ:%.o=%.d)
DEPEND = g++ -MM -MG -I ../

all: $(EXE)

-include $(DEPS)


%.d: %.cpp
	$(DEPEND) $(INC) $< > $@

%.o: %.cpp 
	g++ -c $(CPPFLAGS)  $<  -o $@

%: %.o 
	g++ $(CPPFLAGS) $(LIB) $<  -o $@
	./$@

clean:
	rm -f *.test *.o *.d
	
