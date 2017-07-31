#!/bin/bash

APP=udpcl
APP_DBG=`printf "%s_dbg" "$APP"`
INST_DIR=/usr/sbin
CONF_DIR=/etc/controller

DEBUG_PARAM="-Wall -pedantic"
MODE_DEBUG=-DMODE_DEBUG

NONE=-DNOTHINGANDNOT

function move_bin {
	([ -d $INST_DIR ] || mkdir $INST_DIR) && \
	cp $APP $INST_DIR/$APP && \
	chmod a+x $INST_DIR/$APP && \
	chmod og-w $INST_DIR/$APP && \
	echo "Your $APP executable file: $INST_DIR/$APP";
}

function build_lib {
	gcc $1 -c app.c -D_REENTRANT $DEBUG_PARAM -lpthread && \
	gcc $1 -c crc.c $DEBUG_PARAM && \
	gcc $1 -c timef.c $DEBUG_PARAM && \
	gcc $1 -c udp.c $DEBUG_PARAM && \
	gcc $1 -c util.c $DEBUG_PARAM && \
	cd acp && \
	gcc $1 -c main.c $DEBUG_PARAM && \

	cd ../ && \
	echo "library: making archive..." && \
	rm -f libpac.a
	ar -crv libpac.a app.o crc.o timef.o udp.o util.o acp/main.o && echo "library: done"
	echo "library: done"
	rm -f *.o acp/*.o
}

#    1         2
#debug_mode bin_name
function build {
	cd lib && \
	build_lib $1 && \
	cd ../ 
	gcc -D_REENTRANT $1 main.c -o $2 $DEBUG_PARAM -lpthread -L./lib -lpac && echo "Application successfully compiled. Launch command: sudo ./"$2
}


function full {
	DEBUG_PARAM=$NONE
	build $MODE_DEBUG $APP && \
	move_bin
}

function part_debug {
	build $MODE_DEBUG $APP_DBG
}

f=$1
${f}
