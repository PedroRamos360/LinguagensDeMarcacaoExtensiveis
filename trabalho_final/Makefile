CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0 libxml-2.0 jansson json-c`
LIBS=`pkg-config --libs gtk+-3.0 libxml-2.0 jansson json-c`
TARGET=main
SRC=main.c

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

validate: validate_json.c
	$(CC) validate_json.c -o validate $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET).exe test.exe validate.exe
