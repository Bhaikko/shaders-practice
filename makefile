CXX = g++
RM = rm -f
CPPFLAGS =

GlewLibs=-lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl
GlfwLibs=-lGLEW 

SRCS_CPP= \
	./Helper/commonValues.cpp \
	./Helper/Camera/Camera.cpp \
	./Helper/Shader/Shader.cpp \
	./Helper/Window/Window.cpp \
	./Helper/Texture/Texture.cpp \
	./Helper/CoordinateAxes/CoordinateAxes.cpp \
	./Src/Chapter02/UniformBlock/UniformBlock.cpp \
	main.cpp \
	
run: 
	$(CXX) $(SRCS_CPP) -o application $(GlewLibs) $(GlfwLibs)
