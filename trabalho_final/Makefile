CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0 libxml-2.0 jansson json-c`
LIBS=`pkg-config --libs gtk+-3.0 libxml-2.0 jansson json-c`
TARGET=main
SRC=main.c

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

test: tests/validate_json.c
	$(CC) tests/validate_json.c -o test $(CFLAGS) $(LIBS)

dashboard: dashboard.c
	$(CC) dashboard.c -o dashboard $(CFLAGS) $(LIBS)

debug: debug.c 
	$(CC)  debug.c -o debug $(CFLAGS) $(LIBS) -g

clean:
	rm -f $(TARGET).exe test.exe  dashboard.exe
