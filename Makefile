CC=gcc
CFLAGS=-Wall -Wextra -g -I include -I ext/vec
OFLAGS=-march=native -mtune=native -O3

SRC=src
EXT=ext
DEPS=target/deps
TARGET=target/branch_and_bound

.PHONY: build clean

build: $(TARGET)

run: $(TARGET)
	$(TARGET) sample_config.txt

$(TARGET): $(DEPS)/main.o $(DEPS)/vec.o $(DEPS)/config.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@

$(DEPS)/%.o: $(SRC)/%.c
	@mkdir -p $(DEPS)
	$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@
	
$(DEPS)/%.o: $(EXT)/vec/%.c
	@mkdir -p $(DEPS)
	$(CC) $(CFLAGS) $(OFLAGS) -c $< -o $@

clean:
	@rm -rf target
