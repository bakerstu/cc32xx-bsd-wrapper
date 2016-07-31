include path.mk

VPATH = src

INCLUDES = -Iinclude \
           -I$(CC3200SDKPATH)/simplelink/ \
           -I$(CC3200SDKPATH)/simplelink/include \
           -I$(CC3200SDKPATH)/simplelink_extlib/provisioninglib

ARCHFLAGS = -g -MD -MP -march=armv7-m -mthumb -mfloat-abi=soft
COREFLAGS = $(ARCHFLAGS) -Wall -Werror -Wno-unknown-pragmas \
            -fdata-sections -ffunction-sections \
            -fno-builtin -fno-stack-protector -mfix-cortex-m3-ldrd \
            -D_REENT_SMALL

ASFLAGS  = -c $(ARCHFLAGS)
CFLAGS   = -c $(COREFLAGS) -std=gnu99 $(INCLUDES)
CXXFLAGS = -c $(COREFLAGS) -std=gnu++0x -D_ISOC99_SOURCE \
           -D__STDC_FORMAT_MACROS -fno-exceptions -fno-rtti $(INCLUDES)
AROPTS   = D


LIBNAME = cc32xx-bsd-wrapper.lib

FULLPATHCSRCS = $(wildcard $(VPATH)/*.c)
CSRCS = $(notdir $(FULLPATHCSRCS))
OBJS = $(CSRCS:.c=.o)

.PHONY: all
all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(AR) crs$(AROPTS) $(LIBNAME) $(OBJS)

-include $(OBJS:.o=.d)

.SUFFIXES:
.SUFFIXES: .o .c .cxx .cpp .S

.cpp.o:
	$(CXX) $(CXXFLAGS) -MD -MF $*.d $< -o $@

.cxx.o:
	$(CXX) $(CXXFLAGS) -MD -MF $*.d $< -o $@

.S.o:
	$(AS) $(ASFLAGS) -MD -MF $*.d $< -o $@

.c.o:
	$(CC) $(CFLAGS) -MD -MF $*.d $< -o $@

clean:
	rm *.o *.d *.lib
