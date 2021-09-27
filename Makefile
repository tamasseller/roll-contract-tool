OUTPUT = roll-contract-tool
DEPL_BIN += $(OUTPUT)

OBJDIR = .o

SOURCES += main.cpp
SOURCES += Dump.cpp
SOURCES += Serialize.cpp
SOURCES += CodeGen.cpp

SOURCES += ast/ContractParser.cpp
SOURCES += ast/ContractFormatter.cpp
SOURCES += ast/ContractTextCodec.cpp

SOURCES += gen/Generator.cpp
SOURCES += gen/cpp/Cpp.cpp
SOURCES += gen/cpp/CppCommon.cpp
SOURCES += gen/cpp/CppSymGen.cpp
SOURCES += gen/cpp/CppParamTypeGen.cpp
SOURCES += gen/cpp/CppTypeAliasGen.cpp
SOURCES += gen/cpp/CppStructSerdes.cpp
SOURCES += gen/cpp/CppProxyCommon.cpp
SOURCES += gen/cpp/CppClientProxy.cpp
SOURCES += gen/cpp/CppServerProxy.cpp
SOURCES += gen/cpp/CppSessionProxy.cpp

GENDIR = .gen
CLEAN_EXTRA += $(GENDIR)
SOURCES += $(GENDIR)/rpcLexer.cpp $(GENDIR)/rpcParser.cpp

$(abspath ast/ContractParser.cpp): $(abspath $(GENDIR)/rpcParser.h)

$(abspath $(GENDIR)/%Lexer.cpp $(GENDIR)/%Parser.cpp \
$(GENDIR)/%Lexer.h $(GENDIR)/%Parser.h): %.g4 $(MAKEFILE_LIST)
	@printf "Antlr \e[1;32m$< -> $@\e[0m\n"
	@antlr4 -o $(GENDIR) $< -no-listener -no-visitor -Dlanguage=Cpp
	
LIBS += antlr4-runtime

INCLUDE_DIRS += .
INCLUDE_DIRS += ..
INCLUDE_DIRS += $(GENDIR)
INCLUDE_DIRS += /usr/include/antlr4-runtime

CXXFLAGS += -O0 -g3
CXXFLAGS += --std=c++17
CXXFLAGS += -fno-inline
CXXFLAGS += -fmax-errors=5
CXXFLAGS += -Wall -Werror -Wno-unused-value -Wreturn-local-addr -Wno-attributes 

CXXFLAGS += -rdynamic
CXXFLAGS += --coverage
LDFLAGS += -rdynamic
LIBS += gcov 

LD=$(CXX)

COVROOT = ..

include cli-base/mod.mk
include ultimate-makefile/Makefile.ultimate
