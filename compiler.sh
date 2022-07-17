cmake -B build &&
cd build &&
make &&
./tests/test4 in.txt out.txt 1 1 0 && cd ../ && dot -Tsvg graph.dot
