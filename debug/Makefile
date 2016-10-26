#Makefile version 1

CC = gcc
CXX = g++

TARGET = ./chen_server

LDFLAGS +=  -levent \
            -lpthread 
			
INCLUDE += ../include \

OPTIONS = -g -Wall


OBJS =  ./Main.o \
	./ChenServer.o \
	./ChenLock.o \
	./ChenSpinLock.o

										 
target : $(TARGET)

$(TARGET) :  $(OBJS)
	$(CXX) -o $(@) $(^) -I $(INCLUDE) $(LDFLAGS)
 
./%.o : ../src/%.cpp
	$(CXX) $(OPTIONS) -o $(@) -c $(<) -I $(INCLUDE) $(LDFLAGS)


.PHONY : clean
clean : 
	rm -rf $(OBJS) $(TARGET)
