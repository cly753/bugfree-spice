path_BFLoG=/home/cly/git-workspace/bugfree-spice/try-area/the_BFLoG/BFLoG
path_fftw3=/home/cly/git-workspace/bugfree-spice/try-area/the_BFLoG/fftw-3.3.4

g++ -m64 -shared -fpic -o libbflog_api.so BFLoG_API.cpp -I$path_BFLoG -I$JAVA_HOME/include/ -I$JAVA_HOME/include/linux -I$path_fftw3/api/ -lBFLoG -lfftw3f -lm -L$path_BFLoG