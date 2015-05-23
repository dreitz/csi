LIB = -lm 
SRC := $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)
CPPFLAGS +=  -I ../ -ggdb -g3
DEPS = $(OBJ:%.o=%.d)
DEPEND = g++ -MM -MG -I ../

all:

-include $(DEPS)

libcsi: $(OBJ)
	g++ -shared -o libcsi.so $(OBJ) 

%.d: %.cpp
	$(DEPEND) $(INC) $< > $@

%.o: %.cpp 
	g++ -fPIC -c $(CPPFLAGS) $(LIB) $< -o $@


all:
	make libcsi
	make -C kcdc/
	make -C test/


clean:
	make -C kcdc/ clean
	make -C test/ clean
	rm -f *.o *.d libcsi.so

