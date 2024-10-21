ALLCPP=$(wildcard *.cpp)
SOURCES=$(filter-out authlib.cpp,$(ALLCPP))
TARGETS=$(patsubst %.cpp,%,$(SOURCES))
CPPFLAGS=-Wall -pedantic -Wextra -std=c++11 -Wno-deprecated-declarations
OPENSSL_PREFIX=$(shell brew --prefix openssl@3)
# OPENSSL_FLAGS=-I$(OPENSSL_PREFIX)/include -L$(OPENSSL_PREFIX)/lib
OPENSSL_FLAGS=-I$(OPENSSL_PREFIX)/include -L$(OPENSSL_PREFIX)/lib -lssl -lcrypto
CURL_FLAGS=-I$(CURL_PREFIX)/include -L$(CURL_PREFIX)/lib -lcurl
# JSON_PREFIX=$(shell brew --prefix nlohmann-json)
# JSON_PREFIX=$(shell brew --prefix nlohmann-json)
JSON_FLAGS=-I$(JSON_PREFIX)/include
LDFLAGS=-lssl -lcrypto -lcurl 
LDFLAGS=-L$(OPENSSL_PREFIX)/lib -lssl -lcrypto -L$(CURL_PREFIX)/lib -lcurl
C=g++

all: $(TARGETS)

authlib.o: authlib.cpp
	$(C) $(CPPFLAGS) $(OPENSSL_FLAGS) -c authlib.cpp

%: %.cpp authlib.o
	$(C) $(CPPFLAGS) $(OPENSSL_FLAGS) $(CURL_FLAGS) $(JSON_FLAGS) $< authlib.o $(LDFLAGS) -o $@

clean:
	rm -f $(TARGETS)
	rm -f *~ *.o a.out
