OBJS = Quaternion.o Vector.o Matrix4x4d.o imageLoader.o pmxLoader.o vmdLoader.o mesh.o Texture2D.o utility.o Shader.o
WARNINGS = -Wall -Wextra -W -Wpointer-arith -Wcast-qual -Wcast-align -Woverlength-strings


UNAME = ${shell uname}
ifeq ($(UNAME),Linux)
# for Linux
CXXFLAGS = $(WARNINGS) -std=c++11 -O3
LDLIBS = -lGL -lGLU -lglut -lGLEW -lpng
endif

ifeq ($(UNAME),Darwin)
# for MacOSX
CXXFLAGS = $(WARNINGS) -I/usr/X11R6/include -std=c++0x
LDLIBS = -framework GLUT -framework OpenGL -L/usr/X11/lib -lGLEW -lpng -liconv
endif





.SUFFIXES: .cpp .o

hhd: hhd.o $(OBJS)
	$(CXX) -o $@ $< $(OBJS) $(LDLIBS)

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS)

clean:
	$(RM) *.o hhd
