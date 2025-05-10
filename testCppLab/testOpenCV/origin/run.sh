# rm majorColorApp
# g++ -o majorColorApp collectionMajorColor.cpp `pkg-config --cflags --libs opencv4`
# ./majorColorApp

rm makeMaskApp
g++ -o makeMaskApp createMask.cpp `pkg-config --cflags --libs opencv4`
./makeMaskApp

# rm main_allAPP
# g++ -o main_allAPP main_all.cpp `pkg-config --cflags --libs opencv4`
# ./main_allAPP

# rm my_program
# g++ -o my_program splitOutOfEnv1.cpp `pkg-config --cflags --libs opencv4`
# ./my_program