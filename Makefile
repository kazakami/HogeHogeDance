OBJS = Quaternion.o Vector.o Matrix4x4d.o imageLoader.o pmxLoader.o vmdLoader.o mesh.o Texture2D.o utility.o
WARNINGS = -Wall -Wextra -W -Wpointer-arith -Wcast-qual -Wcast-align -Woverlength-strings
CXXFLAGS = $(WARNINGS) -std=c++11
LDLIBS = -lGL -lGLU -lglut -lpng


.SUFFIXES: .cpp .o

hhd: hhd.o $(OBJS)
	$(CXX) -o $@ $< $(OBJS) $(LDLIBS)

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS)

clean:
	$(RM) *.o hhd
