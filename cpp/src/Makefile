TOP_PATH=../..
THRIFT_PATH = /usr/local/include
LIB_PATH = /usr/local/thrift/lib
TARGET_PATH = $(TOP_PATH)/build
TARGET_INC_PATH = $(TARGET_PATH)/entry_thrift
TARGET_LIB_PATH = $(TARGET_PATH)/lib
GEN_PATH = ./gen-cpp
VPATH = $(GEN_PATH)

INC_PATH = -I${THRIFT_PATH} -I$(GEN_PATH)
ADD_SRC = $(GEN_PATH)/*.cpp

CPP = g++

CPPFLAGS= -std=c++11 $(INC_PATH) -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H
LDFLAGS += -static

export CFLAGS CPPFLAGS LDFLAGS

SOURCES=$(shell echo *.cpp $(ADD_SRC))
HEADERS=$(shell echo *.h)
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=libsimentry_thrift.a
target:
	rm -rf gen-cpp
	thrift --gen cpp $(TOP_PATH)/*.thrift
	make all
all: $(TARGET)
$(TARGET) : $(OBJECTS)
	$(AR) -rc $(TARGET) $(OBJECTS)
	mkdir -p $(TARGET_LIB_PATH)
	cp $(TARGET) $(TARGET_LIB_PATH)
	mkdir -p $(TARGET_INC_PATH)
	cp *.h $(TARGET_INC_PATH)
	cp $(GEN_PATH)/*.h $(TARGET_INC_PATH)

clean:
	rm -rf *.o $(TARGET) $(PROTO_PATH)/*.o gen-cpp


#default: server
#server: server.cpp
#	g++ -g -o server -I${THRIFT_PATH} -I${BOOST_PATH}  -I$(GEN_SRC) -L${LIB_PATH} -lthrift server.cpp ${GEN_SRC}
