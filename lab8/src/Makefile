JFLAGS= 
JC=javac
SOURCES=RMIClient.java  RMIInterface.java  RMISecurityInterface.java  RMISecurity.java  RMIServer.java
RMICFILES=RMISecurity RMIServer
SHELL=/bin/bash
CLASSES=$(SOURCES:.java=.class)

all: rmic
	rmiregistry &

rmic: $(CLASSES)
	rmic $(RMICFILES)

.PHONY: clean

clean:
	$(RM) *.class
	ps -ax | grep rmiregistry | awk '{if ($$5 == "rmiregistry") {print $$1; system("kill -9 "$$1)}}'

%.class:
	$(JC) $(JFLAGS) $*.java 
