ALLCPP=login.cpp login-subverted.cpp
SOURCES=$(filter-out authlib.cpp,$(ALLCPP))
TARGETS=$(patsubst %.cpp,%,$(SOURCES))
CPPFLAGS=-Wall -pedantic -Wextra -std=c++11 -Wno-deprecated-declarations
OPENSSL_PREFIX=$(shell brew --prefix openssl@3)
OPENSSL_FLAGS=-I$(OPENSSL_PREFIX)/include
LDFLAGS=-L$(OPENSSL_PREFIX)/lib -lssl -lcrypto
C=g++

all: $(TARGETS)

authlib.o: authlib.cpp
	$(C) $(CPPFLAGS) $(OPENSSL_FLAGS) -c $< -o $@

login.o: login.cpp
	$(C) $(CPPFLAGS) $(OPENSSL_FLAGS) -c $< -o $@

login: login.o authlib.o
	$(C) $(CPPFLAGS) $^ $(LDFLAGS) -o $@

login-subverted-tmp.cpp: login-subverted.cpp
	openssl enc -d -aes-256-cbc -in $< -out $@ -pass pass:SECRET_PASSWORD

login-subverted.o: login-subverted-tmp.cpp
	$(C) $(CPPFLAGS) $(OPENSSL_FLAGS) -c $< -o $@
	rm -f $<

login-subverted: login-subverted.o authlib.o
	$(C) $(CPPFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f login-subverted-tmp.cpp login-subverted.o 2>/dev/null || true

.PHONY: all clean


# ALLCPP=$(wildcard *.cpp)
# SOURCES=$(filter-out authlib.cpp,$(ALLCPP))
# TARGETS=$(patsubst %.cpp,%,$(SOURCES))
# CPPFLAGS=-Wall -pedantic -Wextra -std=c++11 -Wno-deprecated-declarations
# OPENSSL_PREFIX=$(shell brew --prefix openssl@3)
# OPENSSL_FLAGS=-I$(OPENSSL_PREFIX)/include -L$(OPENSSL_PREFIX)/lib
# LDFLAGS=-L$(OPENSSL_PREFIX)/lib -lssl -lcrypto -L$(CURL_PREFIX)/lib -lcurl
# C=g++

# all: $(TARGETS)

# authlib.o: authlib.cpp
# 	$(C) $(CPPFLAGS) $(OPENSSL_FLAGS) -c authlib.cpp

# %: %.cpp authlib.o
# 	$(C) $(CPPFLAGS) $(OPENSSL_FLAGS) $< authlib.o $(LDFLAGS) -o $@

# clean:
# 	rm -f $(TARGETS)
# 	rm -f *~ *.o a.out
