#Default Flags
FLAGS = -Wall -Wextra -Wconversion -pedantic -Wvla -std=c++11

# make release - will compile "all" with $(FLAGS) and the O3 flag
release: FLAGS += -O3
release: all

# make debug - will compile "all" with $(FLAGS) and the g flag
debug: FLAGS += -g
debug: all

profile: FLAGS += -pg
profile: all

all : server.cpp
	g++ $(FLAGS) -lboost_iostreams -lboost_system -o out server.cpp 

# make clean - remove .o files.  You can modify it to remove whatever
clean:
	rm -f *.o out
