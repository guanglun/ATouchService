ANDROID_ATOUCH_PATH=/data/local/tmp

NDK_PATH=/home/share/Android

SRCDIR = 				\
		src				\
		src/touch		\
		src/socket		\
		src/game

OBJDIR = output/build
BINDIR = output/bin
BINAME = ATouchService
KILLSH_NAME = kill_atouch.sh
KILLSH_PATH = tool

INCLUDES = $(foreach n,$(SRCDIR),-I $(n))

CC = $(NDK_PATH)/android-ndk-r13b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc
TARGET = $(BINDIR)/$(BINAME)
SRCS = $(foreach n,$(SRCDIR),$(n)/*.c)
SRCS := $(wildcard $(SRCS)) 
OBJS = $(SRCS:.c=.o)
OBJS :=$(notdir $(OBJS))  
OBJS := $(foreach n,$(OBJS),$(OBJDIR)/$(n))
CCFLAGS = -pie -fPIE --sysroot=$(NDK_PATH)/android-ndk-r13b/platforms/android-24/arch-arm

$(TARGET):$(OBJS)
	$(CC) $(CCFLAGS) -o $@ $(OBJS)

$(OBJS):$(SRCS)
	@echo $(filter %$(notdir $(@:.o=.c)),$(SRCS))
	@echo LIBFILE: $(LIBFILE)
	@echo SRCS: $(SRCS)
	@echo OBJS: $(OBJS)
	$(CC) $(CCFLAGS) -o $@ -c $(filter %/$(notdir $(@:.o=.c)),$(SRCS)) $(INCLUDES)

clean:
	rm $(OBJDIR)/*

run:
	# adb push $(KILLSH_PATH)/$(KILLSH_NAME) $(ANDROID_ATOUCH_PATH)/
	# adb shell chmod 777 $(ANDROID_ATOUCH_PATH)/$(KILLSH_NAME)
	# adb shell $(ANDROID_ATOUCH_PATH)/$(KILLSH_NAME)
	
	adb push $(TARGET) $(ANDROID_ATOUCH_PATH)
	adb shell chmod 777 $(ANDROID_ATOUCH_PATH)/$(BINAME)
	adb shell $(ANDROID_ATOUCH_PATH)/$(BINAME)

cprun:
	adb shell cp /mnt/sdcard/ATouch/ATouchService $(ANDROID_ATOUCH_PATH)
	adb shell chmod 777 $(ANDROID_ATOUCH_PATH)/$(BINAME)
	adb shell $(ANDROID_ATOUCH_PATH)/$(BINAME)

