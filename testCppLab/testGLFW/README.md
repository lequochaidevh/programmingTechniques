sudo apt update
sudo apt install libglfw3-dev
#build and run 
g++ main.cpp -o my_app -lglfw -lGL
./my_app
