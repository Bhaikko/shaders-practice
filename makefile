CXX = g++
RM = rm -f
CPPFLAGS = -std=c++11 -Wall -g

Libs=-lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lGLEW

SRCS_CPP= \
	./Helper/commonValues.cpp \
	./Helper/Camera/Camera.cpp \
	./Helper/Shader/Shader.cpp \
	./Helper/Window/Window.cpp \
	./Helper/Texture/Texture.cpp \
	./Helper/CoordinateAxes/CoordinateAxes.cpp \
	./Helper/Skybox/Skybox.cpp \
	./Helper/Model/Model.cpp \
	./Helper/Frustum/Frustum.cpp \
	./Src/Chapter08/ShadowMap/ShadowMap.cpp \
	main.cpp \
	
run: 
	$(CXX) $(SRCS_CPP) -o application $(Libs) $(GlfwLibs) $(CPPFLAGS)
