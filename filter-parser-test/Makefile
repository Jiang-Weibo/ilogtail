target: #./BashParser.o ./test.o -std=c++11
	g++ ./BashParser.cpp ./test.cpp -L ./ -ljsoncpp -o test -std=c++11


unit_test:
	g++ ./BashParser.cpp ./unit_test.cpp -L ./ -ljsoncpp -o unit_test -std=c++11
	./unit_test > tests-records.txt && rm unit_test