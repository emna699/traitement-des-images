# traitement-des-images pour le thread: 
g++ -o exe1 singleThread.cpp `pkg-config --cflags --libs opencv4`
./exe1
# traitement-des-images pour le multihread:
g++ -o exe2 multiThread.cpp `pkg-config --cflags --libs opencv4` -lpthread
./exe2
