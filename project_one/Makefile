CXX=g++
RM=rm -f
CPPFLAGS= -O2 -std=c++14 -Wall -Wextra -Wpedantic 
DEBUG= -g
SRCS=ipkcpc.cc
OBJS=ipkcpc

make:$(SRCS)
	$(CXX) $(CPPFLAGS) -o $(OBJS) $(SRCS)

debug:$(SRCS)
	$(CXX) $(CPPFLAGS) $(DEBUG) -o $(OBJS) $(SRCS)

clean:
	$(RM) $(OBJS)