CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0 libxml-2.0 jansson json-c`
LIBS=`pkg-config --libs gtk+-3.0 libxml-2.0 jansson json-c`
TARGET=main
SRC=main.c
VALIDATE_SRC=validate_json.c jsonschema-c/instance_validator.c jsonschema-c/schema_validator.c jsonschema-c/utils.c

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

test:
	$(CC) test.c -o test $(CFLAGS) $(LIBS)

validate:
	$(CC) $(VALIDATE_SRC) -o validate $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET).exe test.exe validate.exe
