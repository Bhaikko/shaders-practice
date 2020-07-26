CXX = g++
RM = rm -f
CPPFLAGS = -std=c++11 -Wall -g

GlewLibs=-lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl
GlfwLibs=-lGLEW 

SRCS_CPP= \
	./Helper/commonValues.cpp \
	./Helper/Camera/Camera.cpp \
	./Helper/Shader/Shader.cpp \
	./Helper/Window/Window.cpp \
	./Helper/Texture/Texture.cpp \
	./Helper/CoordinateAxes/CoordinateAxes.cpp \
	./Helper/Model/Model.cpp \
	./Src/Chapter02/UniformBlock/UniformBlock.cpp \
	./Src/Chapter03/SceneDiscard/SceneDiscard.cpp \
	./Src/Chapter03/sceneSubRoutine/sceneSubRoutine.cpp \
	./Src/Chapter03/Fog/Fog.cpp \
	./Src/Chapter04/PBR/PBR.cpp \
	./Src/Chapter04/ToonShading/ToonShading.cpp \
	./Src/Chapter05/AlphaTest/AlphaTest.cpp \
	./Src/Chapter05/NormalMap/NormalMap.cpp \
	main.cpp \
	
run: 
	$(CXX) $(SRCS_CPP) -o application $(GlewLibs) $(GlfwLibs) $(CPPFLAGS)
