#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/BoreaCode.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/BoreaCode.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../system_config/Board_UDB5/dspic33fj256gp710a/system.c ../fileio/drivers/sd_spi/sd_spi.c ../fileio/src/fileio.c ../filter.c ../funzioni_magnetometro.c ../funzioni_matematiche.c ../attitude.c ../MemHandling.c ../spi.c ../propellers.c ../guidance.c ../SPA_Matrix.c ../comm.c ../MPU6000.c ../receiver.c ../ref_gen.c ../state_machine.c ../main.c ../low_level_drivers/spi/src/drv_spi_16bit.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1147500432/system.o ${OBJECTDIR}/_ext/691940608/sd_spi.o ${OBJECTDIR}/_ext/603767206/fileio.o ${OBJECTDIR}/_ext/1472/filter.o ${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o ${OBJECTDIR}/_ext/1472/funzioni_matematiche.o ${OBJECTDIR}/_ext/1472/attitude.o ${OBJECTDIR}/_ext/1472/MemHandling.o ${OBJECTDIR}/_ext/1472/spi.o ${OBJECTDIR}/_ext/1472/propellers.o ${OBJECTDIR}/_ext/1472/guidance.o ${OBJECTDIR}/_ext/1472/SPA_Matrix.o ${OBJECTDIR}/_ext/1472/comm.o ${OBJECTDIR}/_ext/1472/MPU6000.o ${OBJECTDIR}/_ext/1472/receiver.o ${OBJECTDIR}/_ext/1472/ref_gen.o ${OBJECTDIR}/_ext/1472/state_machine.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1147500432/system.o.d ${OBJECTDIR}/_ext/691940608/sd_spi.o.d ${OBJECTDIR}/_ext/603767206/fileio.o.d ${OBJECTDIR}/_ext/1472/filter.o.d ${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o.d ${OBJECTDIR}/_ext/1472/funzioni_matematiche.o.d ${OBJECTDIR}/_ext/1472/attitude.o.d ${OBJECTDIR}/_ext/1472/MemHandling.o.d ${OBJECTDIR}/_ext/1472/spi.o.d ${OBJECTDIR}/_ext/1472/propellers.o.d ${OBJECTDIR}/_ext/1472/guidance.o.d ${OBJECTDIR}/_ext/1472/SPA_Matrix.o.d ${OBJECTDIR}/_ext/1472/comm.o.d ${OBJECTDIR}/_ext/1472/MPU6000.o.d ${OBJECTDIR}/_ext/1472/receiver.o.d ${OBJECTDIR}/_ext/1472/ref_gen.o.d ${OBJECTDIR}/_ext/1472/state_machine.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1147500432/system.o ${OBJECTDIR}/_ext/691940608/sd_spi.o ${OBJECTDIR}/_ext/603767206/fileio.o ${OBJECTDIR}/_ext/1472/filter.o ${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o ${OBJECTDIR}/_ext/1472/funzioni_matematiche.o ${OBJECTDIR}/_ext/1472/attitude.o ${OBJECTDIR}/_ext/1472/MemHandling.o ${OBJECTDIR}/_ext/1472/spi.o ${OBJECTDIR}/_ext/1472/propellers.o ${OBJECTDIR}/_ext/1472/guidance.o ${OBJECTDIR}/_ext/1472/SPA_Matrix.o ${OBJECTDIR}/_ext/1472/comm.o ${OBJECTDIR}/_ext/1472/MPU6000.o ${OBJECTDIR}/_ext/1472/receiver.o ${OBJECTDIR}/_ext/1472/ref_gen.o ${OBJECTDIR}/_ext/1472/state_machine.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o

