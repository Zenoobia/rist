#c++ -o main main3.cpp two_things.hpp -I/usr/local/include -I/usr/include/lua5.3 -I/usr/local/include/nlohmann -I../include/metas -L/usr/local/lib -llua

trap times EXIT

#c++ -std=c++17 /usr/local/include/sol.hpp -I/usr/include/lua5.3
#c++ -std=c++17 Test.h -I/usr/local/include -I/usr/include/lua5.3

c++ -std=c++17 -o main main4.cpp -I/usr/local/include -I/usr/include/lua5.3 -I/usr/local/include/nlohmann -I../include/metas -L/usr/local/lib -llua

#c++ -std=c++17 -o main main3.cpp  -I/usr/local/include -I/usr/include/lua5.3 -I/usr/local/include/nlohmann -I../include/metas -L/usr/local/lib -llua

#./main 
