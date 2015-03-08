g++ -fPIC -c -o a.o NotAlgoCpp.cpp -I$JAVA_HOME/include/ -I$JAVA_HOME/include/linux
g++ -fPIC -c -o b.o SomeImpl.cpp

ar crv a.a a.o # create, replace, verbose
ar crv b.a b.o

rm {a,b}.o # Brace Expansion

g++ -m64 -shared -fPIC -o libNotAlgoCpp.so -Wl,--whole-archive a.a b.a -Wl,--no-whole-archive # force read from archive, don't need *.o

rm {a,b}.a