# Source Files
SOURCEFILES=../system_config/Board_UDB5/dspic33fj256gp710a/system.c ../fileio/drivers/sd_spi/sd_spi.c ../fileio/src/fileio.c ../filter.c ../funzioni_magnetometro.c ../funzioni_matematiche.c ../attitude.c ../MemHandling.c ../spi.c ../propellers.c ../guidance.c ../SPA_Matrix.c ../comm.c ../MPU6000.c ../receiver.c ../ref_gen.c ../state_machine.c ../main.c ../low_level_drivers/spi/src/drv_spi_16bit.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/BoreaCode.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ256GP710A
MP_LINKER_FILE_OPTION=,--script=p33FJ256GP710A.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1147500432/system.o: ../system_config/Board_UDB5/dspic33fj256gp710a/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1147500432" 
	@${RM} ${OBJECTDIR}/_ext/1147500432/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1147500432/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../system_config/Board_UDB5/dspic33fj256gp710a/system.c  -o ${OBJECTDIR}/_ext/1147500432/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1147500432/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1147500432/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/691940608/sd_spi.o: ../fileio/drivers/sd_spi/sd_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/691940608" 
	@${RM} ${OBJECTDIR}/_ext/691940608/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/691940608/sd_spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../fileio/drivers/sd_spi/sd_spi.c  -o ${OBJECTDIR}/_ext/691940608/sd_spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/691940608/sd_spi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/691940608/sd_spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/603767206/fileio.o: ../fileio/src/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/603767206" 
	@${RM} ${OBJECTDIR}/_ext/603767206/fileio.o.d 
	@${RM} ${OBJECTDIR}/_ext/603767206/fileio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../fileio/src/fileio.c  -o ${OBJECTDIR}/_ext/603767206/fileio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/603767206/fileio.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/603767206/fileio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/filter.o: ../filter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/filter.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/filter.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../filter.c  -o ${OBJECTDIR}/_ext/1472/filter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/filter.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/filter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o: ../funzioni_magnetometro.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../funzioni_magnetometro.c  -o ${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/funzioni_matematiche.o: ../funzioni_matematiche.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/funzioni_matematiche.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/funzioni_matematiche.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../funzioni_matematiche.c  -o ${OBJECTDIR}/_ext/1472/funzioni_matematiche.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/funzioni_matematiche.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/funzioni_matematiche.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/attitude.o: ../attitude.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/attitude.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/attitude.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../attitude.c  -o ${OBJECTDIR}/_ext/1472/attitude.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/attitude.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/attitude.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/MemHandling.o: ../MemHandling.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MemHandling.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MemHandling.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MemHandling.c  -o ${OBJECTDIR}/_ext/1472/MemHandling.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MemHandling.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MemHandling.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/spi.o: ../spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../spi.c  -o ${OBJECTDIR}/_ext/1472/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/spi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/propellers.o: ../propellers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/propellers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/propellers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../propellers.c  -o ${OBJECTDIR}/_ext/1472/propellers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/propellers.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/propellers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/guidance.o: ../guidance.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/guidance.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/guidance.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../guidance.c  -o ${OBJECTDIR}/_ext/1472/guidance.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/guidance.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/guidance.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/SPA_Matrix.o: ../SPA_Matrix.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/SPA_Matrix.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SPA_Matrix.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SPA_Matrix.c  -o ${OBJECTDIR}/_ext/1472/SPA_Matrix.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/SPA_Matrix.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/SPA_Matrix.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/comm.o: ../comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/comm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../comm.c  -o ${OBJECTDIR}/_ext/1472/comm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/comm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/comm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/MPU6000.o: ../MPU6000.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MPU6000.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MPU6000.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MPU6000.c  -o ${OBJECTDIR}/_ext/1472/MPU6000.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MPU6000.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MPU6000.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/receiver.o: ../receiver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/receiver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/receiver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../receiver.c  -o ${OBJECTDIR}/_ext/1472/receiver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/receiver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/receiver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/ref_gen.o: ../ref_gen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ref_gen.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ref_gen.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ref_gen.c  -o ${OBJECTDIR}/_ext/1472/ref_gen.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/ref_gen.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ref_gen.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/state_machine.o: ../state_machine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/state_machine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/state_machine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../state_machine.c  -o ${OBJECTDIR}/_ext/1472/state_machine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/state_machine.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/state_machine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o: ../low_level_drivers/spi/src/drv_spi_16bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/275955384" 
	@${RM} ${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o.d 
	@${RM} ${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../low_level_drivers/spi/src/drv_spi_16bit.c  -o ${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1147500432/system.o: ../system_config/Board_UDB5/dspic33fj256gp710a/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1147500432" 
	@${RM} ${OBJECTDIR}/_ext/1147500432/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1147500432/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../system_config/Board_UDB5/dspic33fj256gp710a/system.c  -o ${OBJECTDIR}/_ext/1147500432/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1147500432/system.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1147500432/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/691940608/sd_spi.o: ../fileio/drivers/sd_spi/sd_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/691940608" 
	@${RM} ${OBJECTDIR}/_ext/691940608/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/691940608/sd_spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../fileio/drivers/sd_spi/sd_spi.c  -o ${OBJECTDIR}/_ext/691940608/sd_spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/691940608/sd_spi.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/691940608/sd_spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/603767206/fileio.o: ../fileio/src/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/603767206" 
	@${RM} ${OBJECTDIR}/_ext/603767206/fileio.o.d 
	@${RM} ${OBJECTDIR}/_ext/603767206/fileio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../fileio/src/fileio.c  -o ${OBJECTDIR}/_ext/603767206/fileio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/603767206/fileio.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/603767206/fileio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/filter.o: ../filter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/filter.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/filter.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../filter.c  -o ${OBJECTDIR}/_ext/1472/filter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/filter.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/filter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o: ../funzioni_magnetometro.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../funzioni_magnetometro.c  -o ${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/funzioni_magnetometro.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/funzioni_matematiche.o: ../funzioni_matematiche.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/funzioni_matematiche.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/funzioni_matematiche.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../funzioni_matematiche.c  -o ${OBJECTDIR}/_ext/1472/funzioni_matematiche.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/funzioni_matematiche.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/funzioni_matematiche.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/attitude.o: ../attitude.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/attitude.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/attitude.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../attitude.c  -o ${OBJECTDIR}/_ext/1472/attitude.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/attitude.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/attitude.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/MemHandling.o: ../MemHandling.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MemHandling.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MemHandling.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MemHandling.c  -o ${OBJECTDIR}/_ext/1472/MemHandling.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MemHandling.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MemHandling.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/spi.o: ../spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../spi.c  -o ${OBJECTDIR}/_ext/1472/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/spi.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/propellers.o: ../propellers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/propellers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/propellers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../propellers.c  -o ${OBJECTDIR}/_ext/1472/propellers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/propellers.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/propellers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/guidance.o: ../guidance.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/guidance.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/guidance.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../guidance.c  -o ${OBJECTDIR}/_ext/1472/guidance.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/guidance.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/guidance.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/SPA_Matrix.o: ../SPA_Matrix.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/SPA_Matrix.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SPA_Matrix.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SPA_Matrix.c  -o ${OBJECTDIR}/_ext/1472/SPA_Matrix.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/SPA_Matrix.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/SPA_Matrix.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/comm.o: ../comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/comm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../comm.c  -o ${OBJECTDIR}/_ext/1472/comm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/comm.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/comm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/MPU6000.o: ../MPU6000.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MPU6000.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MPU6000.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MPU6000.c  -o ${OBJECTDIR}/_ext/1472/MPU6000.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MPU6000.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MPU6000.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/receiver.o: ../receiver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/receiver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/receiver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../receiver.c  -o ${OBJECTDIR}/_ext/1472/receiver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/receiver.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/receiver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/ref_gen.o: ../ref_gen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ref_gen.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ref_gen.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ref_gen.c  -o ${OBJECTDIR}/_ext/1472/ref_gen.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/ref_gen.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ref_gen.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/state_machine.o: ../state_machine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/state_machine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/state_machine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../state_machine.c  -o ${OBJECTDIR}/_ext/1472/state_machine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/state_machine.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/state_machine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o: ../low_level_drivers/spi/src/drv_spi_16bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/275955384" 
	@${RM} ${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o.d 
	@${RM} ${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../low_level_drivers/spi/src/drv_spi_16bit.c  -o ${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o.d"        -g -omf=elf -O0 -I".." -I"." -I"../system_config/Board_UDB5/dspic33fj256gp710a" -I"../low_level_drivers/spi" -I"../" -I"../fileio/inc" -I"../fileio/src" -I"../fileio/drivers/sd_spi" -I"../fileio/config" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/275955384/drv_spi_16bit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/BoreaCode.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/BoreaCode.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf  -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=512,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path=".",--no-force-link,--smart-io,-Map="${DISTDIR}/BoreaCode.X.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/BoreaCode.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/BoreaCode.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=512,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path=".",--no-force-link,--smart-io,-Map="${DISTDIR}/BoreaCode.X.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/BoreaCode.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
