CATCH=$(HOME)/include

CXXFLAGS=-std=c++11 -pedantic -Wall -Wextra -Werror -I$(CATCH)

tests: ptr.hpp
