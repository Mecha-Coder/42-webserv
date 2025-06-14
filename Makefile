# Makefile for the web server project
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
SRCDIR = src
OBJDIR = obj

# Source files
TOML_SRCS = $(SRCDIR)/toml/Lexer.cpp $(SRCDIR)/toml/ParseError.cpp $(SRCDIR)/toml/Parser.cpp \
            $(SRCDIR)/toml/Syntax.cpp $(SRCDIR)/toml/Table.cpp $(SRCDIR)/toml/Token.cpp $(SRCDIR)/toml/Toml.cpp
CONFIG_SRCS = $(SRCDIR)/config/Config.cpp
CGI_SRCS = $(SRCDIR)/cgi/CGIHandler.cpp $(SRCDIR)/cgi/ScopedEnvArray.cpp
UTILS_SRCS = $(SRCDIR)/utils/Utils.cpp

# Object files
TOML_OBJS = $(TOML_SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
CONFIG_OBJS = $(CONFIG_SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
CGI_OBJS = $(CGI_SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
UTILS_OBJS = $(UTILS_SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

ALL_OBJS = $(TOML_OBJS) $(CONFIG_OBJS) $(CGI_OBJS) $(UTILS_OBJS)

# Executables
CONFIG_TARGET = config_test
CGI_TARGET = cgi_test

.PHONY: all clean config cgi

all: $(CONFIG_TARGET) $(CGI_TARGET)

# Create object directories
$(OBJDIR):
	mkdir -p $(OBJDIR)/toml $(OBJDIR)/config $(OBJDIR)/cgi $(OBJDIR)/utils

# Object file compilation
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Config test executable
$(CONFIG_TARGET): $(TOML_OBJS) $(CONFIG_OBJS) $(UTILS_OBJS) $(OBJDIR)/config/main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# CGI test executable  
$(CGI_TARGET): $(CGI_OBJS) $(UTILS_OBJS) $(OBJDIR)/cgi/main_cgi.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# Individual targets
config: $(CONFIG_TARGET)
cgi: $(CGI_TARGET)

clean:
	rm -rf $(OBJDIR) $(CONFIG_TARGET) $(CGI_TARGET)

# Test targets
test-config: $(CONFIG_TARGET)
	./$(CONFIG_TARGET) config.toml

test-cgi: $(CGI_TARGET)
	./$(CGI_TARGET)