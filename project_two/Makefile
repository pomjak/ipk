CXX=g++
RM=rm -f
CPPFLAGS= -O2 -std=c++20 -Wall -Wextra -Wpedantic 
DEBUG= -g
SRCS=ipkcpd.cc ipkcpd_fun.cc udp_lexer.cc udp_parser.cc
OBJS=ipkcpd

make:$(SRCS)
	$(CXX) $(CPPFLAGS) -o $(OBJS) $(SRCS)

debug:$(SRCS)
	$(CXX) $(CPPFLAGS) $(DEBUG) -o $(OBJS) $(SRCS)

clean:
	$(RM) $(OBJS)

changelog:
	git log --pretty="- %s" | head -n -37 > CHANGELOG.md