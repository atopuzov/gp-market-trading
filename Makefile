CXX      = g++
LINK     = ld
# OpenBeagle includes, PACC
IBEAGLE  = -Iincludes/BEAGLE
IPACC    = -Iincludes/PACC
# SQLite includes
ISQLITE  = -Iincludes/SQLITE
# OpenBeagle libraries, PACC
LBEAGLE  = -llibbeagle -llibbeagle-GP -llibbeagle-GA -llibbeagle-Coev
LPACC    = -llibpacc-xml -llibpacc-util -llibpacc-threading -llibpacc-svg -llibpacc-socket -llibpacc-math
# SQLite 3 libraries
LSQLITE  = -lsqlite3
LIBS     = -L. -Llibs $(LBEAGLE) $(LPACC) $(LSQLITE)
#CXXFLAGS = -g -O2 -Wall -Wno-uninitialized ${IBEAGLE} $(IPACC)
CXXFLAGS = -O2 -march=i686 -mtune=pentium4 -pipe -fomit-frame-pointer -Wall -Wno-uninitialized ${IBEAGLE} $(IPACC) $(ISQLITE)
OBJ      = 	eval.o LessThan.o GreaterThan.o IfThenElse.o Avg.o \
			Max.o Min.o Eq.o Context.o EphemeralDay.o Primitives.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@
	
prog : main.o $(OBJ)
	$(CXX) -o prog.exe main.o $(OBJ) $(LIBS)

ind : individual.o $(OBJ)
	$(CXX) -o ind.exe individual.o $(OBJ) $(LIBS)

all : prog ind

clean:
	rm -f prog.exe ind.exe *.o


