CXX=g++
DEBUG=-g
LIB=-lcurl -lpthread
INC=
COM_SRC=detect.cpp common.cpp cJSON.cpp config.cpp thdpool.cpp log.cpp

TARG_DEMO=demo
DEMO_SRC=demo.cpp

TARG_MAIN=main
MAIN_SRC=main.cpp
DST_DIR=../linux

all: clean $(TARG_MAIN)

$(TARG_MAIN):
	$(CXX) $(COM_SRC) $(MAIN_SRC) -o $@ $(LIB) $(INC) $(DEBUG)
	cp $(TARG_MAIN) $(DST_DIR)

$(TARG_DEMO):
	$(CXX) $(COM_SRC) $(DEMO_SRC) -o $@ $(LIB) $(INC) $(DEBUG)

clean:
	rm -f $(TARG_MAIN) $(TARG_DEMO)
