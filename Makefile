LIB = -lm 
SRC := $(wildcard *.cpp)
OBJS := $(wildcard *.o)
OBJ = $(SRC:%.cpp=%.o)
CPPFLAGS +=  -I ../ -ggdb -g3
DEPS = $(OBJ:%.o=%.d)
DEPEND = g++ -MM -MG -I ../


all: $(OBJ) libcsi.so

-include $(OBJS:.o=.d)

%.o: %.cpp
	g++ -c -fPIC $(CPPFLAGS)  $<  -o $@
	$(DEPEND) $(INC) $< > $*.d

libcsi.so: $(OBJ)
	g++ -shared -o libcsi.so $(OBJ) 

all:
	make libcsi.so
	make -C kcdc/
	make -C test/

clean:
	make -C kcdc/ clean
	make -C test/ clean
	rm -f *.o *.d libcsi.so

