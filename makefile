all: scene.o main.o engine.o model3d.o shader.o model2d.o shaderProgramm.o
	g++ scene.o shader.o shaderProgramm.o model2d.o model3d.o engine.o main.o -std=c++11 -lGL -lGLEW -lglfw -lpng -o out
	rm *.o

main.o:
	g++ -c main.cpp

engine.o: ve/source/engine.cpp
	g++ -c -g ve/source/engine.cpp

model3d.o: ve/source/model3d.cpp
	g++ -c ve/source/model3d.cpp

model2d.o: ve/source/model2d.cpp
	g++ -c -g ve/source/model2d.cpp

shader.o: ve/source/shader.cpp
	g++ -c -g ve/source/shader.cpp

shaderProgramm.o: ve/source/shaderProgramm.cpp
	g++ -c -g ve/source/shaderProgramm.cpp

scene.o: ve/source/scene.cpp
	g++ -c -g ve/source/scene.cpp

clear:
	rm *.o
