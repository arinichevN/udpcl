#!/bin/bash

APP=udpcl
INST_DIR=/usr/sbin
CONF_DIR=/etc/controller

function move_bin {
	([ -d $INST_DIR ] || mkdir $INST_DIR) && \
	cp bin $INST_DIR/$APP && \
	chmod a+x $INST_DIR/$APP && \
	chmod og-w $INST_DIR/$APP && \
	echo "Your $APP executable file: $INST_DIR/$APP";
}


#builds hardware independent application (functionality may be reduced)
function for_all {
	cd lib && \
	./build.sh for_a20 && \
	cd ../ && \
	#gcc -DP_ALL main.c -o bin -L./lib -lpac && \
	gcc -D_REENTRANT -DP_A20 main.c -o bin -L./lib -lpac -lpthread && \
	move_bin && \
	echo "Application $APP successfully installed"
}
#builds hardware independent application (functionality may be reduced)
function for_test {
	cd lib && \
	./build.sh for_a20 && \
	cd ../ && \
	#gcc -DMODE_DEBUG -DP_ALL main.c -o bin -L./lib -lpac && \
	gcc -D_REENTRANT -DMODE_DEBUG -DP_A20 main.c -o bin -L./lib -lpac -lpthread && \
	echo "Application $APP successfully installed. Launch command: sudo ./bin"
}


f=$1
${f}
