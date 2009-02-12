LIBRARY = -lOpenMesh_Core -lOpenMesh_Tools -lPocoFoundation -lPocoNet -lPocoUtil -lglut #-lprofiler	
OPTIONS =  -Wall -g -O2 
BASE = bitstring.o ppmesh.o vertexid.o gfmesh.o packetid.o
DISPLAY = render.o visiblepq.o view.o
STREAM = receiver.o
PDISPLAY = prender.o pvisiblepq.o view.o 
PSTREAM = preceiver.o
OBJECTS = bitstring.o ppmesh.o vertexid.o gfmesh.o render.o receiver.o visiblepq.o 
OBJECTS2 = bitstring.o ppmesh.o vertexid.o gfmesh.o prender.o preceiver.o pvisiblepq.o packetid.o view.o
CC = g++
CTAG = ctags
ALL = mkbalancedpm display server client encode decode packetizer pserver pclient packetinfo testview testsplit pm32to64

all: $(ALL)
tags: *.cc *.hh
	$(CTAG) -R *
ppmesh.o:ppmesh.cc ppmesh.hh huffman.hh
	$(CC) $(OPTIONS) -c $<
%.o:%.cc %.hh 
	$(CC) $(OPTIONS) -c $< 
mkbalancedpm: mkbalancedpm.cc
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $<
display: display.cc $(BASE) $(PDISPLAY)
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE) $(PDISPLAY)
server: server.cc $(BASE)
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE)
client: client.cc $(BASE) $(STREAM) $(DISPLAY)
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE) $(STREAM) $(DISPLAY)
encode: encode.cc $(BASE)
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE)
decode: decode.cc $(BASE)
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE)
packetizer: packetizer.cc $(BASE)
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE)
packetinfo: packetinfo.cc $(BASE)
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE)
pserver: pserver.cc $(BASE)
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE)
pclient: pclient.cc $(BASE) $(PSTREAM) $(PDISPLAY)
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE) $(PSTREAM) $(PDISPLAY)
testview: testview.cc $(BASE) view.o 
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE) view.o
testsplit: testsplit.cc $(BASE) view.o
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE) view.o
pm32to64: pm32to64.cc $(BASE)
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE)
clean:
	rm *.o
	rm $(ALL)
