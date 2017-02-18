#!/bin/bash

APP=udpcl
APP_DBG=`printf "%s_dbg" "$APP"`
INST_DIR=/usr/sbin
CONF_DIR=/etc/controller

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
	gcc $1 -c app.c -D_REENTRANT -lpthread && \
	gcc $1 -c crc.c
	gcc $1 -c timef.c && \
	gcc $1 -c udp.c && \
	gcc $1 -c util.c && \
	cd acp && \
	gcc $1 -c main.c && \

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
	gcc -D_REENTRANT $1 main.c -o $2 -lpthread -L./lib -lpac && echo "Application successfully compiled. Launch command: sudo ./"$2
}


function full {
	build $MODE_DEBUG $APP && \
	move_bin
}



f=$1
${f}
