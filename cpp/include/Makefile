THRIFT_DIR = /usr/local/thrift/include
LIB_DIR = /usr/local/thrift/lib
GEN_DIR = ../gen-cpp
TARGET_PATH = ../..
VPATH = $(GEN_DIR)

INC_PATH = -I${THRIFT_DIR} -I$(GEN_DIR) -I../include
GEN_SRC = entry_thrift_types.cpp entry_thrift_constants.cpp EntryThriftSvr.cpp
ADD_SRC = $(GEN_SRC)

CPP = g++
CFLAGS       = -fPIC -Wall
CPPFLAGS       = -g -O3 -Wall -std=c++11 $(INC_PATH) -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H 
DEBUGFLAGS   = -g -O0 -D_DEBUG -DDEBUG -D DEBUG
RELEASEFLAGS = -O2 -DNDEBUG
LDFLAGS      =  

SOURCES=$(shell echo $(ADD_SRC))
HEADERS=$(shell echo *.h)
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=libsimentry_thrift.a
all: $(TARGET)
$(TARGET) : $(OBJECTS)
	$(AR) -rc $(TARGET) $(OBJECTS)
	mkdir -p $(TARGET_PATH)/lib
	cp $(TARGET) $(TARGET_PATH)/lib
	mkdir -p $(TARGET_PATH)/include
	cp *.h $(TARGET_PATH)/include

clean:
	rm -f *.o $(TARGET) $(PROTO_PATH)/*.o


#default: server
#server: server.cpp
#	g++ -g -o server -I${THRIFT_DIR} -I${BOOST_DIR}  -I$(GEN_SRC) -L${LIB_DIR} -lthrift server.cpp ${GEN_SRC}
