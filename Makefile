CC=gcc
CFLAGS=-Wall -Wextra -g -I include
OFLAGS=-march=native -mtune=native -O3

SRC=src
DEPS=target/deps
TARGET=target/branch_and_bound

.PHONY: build clean

build: $(TARGET)

run: $(TARGET)
	$(TARGET)

$(TARGET): $(DEPS)/main.o $(DEPS)/config.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@

$(DEPS)/%.o: $(SRC)/%.c
	@mkdir -p $(DEPS)
	$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@

clean:
	@rm -rf target
