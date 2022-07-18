cmake -B build &&
cd build &&
make &&
./tests/test8 in.txt out.txt 0 1 0 && cd ../ && dot -Tsvg graph.dot
