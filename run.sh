g++ -c main.cpp player.cpp dot.cpp wall.cpp collider.cpp ghost.cpp
g++ main.o player.o dot.o wall.o collider.o ghost.o -o PACMAN -lsfml-graphics -lsfml-window -lsfml-system
./PACMAN
