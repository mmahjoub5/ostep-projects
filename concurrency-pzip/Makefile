CC = gcc
CFLAGS = -g
file ?= hello.txt
TARGET = my_program
TEST_TARGET = test_program
PERF = perf
admin = sudo
SRC_MAIN = src/main.c
SRC_TEST ?= tests/threadHelperUnitTests.c
SRCS_HELPERS =  src/parser.c src/threadHelpers.c

all: $(TARGET)

$(TARGET): $(SRC_MAIN) $(SRCS_HELPERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_MAIN) $(SRCS_HELPERS)

profile: $(TARGET)
	$(admin) $(PERF) record ./$(TARGET) $(file)

analyze:
	$(admin) $(PERF) report

$(TEST_TARGET): $(SRC_TEST) $(SRCS_HELPERS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(SRC_TEST) $(SRCS_HELPERS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)
