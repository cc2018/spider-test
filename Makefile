TARGET=mongo-test
CC=g++
CFLAGS= -g -Wall -pthread -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0 -I/usr/include/libxml2 -I/usr/local/include -fpic
LIBFLAG=-lbson-1.0 -lmongoc-1.0 -lcurl -lpcre -lxml2 -luri -liconv -lfcgi
C_SOURCES=$(shell find . -name "*.cpp")
C_OBJECTS=$(patsubst %.cpp, %.o, $(C_SOURCES))
HEADERS=$(shell find . -name "*.h")

$(warning  $(C_SOURCES))
$(warning  $(C_OBJECTS))

$(TARGET): $(C_OBJECTS)
	$(CC) -o $(TARGET) $(C_OBJECTS) $(LIBFLAG)
	
#%.o: %.cpp
%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
clean:
	rm -f $(shell find . -name "*.o") $(TEMP) $(TARGET)
