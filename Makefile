
HEADERS = io.h tormentor.h
SOURCES = main.cpp io.cpp tormentor.cpp

main: $(HEADERS) $(SOURCES)
	g++ -g $(SOURCES) -o build/main
	

