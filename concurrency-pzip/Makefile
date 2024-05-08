CC = gcc
CFLAGS =   -g  -Wall -pthread -O -fsanitize=leak
file ?= hello.txt
TARGET = blocked_concurrent
TARGET1 = pc_concurrent
TEST_TARGET = test_program
PERF = perf
admin = sudo
SRC_MAIN = src/main.c
SRC_MAIN1 = src/pc_main.c
SRC_TEST ?= tests/UnitTests.c
SRCS_HELPERS =  src/parser.c src/threadHelpers.c src/global_vars.c src/Queue.c src/producerConsumer.c

all: $(TARGET) $(TARGET1) 

$(TARGET): $(SRC_MAIN) $(SRCS_HELPERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_MAIN) $(SRCS_HELPERS)

$(TARGET1): $(SRC_MAIN) $(SRCS_HELPERS)
	$(CC) $(CFLAGS) -o $(TARGET1) $(SRC_MAIN1) $(SRCS_HELPERS)

profile: $(TARGET)
	$(admin) $(PERF) record ./$(TARGET) $(file)

analyze:
	$(admin) $(PERF) report

$(TEST_TARGET): $(SRC_TEST) $(SRCS_HELPERS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(SRC_TEST) $(SRCS_HELPERS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(TARGET1) 
