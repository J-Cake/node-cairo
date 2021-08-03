mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

NODE=-I/usr/include/node -I$(mkfile_dir)node_modules/node-addon-api/ -I$(shell node -p "require('node-addon-api').include")
INCLUDE=$(NODE)
LIBS=$(shell pkg-config --cflags --libs cairo pango pangocairo)

BUILD_DIR=./build
SRC_DIR=./lib

cairo.node: clean lib.o parser.o
	g++ $(BUILD_DIR)/lib.o $(BUILD_DIR)/parser.o -shared -fPIC -o $(BUILD_DIR)/cairo.node -Wall -Wextra $(LIBS) $(INCLUDE)

clean:
	$(if $(BUILD_DIR), rm -rf $(BUILD_DIR)/*)
	mkdir $(BUILD_DIR)

lib.o:
	g++ -c $(SRC_DIR)/lib.cpp -o $(BUILD_DIR)/lib.o -shared -fPIC $(LIBS) $(INCLUDE)

parser.o:
	g++ -c $(SRC_DIR)/parser.cpp -o $(BUILD_DIR)/parser.o -shared -fPIC  $(LIBS) $(INCLUDE)
