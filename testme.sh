#!/bin/bash

echo "================================================================="
echo "== testing theron 3.05.01"
echo "================================================================="

cd Theron
git reset --hard 76f8e86242292da4e989e81d8aa6cdfa82476e7e
cd ..
rm -rf build
mkdir build
cd build
cmake ../
make -j2
tests/topology_test
cd ..
echo "== done test theron 3.05.01, sleeping while you read result... =="
sleep 10

echo "================================================================="
echo "== testing theron 4.02.01"
echo "================================================================="
cd Theron
git pull origin master
cd ..
rm -rf build
mkdir build
cd build
cmake ../
make -j2
tests/topology_test
cd ..
echo "== done test theron 4.02.01, sleeping while you read result... =="
sleep 10

echo "================================================================="
echo "== cleanup..."
echo "================================================================="
rm -rf build
cd Theron
git reset --hard 76f8e86242292da4e989e81d8aa6cdfa82476e7e
cd ..
