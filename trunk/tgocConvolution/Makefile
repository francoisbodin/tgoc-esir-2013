OCVI= -I/usr/local/Cellar/opencv/2.4.7.1/include/opencv\
      -I/usr/local/Cellar/opencv/2.4.7.1/include/opencv2

all: convolution.exe

convolution.exe: graffiti.cpp apirequester.cpp convolution.cpp
	g++ $(OCVI) -O2 -lcurl  graffiti.cpp apirequester.cpp convolution.cpp -o convolution.exe -lopencv_core -lopencv_highgui -lopencv_imgproc

run: convolution.exe
	./convolution.exe

clean:
	rm -f *.o convolution.exe *~


