CXX      = g++
LINK     = ld
# OpenBeagle includes, PACC
IBEAGLE  = -Iincludes/BEAGLE
IPACC    = -Iincludes/PACC
# SQLite includes
ISQLITE  = -Iincludes/SQLITE
# OpenBeagle libraries, PACC
LBEAGLE  = -lbeagle-3.0.3 -lbeagle-GP-3.0.3 -lbeagle-GA-3.0.3 -lbeagle-Coev-3.0.3
LPACC    = -lpacc-xml-1.3.1b -lpacc-util-1.3.1b -lpacc-threading-1.3.1b -lpacc-svg-1.3.1b -lpacc-socket-1.3.1b -lpacc-math-1.3.1b
# SQLite 3 libraries
LSQLITE  = -lsqlite3
LIBS     = -L. -L/Users/aco/local/lib $(LBEAGLE) $(LPACC) $(LSQLITE)
#CXXFLAGS = -g -O2 -Wall -Wno-uninitialized ${IBEAGLE} $(IPACC)
CXXFLAGS = -O2 -pipe -fomit-frame-pointer -Wall -Wno-uninitialized ${IBEAGLE} $(IPACC) $(ISQLITE)
OBJ      = 	eval.o LessThan.o GreaterThan.o IfThenElse.o Avg.o ConstantDouble.o \
			Max.o Min.o Eq.o Context.o EphemeralDay.o Primitives.o ROC.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@
	
prog : main.o $(OBJ)
	$(CXX) -o prog.exe main.o $(OBJ) $(LIBS)

ind : individual.o $(OBJ)
	$(CXX) -o ind.exe individual.o $(OBJ) $(LIBS)

all : prog ind

clean:
	rm -f prog.exe ind.exe *.o


