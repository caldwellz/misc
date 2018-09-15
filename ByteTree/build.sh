gcc -O2 -c -o bytetree.o bytetree.c
g++ -O2 -std=c++11 -s -o test.exe Test.cxx PropertyTree_bytetree.cxx bytetree.o
gcc -O0 -g -c -o bytetree_d.o bytetree.c
g++ -O0 -std=c++11 -g -o test_d.exe Test.cxx PropertyTree_bytetree.cxx bytetree_d.o
echo PropertyTree_map.cxx
