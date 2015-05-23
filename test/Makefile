LIB = -lm -lboost_unit_test_framework-mt ../libcsi.so
SRC := $(wildcard *.cpp)
OBJS := $(wildcard *.o)
OBJ = $(SRC:%.cpp=%.o)
EXE = $(SRC:%.cpp=%)
CPPFLAGS +=  -I ../ -ggdb -g3
DEPS = $(OBJ:%.o=%.d)
DEPEND = g++ -MM -MG -I ../

all: $(OBJ) $(EXE)

-include $(OBJS:.o=.d)

%.o: %.cpp 
	g++ -c $(CPPFLAGS)  $<  -o $@
	$(DEPEND) $(INC) $< > $*.d

%: %.o 
	g++ $(CPPFLAGS) $(LIB) $<  -o $@
	./$@

clean:
	rm -f *.test *.o *.d
	
