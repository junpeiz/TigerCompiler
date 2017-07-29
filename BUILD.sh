cd core
rm -rf build
rm y.tab.c y.tab.h y.output lex.yy.c

mkdir build
yacc -dv tiger.y
flex tiger.lex

cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
cp TigerCompiler ../../
cd ../../
