THRIFT_DIR = /usr/local/include
LIB_DIR = /usr/local/thrift/lib
TOP_DIR = ../../build
VPATH = $(GEN_DIR)

INC_PATH = -I${THRIFT_DIR} -I$(TOP_DIR)/entry_thrift
GEN_SRC = 
LIBS = -L$(TOP_DIR)/lib -lsimentry_thrift  -lboost_system -lboost_thread \
       -L${LIB_DIR} -lthrift -lthriftnb -levent

CPP = g++
CFLAGS       = -fPIC -Wall
CPPFLAGS       = -g -O0 -Wall -std=c++11 $(INC_PATH) -D Debug -DDEBUG -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H 
DEBUGFLAGS   = -g -O0 -D_DEBUG -DDEBUG -D DEBUG
RELEASEFLAGS = -O2 -DNDEBUG
LDFLAGS      =  
TARGET  = client
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
