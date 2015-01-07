THRIFT_DIR = /usr/local/thrift/include
LIB_DIR = /usr/local/thrift/lib
GEN_DIR = ../gen-cpp

VPATH = $(GEN_DIR)

INC_PATH = -I${THRIFT_DIR} -I$(GEN_DIR) -I../include
GEN_SRC = entry_thrift_types.cpp entry_thrift_constants.cpp EntryThriftSvr.cpp
ADD_SRC = $(GEN_SRC)
LIBS = -L${LIB_DIR} -lthrift 

CPP = g++
CFLAGS       = -fPIC -Wall
CPPFLAGS       = -g -O0 -Wall -std=c++11 $(INC_PATH) -D Debug -DDEBUG
DEBUGFLAGS   = -g -O0 -D_DEBUG -DDEBUG -D DEBUG
RELEASEFLAGS = -O2 -DNDEBUG
LDFLAGS      =  

TARGET  = server 
SOURCES = $(shell echo *.cpp $(ADD_SRC))
HEADERS = $(shell echo *.h)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(FLAGS) $(CPPFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o $(TARGET) $(PROTO_PATH)/*.o


#default: server
#server: server.cpp
#	g++ -g -o server -I${THRIFT_DIR} -I${BOOST_DIR}  -I$(GEN_SRC) -L${LIB_DIR} -lthrift server.cpp ${GEN_SRC}
