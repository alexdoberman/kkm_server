CXX            = g++
DEFINES        = -Dlinux
CXXFLAGS       = -pipe -std=c++11 -O2 -Wall -W  -fPIE $(DEFINES)
LDFLAGS        = -Wl,-rpath,/home/user/Qt5.2.1/5.2.1/gcc -Wl,-rpath,/home/user/Qt5.2.1/5.2.1/gcc/lib
SOURCES        = ../src/main.cpp \
		../src/kkm_server.cpp \
		../src/kkm_server_engine.cpp \
		../src/reply_writer.cpp \
		../src/request_parser.cpp \
		../src/request_processor.cpp \
		../src/request_engine.cpp 

OBJECTS        = $(SOURCES:.cpp=.o)

INCPATH        =-I../../../3rdparty/poco-1.4.6p4/Foundation/include \
		-I../../../3rdparty/poco-1.4.6p4/Net/include \
		-I../../../3rdparty/poco-1.4.6p4/Util/include \
		-I../../../3rdparty/poco-1.4.6p4/XML/include \
		-I../../../3rdparty/boost_1_55_0 \
		-I../../kkm_driver/include

LIBS           =-L../../../3rdparty/poco-1.4.6p4/lib/Linux/i686 -lPocoFoundation \
		-L../../../3rdparty/poco-1.4.6p4/lib/Linux/i686 -lPocoNet \
		-L../../../3rdparty/poco-1.4.6p4/lib/Linux/i686 -lPocoUtil \
		-L../../../3rdparty/poco-1.4.6p4/lib/Linux/i686 -lPocoXML \
		-L../../../builds/lib -lkkm_driver 


EXECUTABLE=../../../builds/lib/kkm_server

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $< -o $@

clean:
	rm -rf ../src/*.o $(EXECUTABLE)
