SOURCES=dispatcher.c calc.c stack.c 
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=dispatcher

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) $^ -o $@ 

.PHONY: clean

clean: 
	rm -vf $(TARGET) 

.o:
	$(CC) -c $< -o $@
