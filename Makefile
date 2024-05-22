# Makefile
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
LDLIBS = -lcurl
webpage_downloader.o: webpage_downloader.cpp webpage_downloader.h
	$(CXX) $(CXXFLAGS) -c webpage_downloader.cpp
link_extractor.o: link_extractor.cpp link_extractor.h
	$(CXX) $(CXXFLAGS) -c link_extractor.cpp
setlist.o: setlist.cpp setlist.h
	$(CXX) $(CXXFLAGS) -c setlist.cpp
stripedhashset.o: stripedhashset.cpp stripedhashset.h
	$(CXX) $(CXXFLAGS) -c stripedhashset.cpp
webpage_parser.o: webpage_parser.cpp webpage_parser.h
	$(CXX) $(CXXFLAGS) -c webpage_parser.cpp
crawler.o: crawler.cpp crawler.h webpage_downloader.h link_extractor.h setlist.h stripedhashset.h webpage_parser.h
	$(CXX) $(CXXFLAGS) -c crawler.cpp
tester: tester.cpp webpage_downloader.o link_extractor.o setlist.o stripedhashset.o crawler.o webpage_parser.o
	$(CXX) $(CXXFLAGS) -o tester tester.cpp webpage_downloader.o link_extractor.o setlist.o stripedhashset.o crawler.o webpage_parser.o $(LDLIBS)
main: main.cpp webpage_downloader.o link_extractor.o setlist.o stripedhashset.o crawler.o webpage_parser.o
	$(CXX) $(CXXFLAGS) -o main main.cpp webpage_downloader.o link_extractor.o setlist.o stripedhashset.o crawler.o webpage_parser.o $(LDLIBS)
clean:
	rm -f *.o tester main *.json *.txt
