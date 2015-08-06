#Debug configuration of GetResByHTTP
#Program Name
PROGNAME=archiver
PROGNAME2=dearchiver

#Configuration
CONF=Release
#Macros
ARCH=Linux-x86
DESTDIR=dst
BUILDDIR=bld

#Environment
MKDIR=mkdir
RM=rm
CC=gcc
CCC=/app/gcc/4.8.0/bin/g++
CXX=/app/gcc/4.8.0/bin/g++

#Compiler Flags
CCFLAGS=-std=c++11 -gdwarf-3 -pthread -g
CXXFLAGS=-std=c++11 -gdwarf-3 -pthread -g

#Directory for Object files - OBJECTDIR
#Object Files - OBJECTFILES
#Vars will load from makedata/Makefile-Objectives.mk
include makedata/Makefile-Objectives.mk

JUSTDOIT=.yesJustDoIt

build:	${JUSTDOIT}
	@echo "**************************Build started**************************";
	${MAKE}  -f makedata/Makefile-${CONF}.mk ${DESTDIR}/${CONF}/${ARCH}/${PROGNAME}
	${MAKE}  -f makedata/Makefile-${CONF}.mk ${DESTDIR}/${CONF}/${ARCH}/${PROGNAME2}

${DESTDIR}/${CONF}/${ARCH}/${PROGNAME}: ${OBJECTFILES}
	${MKDIR} -p ${DESTDIR}/${CONF}/${ARCH}
#	linker      	dest			  program_name
	${LINK.cc} -o ${DESTDIR}/${CONF}/${ARCH}/${PROGNAME} ${OBJECTFILES} ${LDLIBSOPTIONS}

${DESTDIR}/${CONF}/${ARCH}/${PROGNAME2}: ${OBJECTFILES2}
	${MKDIR} -p ${DESTDIR}/${CONF}/${ARCH}
#	linker      	dest			  program_name
	${LINK.cc} -o ${DESTDIR}/${CONF}/${ARCH}/${PROGNAME2} ${OBJECTFILES2} ${LDLIBSOPTIONS}
	@echo "**************************Build finished*************************";

${OBJECTDIR}/src/BitBunch.o: ./src/BitBunch.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/BitBunch.o ./src/BitBunch.cpp
-include ${OBJECTDIR}/src/BitBunch.o.d

${OBJECTDIR}/src/CompressedData.o: ./src/CompressedData.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CompressedData.o ./src/CompressedData.cpp
-include ${OBJECTDIR}/src/CompressedData.o.d

${OBJECTDIR}/src/CompressedFileReader.o: ./src/CompressedFileReader.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CompressedFileReader.o ./src/CompressedFileReader.cpp
-include ${OBJECTDIR}/src/CompressedFileReader.o.d

${OBJECTDIR}/src/Compressor.o: ./src/Compressor.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Compressor.o ./src/Compressor.cpp
-include ${OBJECTDIR}/src/Compressor.o.d

${OBJECTDIR}/src/dearchiver.o: ./src/dearchiver.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/dearchiver.o ./src/dearchiver.cpp
-include ${OBJECTDIR}/src/dearchiver.o.d

${OBJECTDIR}/src/DecompressedPageWriter.o: ./src/DecompressedPageWriter.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DecompressedPageWriter.o ./src/DecompressedPageWriter.cpp
-include ${OBJECTDIR}/src/DecompressedPageWriter.o.d

${OBJECTDIR}/src/Decompression.o: ./src/Decompression.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Decompression.o ./src/Decompression.cpp
-include ${OBJECTDIR}/src/Decompression.o.d

${OBJECTDIR}/src/Decompressor.o: ./src/Decompressor.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Decompressor.o ./src/Decompressor.cpp
-include ${OBJECTDIR}/src/Decompressor.o.d

${OBJECTDIR}/src/FrequencyCounter.o: ./src/FrequencyCounter.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/FrequencyCounter.o ./src/FrequencyCounter.cpp
-include ${OBJECTDIR}/src/FrequencyCounter.o.d

${OBJECTDIR}/src/HammanData.o: ./src/HammanData.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HammanData.o ./src/HammanData.cpp
-include ${OBJECTDIR}/src/HammanData.o.d

${OBJECTDIR}/src/HammanTreeNode.o: ./src/HammanTreeNode.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/HammanTreeNode.o ./src/HammanTreeNode.cpp
-include ${OBJECTDIR}/src/HammanTreeNode.o.d

${OBJECTDIR}/src/main.o: ./src/main.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/main.o ./src/main.cpp
-include ${OBJECTDIR}/src/main.o.d

${OBJECTDIR}/src/RawPage.o: ./src/RawPage.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/RawPage.o ./src/RawPage.cpp
-include ${OBJECTDIR}/src/RawPage.o.d

${OBJECTDIR}/src/Reader.o: ./src/Reader.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Reader.o ./src/Reader.cpp
-include ${OBJECTDIR}/src/Reader.o.d

${OBJECTDIR}/src/Writer.o: ./src/Writer.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	@if [ -f $@.d ];\
	then \
		${RM} $@.d ;\
	fi
	$(COMPILE.cc) $(CXXFLAGS) -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Writer.o ./src/Writer.cpp
-include ${OBJECTDIR}/src/Writer.o.d


.yesJustDoIt:
