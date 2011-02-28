CXX      = g++
LINK     = ld
# OpenBeagle includes, PACC
IBEAGLE  = -I/Users/aco/local/Cellar/beagle/3.0.3/include
IPACC    = -I/Users/aco/local/Cellar/pacc/1.4.1/include
# SQLite includes
ISQLITE  = 
# OpenBeagle libraries, PACC
LBEAGLE  = -lbeagle -lbeagle-GP -lbeagle-GA -lbeagle-Coev
LPACC    = -lpacc-xml -lpacc-util -lpacc-threading -lpacc-svg -lpacc-socket -lpacc-math
# SQLite 3 libraries
LSQLITE  = -lsqlite3
LIBS     = -L. -L/Users/aco/local/lib $(LBEAGLE) $(LPACC) $(LSQLITE)
#CXXFLAGS = -g -O2 -Wall -Wno-uninitialized ${IBEAGLE} $(IPACC)
CXXFLAGS = -O2 -pipe -fomit-frame-pointer -Wall -Wno-uninitialized ${IBEAGLE} $(IPACC) $(ISQLITE)
OBJ      = 	eval.o LessThan.o GreaterThan.o IfThenElse.o Avg.o ConstantDouble.o \
			Max.o Min.o Eq.o Abs.o Context.o EphemeralDay.o Primitives.o ROC.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

prog : main.o $(OBJ)
	$(CXX) -o prog.exe main.o $(OBJ) $(LIBS)

ind : individual.o $(OBJ)
	$(CXX) -o ind.exe individual.o $(OBJ) $(LIBS)

all : prog ind

clean:
	rm -f prog.exe ind.exe *.o


