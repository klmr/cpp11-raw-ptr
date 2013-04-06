CATCH=deps/catch

CXXFLAGS=-std=c++11 -pedantic -Wall -Wextra -Werror -isystem$(CATCH)

tests: ptr.hpp
