TARGET = lulesh

#CXX = mpicc
CXX = mpicxx

$(TARGET): $(TARGET).cc
	$(CXX) -O3 -g $< -lm -DLULESH_SHOW_PROGRESS=1 -o $@

clean:
	rm -f *~ $(TARGET)
