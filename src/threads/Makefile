TARGET = threads

CXX = g++
#CXX = icc

$(TARGET): $(TARGET).cxx
	$(CXX) -O0 -g $< -lm -lpthread -o $@

clean:
	rm -f threads
