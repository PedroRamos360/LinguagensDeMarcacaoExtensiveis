CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0 libxml-2.0 jansson json-c`
LIBS=`pkg-config --libs gtk+-3.0 libxml-2.0 jansson json-c`
TARGET=main
SRC=main.c dashboard.c xml_to_json.c validate_json.c

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

test: tests/test_validate.c
	$(CC) tests/test_validate.c validate_json.c -o test $(CFLAGS) $(LIBS)

dashboard: dashboard.c
	$(CC) dashboard.c -o dashboard $(CFLAGS) $(LIBS)

debug: debug.c 
	$(CC)  debug.c -o debug $(CFLAGS) $(LIBS) -g

clean:
	rm -f $(TARGET).exe test.exe  dashboard.exe debug.exe
