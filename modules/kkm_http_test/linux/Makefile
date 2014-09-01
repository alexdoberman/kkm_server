CXX            = g++
DEFINES        = -Dlinux
CXXFLAGS       = -pipe -std=c++11 -g -Wall -W  -fPIE $(DEFINES)
LDFLAGS        = -Wl,-rpath,/home/alex/Qt5.2.1/5.2.1/gcc_64 -Wl,-rpath,/home/alex/Qt5.2.1/5.2.1/gcc_64/lib
SOURCES        = ../src/main.cpp 

OBJECTS        = $(SOURCES:.cpp=.o)

INCPATH        =-I../../../3rdparty/poco-1.4.6p4-all/Foundation/include \
		-I../../../3rdparty/poco-1.4.6p4-all/Net/include \
		-I../../../3rdparty/poco-1.4.6p4-all/Util/include \
		-I../../../3rdparty/poco-1.4.6p4-all/XML/include 

LIBS           =-L../../../3rdparty/poco-1.4.6p4-all/lib/Linux/x86_64 -lPocoFoundation \
		-L../../../3rdparty/poco-1.4.6p4-all/lib/Linux/x86_64 -lPocoNet \
		-L../../../3rdparty/poco-1.4.6p4-all/lib/Linux/x86_64 -lPocoUtil \
		-L../../../3rdparty/poco-1.4.6p4-all/lib/Linux/x86_64 -lPocoXML 


EXECUTABLE=../../../builds/lib/kkm_http_test

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $< -o $@

clean:
	rm -rf ../src/*.o $(EXECUTABLE)
