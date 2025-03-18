all: assign4
CPP=g++
CFLAGS=-std=c++11

assign4: main.o removecomments.o tokenization.o concretesyntaxtree.o symboltable.o
	$(CPP) -ggdb -o assign4 main.o removecomments.o tokenization.o concretesyntaxtree.o symboltable.o

main.o: main.cpp removecomments.hpp tokenization.hpp concretesyntaxtree.hpp symboltable.hpp
	$(CPP) -c main.cpp $(CFLAGS)

symboltable.o: symboltable.cpp symboltable.hpp concretesyntaxtree.hpp
	$(CPP) -c symboltable.cpp $(CFLAGS)

concretesyntaxtree.o: concretesyntaxtree.cpp concretesyntaxtree.hpp tokenization.hpp
	$(CPP) -c concretesyntaxtree.cpp $(CFLAGS)

tokenization.o: tokenization.cpp tokenization.hpp
	$(CPP) -c tokenization.cpp $(CFLAGS)

removecomments.o: removecomments.cpp removecomments.hpp
	$(CPP) -c removecomments.cpp $(CFLAGS)

clean:
	rm -f *.o *~