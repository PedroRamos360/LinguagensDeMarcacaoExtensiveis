CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0` -I/mingw64/include/libxml2
LIBS=`pkg-config --libs gtk+-3.0` -lxml2
TARGET=main
SRC=main.c

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET).exe
