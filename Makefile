ifeq ($(shell uname -m),x86_64)

ROOT=.
DEPS_DIR=$(ROOT)/deps
BASE_DIR=$(DEPS_DIR)/base

CXX=g++

CCHECK=../tools/cpplint.py

# export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH

# Modules
MODULE=smalltable

# 自动获取Git版本信息,支持-v查看如./rs -v
# RELEASE_VERSION := $(shell sh -c './version/gen_version.sh')

# LDFLAGS=-static-libgcc -static-libstdc++
# LDFLAGS=

CXXFLAGS=-ggdb \
		 -pipe \
		 -W \
		 -Wall \
		 -fPIC \
		 -Wextra \
		 -m64 \
		 -DNDEBUG \
		 -fno-strict-aliasing \
		 -Wno-invalid-offsetof \
		 -Wno-deprecated \
		 -Wno-deprecated-declarations \
		 -Wno-unused-parameter \
		 -Wno-sign-compare \
		 -Wno-write-strings \
		 -Wno-unused-local-typedefs \
		 -Wno-literal-suffix \
		 -Wno-narrowing \
		 -Wno-parentheses \
		 -Wno-unused-but-set-variable \
		 -Wno-unused-variable \
		 -Wno-char-subscripts \
		 -D__STDC_LIMIT_MACROS \
		 -O2

INC_DIR=-I. -I./detail

LIB_INC=

######################TCMALLOC######################
# TCMALLOC = tcmalloc
ifdef TCMALLOC
LDFLAGS += -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
CXXFLAGS += -DUSE_TCMALLOC
INC_DIR += -I$(DEPS_DIR)/include/tcmalloc/include
LIB_INC += $(DEPS_DIR)/lib/tcmalloc/lib/libtcmalloc_and_profiler.a
endif

all: lib$(MODULE).a test_smalltable
	@echo "[[1;32;40mBUILD[0m][Target:'[1;32;40mall[0m']"
	@mkdir -p output/include
	@mkdir -p output/lib
	@cp -rf lib$(MODULE).a output/lib
	@cp -rf *.hpp output/include
	@cp -rf *.h output/include
	@echo "make all done"

system-check:
	@echo "CHECK DEPENDENCY"

# 语法规范检查
style:
	python ../tools/cpplint.py --extensions=hpp,cpp --linelength=80 *.cpp

clean:
	@rm -rf lib$(MODULE).a
	@rm -rf output
	@rm -rf test
	@rm -rf test_smalltable
	@find . -name "*.o" | xargs -I {} rm {}
.phony:clean

SERVER_OBJS += $(patsubst %.cpp,%.o, $(shell find . -name "*.cpp" | egrep -v "main.cpp|test.cpp"))
SERVER_OBJS += $(patsubst %.cc,%.o, $(shell find . -name "*.cc" | egrep -v "main.cpp|test.cpp"))
SERVER_OBJS += $(patsubst %.c,%.o, $(shell find . -name "*.c" | egrep -v "main.cpp|test.cpp"))
# SERVER_INCS += $(shell find . -name "*.hpp")
# SERVER_INCS += $(shell find . -name "*.h")

%.o:%.cpp
	@echo "[[1;32;40mBUILD[0m][Target:'[1;32;40m$<[0m']"
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c $< -o $@

%.o:%.cc
	@echo "[[1;32;40mBUILD[0m][Target:'[1;32;40m$<[0m']"
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c $< -o $@

%.o:%.c
	@echo "[[1;32;40mBUILD[0m][Target:'[1;32;40m$<[0m']"
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c $< -o $@

lib$(MODULE).a:$(SERVER_OBJS) 
	# ar crs lib$(MODULE).a $(SERVER_OBJS) $(SERVER_INCS) $(LIB_INC)
	ar crs lib$(MODULE).a $(SERVER_OBJS) $(LIB_INC)
	mkdir -p ./output/lib
	cp -f libsmalltable.a ./output/lib
	mkdir -p ./output/include
	cp -f *.h ./output/include
	cp -f *.hpp ./output/include
	cp -rf detail ./output/include

test_smalltable:$(SERVER_OBJS) 
	@$(CXX) $(INC_DIR) $(CXXFLAGS) $^ main.cpp -o $@

endif #ifeq ($(shell uname -m),x86_64)
