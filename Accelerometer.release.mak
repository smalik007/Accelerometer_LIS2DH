###########################################################
# Makefile generated by xIDE                               
#                                                          
# Project: Accelerometer
# Configuration: Release
# Generated: Sat Dec 9 21:19:50 2017
#                                                          
# WARNING: Do not edit this file. Any changes will be lost 
#          when the project is rebuilt.                    
#                                                          
###########################################################

OUTPUT=Accelerometer
OUTDIR=C:/ADK3.5.1/apps/Accelerometer_LIS2DH
HARDWARE_INDEX=1
DEFS=

DEBUGTRANSPORT=SPITRANS=USB SPIPORT=0
EXECUTION_MODE=hw_default
STACKSIZE=0
TRANSPORT=bcsp
FIRMWARE=unified
FIRMWAREIMAGE=
LIBRARY_VERSION=
GENERATE_MAP=1

INPUTS=\
      accelerometer.c\
      main.c\
      accelerometer.h

-include Accelerometer.mak
include $(BLUELAB)/Makefile.vm
