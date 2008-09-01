CXX      = g++
LINK     = ld
CXXFLAGS = -O2 -I.. -Wall -Wno-uninitialized
OBJ      = plot.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@


prog.exe : $(OBJ)
	$(CXX) -o prog.exe -L. -lsqlite3 $(OBJ) 

all:	prog.exe

clean:
	rm -f prog.exe *.o *.png *.dat
