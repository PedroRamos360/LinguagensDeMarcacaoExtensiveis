CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0 libxml-2.0 jansson`
LIBS=`pkg-config --libs gtk+-3.0 libxml-2.0 jansson`
TARGET=core
SRC=core.c

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET).exe
