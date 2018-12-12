BIN=./bin

all: train

.PHONY: train
train: $(BIN)
	@gcc -o $(BIN)/train -Iutil main.c 1lnn.c util/screen.c util/mnist-utils.c util/mnist-stats.c
	@echo Compiled...
	@$(BIN)/train

.PHONY: test
test: $(BIN)
	@gcc -o $(BIN)/test -Iutil test.c 1lnn.c util/screen.c util/mnist-utils.c util/mnist-stats.c
	@echo Compiled...
	@$(BIN)/test

.PHONY: convert
convert: $(BIN) mnist-convert.c
	@gcc -o $(BIN)/mnist-convert mnist-convert.c -Iutil 1lnn.c util/mnist-utils.c
	@echo Compiled...
	@$(BIN)/mnist-convert
	
$(bin):
	@mkdir $(BIN)
