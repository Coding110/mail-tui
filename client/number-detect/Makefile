CXX=g++
DEBUG=-g
LIB=-lcurl -lpthread
INC=
COM_SRC=detect.cpp common.cpp cJSON.cpp config.cpp thdpool.cpp log.cpp

TARG_DEMO=demo
DEMO_SRC=demo.cpp

TARG_MAIN=main
MAIN_SRC=main.cpp

all: clean $(TARG_MAIN)

$(TARG_MAIN):
	$(CXX) $(COM_SRC) $(MAIN_SRC) -o $@ $(LIB) $(INC) $(DEBUG)

$(TARG_DEMO):
	$(CXX) $(COM_SRC) $(DEMO_SRC) -o $@ $(LIB) $(INC) $(DEBUG)

clean:
	rm -f $(TARG_MAIN) $(TARG_DEMO)
