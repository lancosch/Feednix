bin_PROGRAMS = feednix 
feednix_SOURCES = main.cpp FeedlyProvider.cpp CursesProvider.cpp 
AM_CFLAGS = -lcurl -ljsoncpp -lmenu -lpanel -lncursesw
AM_LIBS = curl jsoncpp menu panel ncurses 
feednix_CPPFLAGS =-DDEBUG -std=c++11 -Wall -I /usr/include/jsoncpp
