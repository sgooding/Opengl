INCLUDE = -I/usr/include/
LIBDIR  = -L/usr/X11R6/lib
COMPILERFLAGS = -Wall
CXX = g++
CXXFLAGS = $(COMPILERFLAGS) $(INCLUDE)
LIBS = -lglui -lopengl32 -lglu32 -lglut32 
TARGET=hello_world


$(TARGET): $(TARGET).o
	$(CXX) $(OPT) $^ -o $@ $(LIBDIR) $(LIBS) 

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXDIR) -c $< -o $@

clean: 
	rm *.o $(TARGET) 



