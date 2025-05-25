### build project

```bash
cd <your_path_to_project>/prj/support/tools/ 
source setup_env.sh
# build clean and run
cd $PROJECT_DIR/client/ && [ -d "build" ] && rm -rf build/ && mkdir build && cd build/ && cmake .. && cmake --build . -- -j4 && ./minecraftC

# build update and run
cd $PROJECT_DIR/client/ && cd build/ && cmake .. && cmake --build . -- -j4 && ./minecraftC

```