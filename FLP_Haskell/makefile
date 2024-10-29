.PHONY: all clean zip

SRC_DIR = src
OPT = -Wall

MAIN = $(SRC_DIR)/Main.hs
MY_UTILS = $(SRC_DIR)/MyUtils.hs
TYPES = $(SRC_DIR)/Types.hs
UNREACHABLE = $(SRC_DIR)/Unreachable.hs
SINK = $(SRC_DIR)/Sink.hs
MINIMALIZE = $(SRC_DIR)/Minimalize.hs
CANON = $(SRC_DIR)/Canon.hs

FILES = $(MAIN) $(MY_UTILS) $(TYPES) $(UNREACHABLE) $(SINK) $(MINIMALIZE) $(CANON)

all: $(FILES)
	ghc $(OPT) -o flp21-fun $^

clean:
	rm -f $(SRC_DIR)/*.hi $(SRC_DIR)/*.o flp21-fun flp-fun-xsladk07.zip

zip:
	zip flp-fun-xsladk07.zip $(SRC_DIR)/*.hs makefile README.md tests/*
