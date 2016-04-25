TARGET = loopcheck

cc = gcc
#cc = icc

$(TARGET): $(TARGET).c
	$(cc) -O0 -g $< -lm -o $@

clean:
	rm -f loopcheck
