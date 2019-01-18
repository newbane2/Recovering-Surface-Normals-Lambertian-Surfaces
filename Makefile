########################################
##
## Makefile
## LINUX compilation 
##
##############################################





#FLAGS
C++FLAG = -g -std=c++11

MATH_LIBS = -lm

EXEC_DIR=.


.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I. 

#-->All libraries (without LEDA)
LIBS_ALL =  -L/usr/lib -L/usr/local/lib 


#First Program (ListTest)

Cpp_OBJ=image.o image_demo.o

PROGRAM_NAME=image_demo

$(PROGRAM_NAME): $(Cpp_OBJ)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ) $(INCLUDES) $(LIBS_ALL)


all: 
	make s1
	make s2
	make s3 
	make s4

s1: 
	g++ $(C++FLAG) -I.   -c image.cc -o image.o
	g++ $(C++FLAG) -I.   -c s1.cpp -o s1.o
	g++ $(C++FLAG) -o s1 s1.cpp image.cc

s2: 
	g++ $(C++FLAG) -I.   -c image.cc -o image.o
	g++ $(C++FLAG) -I.   -c s2.cpp -o s2.o
	g++ $(C++FLAG) -o s2 s2.cpp image.cc
	
s3:
	g++ $(C++FLAG) -I.   -c image.cc -o image.o
	g++ $(C++FLAG) -I.   -c s3.cpp -o s3.o
	g++ $(C++FLAG) -o s3 s3.cpp image.cc

s4:
	g++ $(C++FLAG) -I.   -c image.cc -o image.o
	g++ $(C++FLAG) -I.   -c s4.cpp -o s4.o
	g++ $(C++FLAG) -o s4 s4.cpp image.cc
	
clean:
	(rm -f *.o; rm s1)
	(rm -f *.o; rm s2)
	(rm -f *.o; rm s3)
	(rm -f *.o; rm s4)
(:
