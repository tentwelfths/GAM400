# Straight forward Makefile to compile all examples in a row

# If cross-compiling, you may wish to set the following environment variable to the root location of 
# your 'sdk'
# SDKSTAGE=/home/foo/raspberrypi

INCDIR=-I./Common -I$(SDKSTAGE)/opt/vc/include -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux

LIBS=-lGLESv2 -lEGL -lpng -lm -lbcm_host -L$(SDKSTAGE)/opt/vc/lib

CFLAGS+=-DRPI_NO_X

# CFLAGS+=-DRPI_NO_X -DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi -I$(SDKSTAGE)/opt/vc/include/ -I./ -I$(SDKSTAGE)/opt/vc/lib



MYSRC=./main.cpp GraphicsSystem.cpp NetworkingSystem.cpp MCP3008SPI.cpp GPIOPin.cpp AudioSystem.cpp Debug.cpp Controllers.cpp

default: all

all: ./SRC

clean: all

./SRC: ${MYSRC}
	g++ -std=c++11 -I/usr/local/include $(CFLAGS) ${COMMONSRC} ${MYSRC} -o ./$@ ${INCDIR} ${LIBS} -lpthread -Wl,-rpath=/usr/local/lib /usr/local/lib/libfmod.so /usr/local/lib/libfmodstudio.so
