cmake -B build &&
cd build &&
make &&
./tests/test5 in.txt out.txt 1 1 0 && cd ../ && dot -Tsvg out.dot