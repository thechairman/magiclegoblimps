CC = g++

LIBS = -lmysqlcppconn -lboost_system -lboost_thread
LIBDIRS = -L ../lib

INCLUDES = -I ../include 

DEBUG = -g3


all: AdminClient VideoClient connection testdata


TESTDATA_DEPS = ../build/DataFile.o ../build/Robot.o 

testdata: testdata.cpp ${TESTDATA_DEPS}
	@echo "Building: $@"
	${CC} -o"$@" ${DEBUG} ${LIBDIRS} ${LIBS} ${INCLUDES} -I ../ $@.cpp ${TESTDATA_DEPS}

AdminClient: AdminClient.cpp
	@echo "Building: $@"
	${CC} -o"$@" ${DEBUG} ${LIBDIRS} ${LIBS} ${INCLUDES} $@.cpp
	@echo "Done building: $@"

VideoClient: VideoClient.cpp
	@echo "Building: $@"
	${CC} -o"$@" ${DEBUG} ${LIBDIRS} ${LIBS} ${INCLUDES} $@.cpp
	@echo "Done building: $@"

connection: connection.java
	javac $@.java


../build/%.o: ../%.cpp
	@cd ../build && ${MAKE} ${@:../build/%.o=%.o}

clean:
	${RM} AdminClient VideoClient testdata connection.class

.PHONY: clean