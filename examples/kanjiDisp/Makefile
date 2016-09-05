ARDUINO_VARIANT = nodemcu
SERIAL_PORT := COM3

ARDUINO_HOME     := c:/Users/nari/AppData/Local/Arduino15/packages/esp8266/hardware/esp8266/2.3.0
ESP8266_ARDUINO_HOME := /c/Users/nari/AppData/Local/Arduino15/packages/esp8266
XTENSA_TOOLCHAIN := $(ESP8266_ARDUINO_HOME)/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/
ESPTOOL := $(ESP8266_ARDUINO_HOME)/tools/esptool/0.4.9/esptool.exe

GLOBAL_USER_LIBDIR := /c/Program\ Files\ (x86)/Arduino/libraries
LOCAL_USER_LIBDIR := c:/Users/nari/Documents/Arduino/libraries

ARDUINO_LIBS := SPI
USER_LIBS := Adafruit_GFX Adafruit_ILI9341
ULIBDIRS := $(LOCAL_USER_LIBDIR)/Adafruit_GFX_Library
ULIBDIRS += $(LOCAL_USER_LIBDIR)/Adafruit_ILI9341-master

include ./esp8266Arduino.mk

