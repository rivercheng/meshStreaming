LIBRARY = -lOpenMesh_Core -lOpenMesh_Tools -lPocoFoundation -lPocoNet -lPocoUtil -lglut #-lprofiler	
OPTIONS =  -Wall -g -O2 
BASE = bitstring.o ppmesh.o vertexid.o gfpmesh.o packetid.o
DISPLAY = render.o visiblepq.o view.o
STREAM = receiver.o
PDISPLAY = prender.o pvisiblepq.o view.o 
PSTREAM = preceiver.o
OBJECTS = bitstring.o ppmesh.o vertexid.o gfpmesh.o render.o receiver.o visiblepq.o 
OBJECTS2 = bitstring.o ppmesh.o vertexid.o gfpmesh.o prender.o preceiver.o pvisiblepq.o packetid.o view.o
CC = g++
CTAG = ctags
ALL = mkbalancedpm display server client encode decode packetizer pserver pclient packetinfo testview testsplit pm32to64 viewer testroot

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
viewer: viewer.cc gfmesh.o viewRender.o
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< gfmesh.o viewRender.o
testroot:testroot.cc $(BASE)
	$(CC) $(OPTIONS) $(LIBRARY) -o $@ $< $(BASE)
clean:
	rm *.o
	rm $(ALL)
# DO NOT DELETE

bitstring.o: bitstring.hh
client.o: /usr/include/Poco/Net/StreamSocket.h /usr/include/Poco/Net/Net.h
client.o: /usr/include/Poco/Foundation.h /usr/include/Poco/Config.h
client.o: /usr/include/Poco/Platform.h /usr/include/Poco/Platform_POSIX.h
client.o: /usr/include/Poco/Bugcheck.h /usr/include/Poco/Types.h
client.o: /usr/include/Poco/Net/Socket.h /usr/include/Poco/Net/SocketImpl.h
client.o: /usr/include/Poco/Net/SocketDefs.h /usr/include/unistd.h
client.o: /usr/include/features.h /usr/include/sys/cdefs.h
client.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
client.o: /usr/include/gnu/stubs-32.h /usr/include/bits/posix_opt.h
client.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
client.o: /usr/include/bits/confname.h /usr/include/getopt.h
client.o: /usr/include/errno.h /usr/include/bits/errno.h
client.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
client.o: /usr/include/asm-generic/errno.h
client.o: /usr/include/asm-generic/errno-base.h /usr/include/sys/types.h
client.o: /usr/include/time.h /usr/include/endian.h
client.o: /usr/include/bits/endian.h /usr/include/sys/select.h
client.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
client.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
client.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
client.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
client.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
client.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
client.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
client.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
client.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
client.o: /usr/include/sys/ttydefaults.h /usr/include/arpa/inet.h
client.o: /usr/include/netinet/in.h /usr/include/stdint.h
client.o: /usr/include/bits/wchar.h /usr/include/bits/in.h
client.o: /usr/include/bits/byteswap.h /usr/include/netinet/tcp.h
client.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
client.o: /usr/include/bits/netdb.h /usr/include/net/if.h
client.o: /usr/include/Poco/Net/SocketAddress.h
client.o: /usr/include/Poco/Net/IPAddress.h
client.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/Mutex.h
client.o: /usr/include/Poco/Exception.h /usr/include/Poco/ScopedLock.h
client.o: /usr/include/Poco/Mutex_POSIX.h /usr/include/pthread.h
client.o: /usr/include/sched.h /usr/include/bits/sched.h
client.o: /usr/include/signal.h /usr/include/bits/setjmp.h
client.o: /usr/include/Poco/Timespan.h /usr/include/Poco/Timestamp.h
client.o: /usr/include/Poco/Net/SocketStream.h
client.o: /usr/include/Poco/BufferedBidirectionalStreamBuf.h
client.o: /usr/include/Poco/BufferAllocator.h /usr/include/Poco/StreamUtil.h
client.o: /usr/include/Poco/Path.h /usr/include/Poco/Thread.h
client.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
client.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
client.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
client.o: /usr/include/Poco/AutoPtr.h ppmesh.hh
client.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
client.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
client.o: /usr/include/OpenMesh/Core/System/compiler.hh
client.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
client.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
client.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
client.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
client.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
client.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
client.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
client.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
client.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
client.o: /usr/include/bits/mathcalls.h
client.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
client.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
client.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
client.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
client.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
client.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
client.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
client.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
client.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
client.o: /usr/include/OpenMesh/Core/System/omstream.hh
client.o: /usr/include/OpenMesh/Core/System/mostream.hh
client.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
client.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
client.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
client.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
client.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
client.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
client.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
client.o: /usr/include/OpenMesh/Core/Utils/Property.hh
client.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
client.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
client.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
client.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
client.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
client.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
client.o: /usr/include/inttypes.h /usr/include/GL/glu.h
client.o: /usr/include/Poco/ThreadPool.h bitstring.hh huffman.hh vertexid.hh
client.o: commonType.hh render.hh visiblepq.hh
client.o: /usr/include/Poco/RunnableAdapter.h gfmesh.hh receiver.hh
decode.o: ppmesh.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
decode.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
decode.o: /usr/include/features.h /usr/include/sys/cdefs.h
decode.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
decode.o: /usr/include/gnu/stubs-32.h
decode.o: /usr/include/OpenMesh/Core/System/compiler.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
decode.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
decode.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
decode.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
decode.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
decode.o: /usr/include/bits/mathcalls.h
decode.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
decode.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
decode.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
decode.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
decode.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
decode.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
decode.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
decode.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
decode.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
decode.o: /usr/include/OpenMesh/Core/System/omstream.hh
decode.o: /usr/include/OpenMesh/Core/System/mostream.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
decode.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
decode.o: /usr/include/OpenMesh/Core/Utils/Property.hh
decode.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
decode.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
decode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
decode.o: /usr/include/Poco/Mutex.h /usr/include/Poco/Foundation.h
decode.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
decode.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
decode.o: /usr/include/Poco/Types.h /usr/include/Poco/Exception.h
decode.o: /usr/include/Poco/ScopedLock.h /usr/include/Poco/Mutex_POSIX.h
decode.o: /usr/include/pthread.h /usr/include/endian.h
decode.o: /usr/include/bits/endian.h /usr/include/sched.h
decode.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
decode.o: /usr/include/time.h /usr/include/bits/sched.h /usr/include/signal.h
decode.o: /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h
decode.o: /usr/include/bits/setjmp.h /usr/include/errno.h
decode.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
decode.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
decode.o: /usr/include/asm-generic/errno-base.h /usr/include/GL/glut.h
decode.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
decode.o: /usr/include/GL/glext.h /usr/include/inttypes.h
decode.o: /usr/include/stdint.h /usr/include/bits/wchar.h
decode.o: /usr/include/GL/glu.h /usr/include/Poco/Thread.h
decode.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
decode.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
decode.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
decode.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
decode.o: /usr/include/sys/select.h /usr/include/bits/select.h
decode.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h
decode.o: bitstring.hh huffman.hh vertexid.hh commonType.hh
display.o: ppmesh.hh
display.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
display.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
display.o: /usr/include/features.h /usr/include/sys/cdefs.h
display.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
display.o: /usr/include/gnu/stubs-32.h
display.o: /usr/include/OpenMesh/Core/System/compiler.hh
display.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
display.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
display.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
display.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
display.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
display.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
display.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
display.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
display.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
display.o: /usr/include/bits/mathcalls.h
display.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
display.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
display.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
display.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
display.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
display.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
display.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
display.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
display.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
display.o: /usr/include/OpenMesh/Core/System/omstream.hh
display.o: /usr/include/OpenMesh/Core/System/mostream.hh
display.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
display.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
display.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
display.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
display.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
display.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
display.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
display.o: /usr/include/OpenMesh/Core/Utils/Property.hh
display.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
display.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
display.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
display.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
display.o: /usr/include/Poco/Mutex.h /usr/include/Poco/Foundation.h
display.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
display.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
display.o: /usr/include/Poco/Types.h /usr/include/Poco/Exception.h
display.o: /usr/include/Poco/ScopedLock.h /usr/include/Poco/Mutex_POSIX.h
display.o: /usr/include/pthread.h /usr/include/endian.h
display.o: /usr/include/bits/endian.h /usr/include/sched.h
display.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
display.o: /usr/include/time.h /usr/include/bits/sched.h
display.o: /usr/include/signal.h /usr/include/bits/sigset.h
display.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/setjmp.h
display.o: /usr/include/errno.h /usr/include/bits/errno.h
display.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
display.o: /usr/include/asm-generic/errno.h
display.o: /usr/include/asm-generic/errno-base.h /usr/include/GL/glut.h
display.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
display.o: /usr/include/GL/glext.h /usr/include/inttypes.h
display.o: /usr/include/stdint.h /usr/include/bits/wchar.h
display.o: /usr/include/GL/glu.h /usr/include/Poco/Thread.h
display.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
display.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
display.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
display.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
display.o: /usr/include/sys/select.h /usr/include/bits/select.h
display.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h
display.o: bitstring.hh huffman.hh vertexid.hh commonType.hh logger.hh
display.o: /usr/include/sys/time.h packetid.hh prender.hh pvisiblepq.hh
display.o: /usr/include/Poco/RunnableAdapter.h
encode.o: ppmesh.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
encode.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
encode.o: /usr/include/features.h /usr/include/sys/cdefs.h
encode.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
encode.o: /usr/include/gnu/stubs-32.h
encode.o: /usr/include/OpenMesh/Core/System/compiler.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
encode.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
encode.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
encode.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
encode.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
encode.o: /usr/include/bits/mathcalls.h
encode.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
encode.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
encode.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
encode.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
encode.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
encode.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
encode.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
encode.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
encode.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
encode.o: /usr/include/OpenMesh/Core/System/omstream.hh
encode.o: /usr/include/OpenMesh/Core/System/mostream.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
encode.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
encode.o: /usr/include/OpenMesh/Core/Utils/Property.hh
encode.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
encode.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
encode.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
encode.o: /usr/include/Poco/Mutex.h /usr/include/Poco/Foundation.h
encode.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
encode.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
encode.o: /usr/include/Poco/Types.h /usr/include/Poco/Exception.h
encode.o: /usr/include/Poco/ScopedLock.h /usr/include/Poco/Mutex_POSIX.h
encode.o: /usr/include/pthread.h /usr/include/endian.h
encode.o: /usr/include/bits/endian.h /usr/include/sched.h
encode.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
encode.o: /usr/include/time.h /usr/include/bits/sched.h /usr/include/signal.h
encode.o: /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h
encode.o: /usr/include/bits/setjmp.h /usr/include/errno.h
encode.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
encode.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
encode.o: /usr/include/asm-generic/errno-base.h /usr/include/GL/glut.h
encode.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
encode.o: /usr/include/GL/glext.h /usr/include/inttypes.h
encode.o: /usr/include/stdint.h /usr/include/bits/wchar.h
encode.o: /usr/include/GL/glu.h /usr/include/Poco/Thread.h
encode.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
encode.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
encode.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
encode.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
encode.o: /usr/include/sys/select.h /usr/include/bits/select.h
encode.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h
encode.o: bitstring.hh huffman.hh vertexid.hh commonType.hh
gfmesh.o: ppmesh.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
gfmesh.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
gfmesh.o: /usr/include/features.h /usr/include/sys/cdefs.h
gfmesh.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
gfmesh.o: /usr/include/gnu/stubs-32.h
gfmesh.o: /usr/include/OpenMesh/Core/System/compiler.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
gfmesh.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
gfmesh.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
gfmesh.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
gfmesh.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
gfmesh.o: /usr/include/bits/mathcalls.h
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
gfmesh.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
gfmesh.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
gfmesh.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
gfmesh.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
gfmesh.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
gfmesh.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
gfmesh.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
gfmesh.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
gfmesh.o: /usr/include/OpenMesh/Core/System/omstream.hh
gfmesh.o: /usr/include/OpenMesh/Core/System/mostream.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
gfmesh.o: /usr/include/OpenMesh/Core/Utils/Property.hh
gfmesh.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
gfmesh.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
gfmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
gfmesh.o: /usr/include/Poco/Mutex.h /usr/include/Poco/Foundation.h
gfmesh.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
gfmesh.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
gfmesh.o: /usr/include/Poco/Types.h /usr/include/Poco/Exception.h
gfmesh.o: /usr/include/Poco/ScopedLock.h /usr/include/Poco/Mutex_POSIX.h
gfmesh.o: /usr/include/pthread.h /usr/include/endian.h
gfmesh.o: /usr/include/bits/endian.h /usr/include/sched.h
gfmesh.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
gfmesh.o: /usr/include/time.h /usr/include/bits/sched.h /usr/include/signal.h
gfmesh.o: /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h
gfmesh.o: /usr/include/bits/setjmp.h /usr/include/errno.h
gfmesh.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
gfmesh.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
gfmesh.o: /usr/include/asm-generic/errno-base.h /usr/include/GL/glut.h
gfmesh.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
gfmesh.o: /usr/include/GL/glext.h /usr/include/inttypes.h
gfmesh.o: /usr/include/stdint.h /usr/include/bits/wchar.h
gfmesh.o: /usr/include/GL/glu.h /usr/include/Poco/Thread.h
gfmesh.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
gfmesh.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
gfmesh.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
gfmesh.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
gfmesh.o: /usr/include/sys/select.h /usr/include/bits/select.h
gfmesh.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h
gfmesh.o: bitstring.hh huffman.hh vertexid.hh commonType.hh gfmesh.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/MeshIO.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/System/config.hh
mkbalancedpm.o: /usr/include/assert.h /usr/include/features.h
mkbalancedpm.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
mkbalancedpm.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
mkbalancedpm.o: /usr/include/OpenMesh/Core/System/compiler.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/SR_store.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/SR_types.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/SR_rbo.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/System/omstream.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/System/mostream.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/SR_binary_spec.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
mkbalancedpm.o: /usr/include/math.h /usr/include/bits/huge_val.h
mkbalancedpm.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
mkbalancedpm.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/SR_binary_vector_of_fundamentals.inl
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/SR_binary_vector_of_string.inl
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/SR_binary_vector_of_bool.inl
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/IOManager.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/Options.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/reader/BaseReader.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/importer/BaseImporter.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Utils/Property.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/writer/BaseWriter.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/exporter/BaseExporter.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/importer/ImporterT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/exporter/ExporterT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Utils/color_cast.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
mkbalancedpm.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Utils/getopt.h
mkbalancedpm.o: /usr/include/getopt.h
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Utils/Timer.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Decimater/DecimaterT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Utils/HeapT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Decimater/ModBaseT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Utils/Noncopyable.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Decimater/CollapseInfoT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Decimater/DecimaterT.cc
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Utils/Progress.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Decimater/ModQuadricT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Geometry/QuadricT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Decimater/ModQuadricT.cc
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Decimater/ModNormalFlippingT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Decimater/ModProgMeshT.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Decimater/ModProgMeshT.cc
mkbalancedpm.o: /usr/include/OpenMesh/Core/IO/BinaryHelper.hh
mkbalancedpm.o: /usr/include/OpenMesh/Core/Utils/Endian.hh
mkbalancedpm.o: /usr/include/OpenMesh/Tools/Decimater/ModIndependentSetsT.hh
packetid.o: packetid.hh vertexid.hh bitstring.hh ppmesh.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
packetid.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
packetid.o: /usr/include/features.h /usr/include/sys/cdefs.h
packetid.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
packetid.o: /usr/include/gnu/stubs-32.h
packetid.o: /usr/include/OpenMesh/Core/System/compiler.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
packetid.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
packetid.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
packetid.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
packetid.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
packetid.o: /usr/include/bits/mathcalls.h
packetid.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
packetid.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
packetid.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
packetid.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
packetid.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
packetid.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
packetid.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
packetid.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
packetid.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
packetid.o: /usr/include/OpenMesh/Core/System/omstream.hh
packetid.o: /usr/include/OpenMesh/Core/System/mostream.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
packetid.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
packetid.o: /usr/include/OpenMesh/Core/Utils/Property.hh
packetid.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
packetid.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
packetid.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
packetid.o: /usr/include/Poco/Mutex.h /usr/include/Poco/Foundation.h
packetid.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
packetid.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
packetid.o: /usr/include/Poco/Types.h /usr/include/Poco/Exception.h
packetid.o: /usr/include/Poco/ScopedLock.h /usr/include/Poco/Mutex_POSIX.h
packetid.o: /usr/include/pthread.h /usr/include/endian.h
packetid.o: /usr/include/bits/endian.h /usr/include/sched.h
packetid.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
packetid.o: /usr/include/time.h /usr/include/bits/sched.h
packetid.o: /usr/include/signal.h /usr/include/bits/sigset.h
packetid.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/setjmp.h
packetid.o: /usr/include/errno.h /usr/include/bits/errno.h
packetid.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
packetid.o: /usr/include/asm-generic/errno.h
packetid.o: /usr/include/asm-generic/errno-base.h /usr/include/GL/glut.h
packetid.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
packetid.o: /usr/include/GL/glext.h /usr/include/inttypes.h
packetid.o: /usr/include/stdint.h /usr/include/bits/wchar.h
packetid.o: /usr/include/GL/glu.h /usr/include/Poco/Thread.h
packetid.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
packetid.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
packetid.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
packetid.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
packetid.o: /usr/include/sys/select.h /usr/include/bits/select.h
packetid.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h
packetid.o: huffman.hh commonType.hh
packetinfo.o: packetid.hh vertexid.hh bitstring.hh ppmesh.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
packetinfo.o: /usr/include/OpenMesh/Core/System/config.hh
packetinfo.o: /usr/include/assert.h /usr/include/features.h
packetinfo.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
packetinfo.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
packetinfo.o: /usr/include/OpenMesh/Core/System/compiler.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
packetinfo.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
packetinfo.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
packetinfo.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
packetinfo.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
packetinfo.o: /usr/include/bits/mathcalls.h
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
packetinfo.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
packetinfo.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
packetinfo.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
packetinfo.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
packetinfo.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
packetinfo.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
packetinfo.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
packetinfo.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
packetinfo.o: /usr/include/OpenMesh/Core/System/omstream.hh
packetinfo.o: /usr/include/OpenMesh/Core/System/mostream.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
packetinfo.o: /usr/include/OpenMesh/Core/Utils/Property.hh
packetinfo.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
packetinfo.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
packetinfo.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
packetinfo.o: /usr/include/Poco/Mutex.h /usr/include/Poco/Foundation.h
packetinfo.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
packetinfo.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
packetinfo.o: /usr/include/Poco/Types.h /usr/include/Poco/Exception.h
packetinfo.o: /usr/include/Poco/ScopedLock.h /usr/include/Poco/Mutex_POSIX.h
packetinfo.o: /usr/include/pthread.h /usr/include/endian.h
packetinfo.o: /usr/include/bits/endian.h /usr/include/sched.h
packetinfo.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
packetinfo.o: /usr/include/time.h /usr/include/bits/sched.h
packetinfo.o: /usr/include/signal.h /usr/include/bits/sigset.h
packetinfo.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/setjmp.h
packetinfo.o: /usr/include/errno.h /usr/include/bits/errno.h
packetinfo.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
packetinfo.o: /usr/include/asm-generic/errno.h
packetinfo.o: /usr/include/asm-generic/errno-base.h /usr/include/GL/glut.h
packetinfo.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
packetinfo.o: /usr/include/GL/glext.h /usr/include/inttypes.h
packetinfo.o: /usr/include/stdint.h /usr/include/bits/wchar.h
packetinfo.o: /usr/include/GL/glu.h /usr/include/Poco/Thread.h
packetinfo.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
packetinfo.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
packetinfo.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
packetinfo.o: /usr/include/Poco/RefCountedObject.h
packetinfo.o: /usr/include/Poco/AutoPtr.h /usr/include/sys/select.h
packetinfo.o: /usr/include/bits/select.h /usr/include/bits/time.h
packetinfo.o: /usr/include/Poco/ThreadPool.h huffman.hh commonType.hh
packetizer.o: ppmesh.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
packetizer.o: /usr/include/OpenMesh/Core/System/config.hh
packetizer.o: /usr/include/assert.h /usr/include/features.h
packetizer.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
packetizer.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
packetizer.o: /usr/include/OpenMesh/Core/System/compiler.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
packetizer.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
packetizer.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
packetizer.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
packetizer.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
packetizer.o: /usr/include/bits/mathcalls.h
packetizer.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
packetizer.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
packetizer.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
packetizer.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
packetizer.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
packetizer.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
packetizer.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
packetizer.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
packetizer.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
packetizer.o: /usr/include/OpenMesh/Core/System/omstream.hh
packetizer.o: /usr/include/OpenMesh/Core/System/mostream.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
packetizer.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
packetizer.o: /usr/include/OpenMesh/Core/Utils/Property.hh
packetizer.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
packetizer.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
packetizer.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
packetizer.o: /usr/include/Poco/Mutex.h /usr/include/Poco/Foundation.h
packetizer.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
packetizer.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
packetizer.o: /usr/include/Poco/Types.h /usr/include/Poco/Exception.h
packetizer.o: /usr/include/Poco/ScopedLock.h /usr/include/Poco/Mutex_POSIX.h
packetizer.o: /usr/include/pthread.h /usr/include/endian.h
packetizer.o: /usr/include/bits/endian.h /usr/include/sched.h
packetizer.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
packetizer.o: /usr/include/time.h /usr/include/bits/sched.h
packetizer.o: /usr/include/signal.h /usr/include/bits/sigset.h
packetizer.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/setjmp.h
packetizer.o: /usr/include/errno.h /usr/include/bits/errno.h
packetizer.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
packetizer.o: /usr/include/asm-generic/errno.h
packetizer.o: /usr/include/asm-generic/errno-base.h /usr/include/GL/glut.h
packetizer.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
packetizer.o: /usr/include/GL/glext.h /usr/include/inttypes.h
packetizer.o: /usr/include/stdint.h /usr/include/bits/wchar.h
packetizer.o: /usr/include/GL/glu.h /usr/include/Poco/Thread.h
packetizer.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
packetizer.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
packetizer.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
packetizer.o: /usr/include/Poco/RefCountedObject.h
packetizer.o: /usr/include/Poco/AutoPtr.h /usr/include/sys/select.h
packetizer.o: /usr/include/bits/select.h /usr/include/bits/time.h
packetizer.o: /usr/include/Poco/ThreadPool.h bitstring.hh huffman.hh
packetizer.o: vertexid.hh commonType.hh packetid.hh
pclient.o: /usr/include/Poco/Net/StreamSocket.h /usr/include/Poco/Net/Net.h
pclient.o: /usr/include/Poco/Foundation.h /usr/include/Poco/Config.h
pclient.o: /usr/include/Poco/Platform.h /usr/include/Poco/Platform_POSIX.h
pclient.o: /usr/include/Poco/Bugcheck.h /usr/include/Poco/Types.h
pclient.o: /usr/include/Poco/Net/Socket.h /usr/include/Poco/Net/SocketImpl.h
pclient.o: /usr/include/Poco/Net/SocketDefs.h /usr/include/unistd.h
pclient.o: /usr/include/features.h /usr/include/sys/cdefs.h
pclient.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
pclient.o: /usr/include/gnu/stubs-32.h /usr/include/bits/posix_opt.h
pclient.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pclient.o: /usr/include/bits/confname.h /usr/include/getopt.h
pclient.o: /usr/include/errno.h /usr/include/bits/errno.h
pclient.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pclient.o: /usr/include/asm-generic/errno.h
pclient.o: /usr/include/asm-generic/errno-base.h /usr/include/sys/types.h
pclient.o: /usr/include/time.h /usr/include/endian.h
pclient.o: /usr/include/bits/endian.h /usr/include/sys/select.h
pclient.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pclient.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
pclient.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
pclient.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
pclient.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
pclient.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
pclient.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
pclient.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
pclient.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
pclient.o: /usr/include/sys/ttydefaults.h /usr/include/arpa/inet.h
pclient.o: /usr/include/netinet/in.h /usr/include/stdint.h
pclient.o: /usr/include/bits/wchar.h /usr/include/bits/in.h
pclient.o: /usr/include/bits/byteswap.h /usr/include/netinet/tcp.h
pclient.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
pclient.o: /usr/include/bits/netdb.h /usr/include/net/if.h
pclient.o: /usr/include/Poco/Net/SocketAddress.h
pclient.o: /usr/include/Poco/Net/IPAddress.h
pclient.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/Mutex.h
pclient.o: /usr/include/Poco/Exception.h /usr/include/Poco/ScopedLock.h
pclient.o: /usr/include/Poco/Mutex_POSIX.h /usr/include/pthread.h
pclient.o: /usr/include/sched.h /usr/include/bits/sched.h
pclient.o: /usr/include/signal.h /usr/include/bits/setjmp.h
pclient.o: /usr/include/Poco/Timespan.h /usr/include/Poco/Timestamp.h
pclient.o: /usr/include/Poco/Net/SocketStream.h
pclient.o: /usr/include/Poco/BufferedBidirectionalStreamBuf.h
pclient.o: /usr/include/Poco/BufferAllocator.h /usr/include/Poco/StreamUtil.h
pclient.o: /usr/include/Poco/Path.h /usr/include/Poco/Thread.h
pclient.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
pclient.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
pclient.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
pclient.o: /usr/include/Poco/AutoPtr.h
pclient.o: /usr/include/OpenMesh/Core/Utils/Endian.hh
pclient.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
pclient.o: /usr/include/OpenMesh/Core/System/compiler.hh ppmesh.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
pclient.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
pclient.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
pclient.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
pclient.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
pclient.o: /usr/include/bits/mathcalls.h
pclient.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
pclient.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
pclient.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
pclient.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
pclient.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
pclient.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
pclient.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
pclient.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
pclient.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
pclient.o: /usr/include/OpenMesh/Core/System/omstream.hh
pclient.o: /usr/include/OpenMesh/Core/System/mostream.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
pclient.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
pclient.o: /usr/include/OpenMesh/Core/Utils/Property.hh
pclient.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
pclient.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
pclient.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
pclient.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
pclient.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
pclient.o: /usr/include/inttypes.h /usr/include/GL/glu.h
pclient.o: /usr/include/Poco/ThreadPool.h bitstring.hh huffman.hh vertexid.hh
pclient.o: commonType.hh prender.hh pvisiblepq.hh
pclient.o: /usr/include/Poco/RunnableAdapter.h packetid.hh logger.hh
pclient.o: /usr/include/sys/time.h preceiver.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/BinaryHelper.hh
pm32to64.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
pm32to64.o: /usr/include/features.h /usr/include/sys/cdefs.h
pm32to64.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
pm32to64.o: /usr/include/gnu/stubs-32.h
pm32to64.o: /usr/include/OpenMesh/Core/System/compiler.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/MeshIO.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/SR_store.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/SR_types.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/SR_rbo.hh
pm32to64.o: /usr/include/OpenMesh/Core/System/omstream.hh
pm32to64.o: /usr/include/OpenMesh/Core/System/mostream.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/SR_binary_spec.hh
pm32to64.o: /usr/include/OpenMesh/Core/Math/VectorT.hh /usr/include/math.h
pm32to64.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
pm32to64.o: /usr/include/bits/mathcalls.h
pm32to64.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
pm32to64.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/SR_binary_vector_of_fundamentals.inl
pm32to64.o: /usr/include/OpenMesh/Core/IO/SR_binary_vector_of_string.inl
pm32to64.o: /usr/include/OpenMesh/Core/IO/SR_binary_vector_of_bool.inl
pm32to64.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/IOManager.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/Options.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/reader/BaseReader.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/importer/BaseImporter.hh
pm32to64.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
pm32to64.o: /usr/include/OpenMesh/Core/Utils/Property.hh
pm32to64.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
pm32to64.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
pm32to64.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
pm32to64.o: /usr/include/OpenMesh/Core/IO/writer/BaseWriter.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/exporter/BaseExporter.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/importer/ImporterT.hh
pm32to64.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
pm32to64.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
pm32to64.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
pm32to64.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
pm32to64.o: /usr/include/OpenMesh/Core/IO/exporter/ExporterT.hh
pm32to64.o: /usr/include/OpenMesh/Core/Utils/color_cast.hh
pm32to64.o: /usr/include/OpenMesh/Core/Utils/Endian.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/BinaryHelper.hh
ppmesh.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
ppmesh.o: /usr/include/features.h /usr/include/sys/cdefs.h
ppmesh.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
ppmesh.o: /usr/include/gnu/stubs-32.h
ppmesh.o: /usr/include/OpenMesh/Core/System/compiler.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/MeshIO.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/SR_store.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/SR_types.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/SR_rbo.hh
ppmesh.o: /usr/include/OpenMesh/Core/System/omstream.hh
ppmesh.o: /usr/include/OpenMesh/Core/System/mostream.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/SR_binary_spec.hh
ppmesh.o: /usr/include/OpenMesh/Core/Math/VectorT.hh /usr/include/math.h
ppmesh.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
ppmesh.o: /usr/include/bits/mathcalls.h
ppmesh.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
ppmesh.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/SR_binary_vector_of_fundamentals.inl
ppmesh.o: /usr/include/OpenMesh/Core/IO/SR_binary_vector_of_string.inl
ppmesh.o: /usr/include/OpenMesh/Core/IO/SR_binary_vector_of_bool.inl
ppmesh.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/IOManager.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/Options.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/reader/BaseReader.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/importer/BaseImporter.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
ppmesh.o: /usr/include/OpenMesh/Core/Utils/Property.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
ppmesh.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
ppmesh.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
ppmesh.o: /usr/include/OpenMesh/Core/IO/writer/BaseWriter.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/exporter/BaseExporter.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/importer/ImporterT.hh
ppmesh.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
ppmesh.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
ppmesh.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
ppmesh.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
ppmesh.o: /usr/include/OpenMesh/Core/IO/exporter/ExporterT.hh
ppmesh.o: /usr/include/OpenMesh/Core/Utils/color_cast.hh
ppmesh.o: /usr/include/OpenMesh/Core/Utils/Endian.hh ppmesh.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
ppmesh.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
ppmesh.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
ppmesh.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
ppmesh.o: /usr/include/Poco/Mutex.h /usr/include/Poco/Foundation.h
ppmesh.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
ppmesh.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
ppmesh.o: /usr/include/Poco/Types.h /usr/include/Poco/Exception.h
ppmesh.o: /usr/include/Poco/ScopedLock.h /usr/include/Poco/Mutex_POSIX.h
ppmesh.o: /usr/include/pthread.h /usr/include/endian.h
ppmesh.o: /usr/include/bits/endian.h /usr/include/sched.h
ppmesh.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
ppmesh.o: /usr/include/time.h /usr/include/bits/sched.h /usr/include/signal.h
ppmesh.o: /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h
ppmesh.o: /usr/include/bits/setjmp.h /usr/include/errno.h
ppmesh.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
ppmesh.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
ppmesh.o: /usr/include/asm-generic/errno-base.h /usr/include/GL/glut.h
ppmesh.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
ppmesh.o: /usr/include/GL/glext.h /usr/include/inttypes.h
ppmesh.o: /usr/include/stdint.h /usr/include/bits/wchar.h
ppmesh.o: /usr/include/GL/glu.h /usr/include/Poco/Thread.h
ppmesh.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
ppmesh.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
ppmesh.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
ppmesh.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
ppmesh.o: /usr/include/sys/select.h /usr/include/bits/select.h
ppmesh.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h
ppmesh.o: bitstring.hh huffman.hh vertexid.hh commonType.hh gfmesh.hh
ppmesh.o: logger.hh /usr/include/sys/time.h packetid.hh
preceiver.o: /usr/include/Poco/Net/StreamSocket.h /usr/include/Poco/Net/Net.h
preceiver.o: /usr/include/Poco/Foundation.h /usr/include/Poco/Config.h
preceiver.o: /usr/include/Poco/Platform.h /usr/include/Poco/Platform_POSIX.h
preceiver.o: /usr/include/Poco/Bugcheck.h /usr/include/Poco/Types.h
preceiver.o: /usr/include/Poco/Net/Socket.h
preceiver.o: /usr/include/Poco/Net/SocketImpl.h
preceiver.o: /usr/include/Poco/Net/SocketDefs.h /usr/include/unistd.h
preceiver.o: /usr/include/features.h /usr/include/sys/cdefs.h
preceiver.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
preceiver.o: /usr/include/gnu/stubs-32.h /usr/include/bits/posix_opt.h
preceiver.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
preceiver.o: /usr/include/bits/confname.h /usr/include/getopt.h
preceiver.o: /usr/include/errno.h /usr/include/bits/errno.h
preceiver.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
preceiver.o: /usr/include/asm-generic/errno.h
preceiver.o: /usr/include/asm-generic/errno-base.h /usr/include/sys/types.h
preceiver.o: /usr/include/time.h /usr/include/endian.h
preceiver.o: /usr/include/bits/endian.h /usr/include/sys/select.h
preceiver.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
preceiver.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
preceiver.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
preceiver.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
preceiver.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
preceiver.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
preceiver.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
preceiver.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
preceiver.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
preceiver.o: /usr/include/sys/ttydefaults.h /usr/include/arpa/inet.h
preceiver.o: /usr/include/netinet/in.h /usr/include/stdint.h
preceiver.o: /usr/include/bits/wchar.h /usr/include/bits/in.h
preceiver.o: /usr/include/bits/byteswap.h /usr/include/netinet/tcp.h
preceiver.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
preceiver.o: /usr/include/bits/netdb.h /usr/include/net/if.h
preceiver.o: /usr/include/Poco/Net/SocketAddress.h
preceiver.o: /usr/include/Poco/Net/IPAddress.h
preceiver.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/Mutex.h
preceiver.o: /usr/include/Poco/Exception.h /usr/include/Poco/ScopedLock.h
preceiver.o: /usr/include/Poco/Mutex_POSIX.h /usr/include/pthread.h
preceiver.o: /usr/include/sched.h /usr/include/bits/sched.h
preceiver.o: /usr/include/signal.h /usr/include/bits/setjmp.h
preceiver.o: /usr/include/Poco/Timespan.h /usr/include/Poco/Timestamp.h
preceiver.o: /usr/include/Poco/Net/DatagramSocket.h
preceiver.o: /usr/include/Poco/Net/SocketStream.h
preceiver.o: /usr/include/Poco/BufferedBidirectionalStreamBuf.h
preceiver.o: /usr/include/Poco/BufferAllocator.h
preceiver.o: /usr/include/Poco/StreamUtil.h /usr/include/Poco/StreamCopier.h
preceiver.o: /usr/include/Poco/Path.h ppmesh.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
preceiver.o: /usr/include/OpenMesh/Core/System/config.hh
preceiver.o: /usr/include/assert.h
preceiver.o: /usr/include/OpenMesh/Core/System/compiler.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
preceiver.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
preceiver.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
preceiver.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
preceiver.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
preceiver.o: /usr/include/bits/mathcalls.h
preceiver.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
preceiver.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
preceiver.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
preceiver.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
preceiver.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
preceiver.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
preceiver.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
preceiver.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
preceiver.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
preceiver.o: /usr/include/OpenMesh/Core/System/omstream.hh
preceiver.o: /usr/include/OpenMesh/Core/System/mostream.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
preceiver.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
preceiver.o: /usr/include/OpenMesh/Core/Utils/Property.hh
preceiver.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
preceiver.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
preceiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
preceiver.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
preceiver.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
preceiver.o: /usr/include/inttypes.h /usr/include/GL/glu.h
preceiver.o: /usr/include/Poco/Thread.h /usr/include/Poco/Thread_POSIX.h
preceiver.o: /usr/include/Poco/Runnable.h /usr/include/Poco/SignalHandler.h
preceiver.o: /usr/include/setjmp.h /usr/include/Poco/Event.h
preceiver.o: /usr/include/Poco/Event_POSIX.h /usr/include/Poco/AutoPtr.h
preceiver.o: /usr/include/Poco/ThreadPool.h bitstring.hh huffman.hh
preceiver.o: vertexid.hh commonType.hh preceiver.hh pvisiblepq.hh
preceiver.o: /usr/include/Poco/RunnableAdapter.h packetid.hh logger.hh
preceiver.o: /usr/include/sys/time.h prender.hh
prender.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
prender.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
prender.o: /usr/include/inttypes.h /usr/include/features.h
prender.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
prender.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
prender.o: /usr/include/stdint.h /usr/include/bits/wchar.h
prender.o: /usr/include/GL/glu.h /usr/include/Poco/Mutex.h
prender.o: /usr/include/Poco/Foundation.h /usr/include/Poco/Config.h
prender.o: /usr/include/Poco/Platform.h /usr/include/Poco/Platform_POSIX.h
prender.o: /usr/include/Poco/Bugcheck.h /usr/include/Poco/Types.h
prender.o: /usr/include/Poco/Exception.h /usr/include/Poco/ScopedLock.h
prender.o: /usr/include/Poco/Mutex_POSIX.h /usr/include/pthread.h
prender.o: /usr/include/endian.h /usr/include/bits/endian.h
prender.o: /usr/include/sched.h /usr/include/bits/types.h
prender.o: /usr/include/bits/typesizes.h /usr/include/time.h
prender.o: /usr/include/bits/sched.h /usr/include/signal.h
prender.o: /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h
prender.o: /usr/include/bits/setjmp.h /usr/include/errno.h
prender.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
prender.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
prender.o: /usr/include/asm-generic/errno-base.h prender.hh ppmesh.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
prender.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
prender.o: /usr/include/OpenMesh/Core/System/compiler.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
prender.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
prender.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
prender.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
prender.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
prender.o: /usr/include/bits/mathcalls.h
prender.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
prender.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
prender.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
prender.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
prender.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
prender.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
prender.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
prender.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
prender.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
prender.o: /usr/include/OpenMesh/Core/System/omstream.hh
prender.o: /usr/include/OpenMesh/Core/System/mostream.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
prender.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
prender.o: /usr/include/OpenMesh/Core/Utils/Property.hh
prender.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
prender.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
prender.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
prender.o: /usr/include/Poco/Thread.h /usr/include/Poco/Thread_POSIX.h
prender.o: /usr/include/Poco/Runnable.h /usr/include/Poco/SignalHandler.h
prender.o: /usr/include/setjmp.h /usr/include/Poco/Event.h
prender.o: /usr/include/Poco/Event_POSIX.h
prender.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
prender.o: /usr/include/sys/select.h /usr/include/bits/select.h
prender.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h
prender.o: bitstring.hh huffman.hh vertexid.hh commonType.hh pvisiblepq.hh
prender.o: /usr/include/Poco/RunnableAdapter.h packetid.hh logger.hh
prender.o: /usr/include/sys/time.h gfmesh.hh
pserver.o: /usr/include/Poco/Net/TCPServer.h /usr/include/Poco/Net/Net.h
pserver.o: /usr/include/Poco/Foundation.h /usr/include/Poco/Config.h
pserver.o: /usr/include/Poco/Platform.h /usr/include/Poco/Platform_POSIX.h
pserver.o: /usr/include/Poco/Bugcheck.h /usr/include/Poco/Types.h
pserver.o: /usr/include/Poco/Net/ServerSocket.h
pserver.o: /usr/include/Poco/Net/Socket.h /usr/include/Poco/Net/SocketImpl.h
pserver.o: /usr/include/Poco/Net/SocketDefs.h /usr/include/unistd.h
pserver.o: /usr/include/features.h /usr/include/sys/cdefs.h
pserver.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
pserver.o: /usr/include/gnu/stubs-32.h /usr/include/bits/posix_opt.h
pserver.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pserver.o: /usr/include/bits/confname.h /usr/include/getopt.h
pserver.o: /usr/include/errno.h /usr/include/bits/errno.h
pserver.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pserver.o: /usr/include/asm-generic/errno.h
pserver.o: /usr/include/asm-generic/errno-base.h /usr/include/sys/types.h
pserver.o: /usr/include/time.h /usr/include/endian.h
pserver.o: /usr/include/bits/endian.h /usr/include/sys/select.h
pserver.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pserver.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
pserver.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
pserver.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
pserver.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
pserver.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
pserver.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
pserver.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
pserver.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
pserver.o: /usr/include/sys/ttydefaults.h /usr/include/arpa/inet.h
pserver.o: /usr/include/netinet/in.h /usr/include/stdint.h
pserver.o: /usr/include/bits/wchar.h /usr/include/bits/in.h
pserver.o: /usr/include/bits/byteswap.h /usr/include/netinet/tcp.h
pserver.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
pserver.o: /usr/include/bits/netdb.h /usr/include/net/if.h
pserver.o: /usr/include/Poco/Net/SocketAddress.h
pserver.o: /usr/include/Poco/Net/IPAddress.h
pserver.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/Mutex.h
pserver.o: /usr/include/Poco/Exception.h /usr/include/Poco/ScopedLock.h
pserver.o: /usr/include/Poco/Mutex_POSIX.h /usr/include/pthread.h
pserver.o: /usr/include/sched.h /usr/include/bits/sched.h
pserver.o: /usr/include/signal.h /usr/include/bits/setjmp.h
pserver.o: /usr/include/Poco/Timespan.h /usr/include/Poco/Timestamp.h
pserver.o: /usr/include/Poco/Net/StreamSocket.h /usr/include/Poco/Runnable.h
pserver.o: /usr/include/Poco/Thread.h /usr/include/Poco/Thread_POSIX.h
pserver.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
pserver.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
pserver.o: /usr/include/Poco/AutoPtr.h /usr/include/Poco/ThreadPool.h
pserver.o: /usr/include/Poco/Net/TCPServerConnection.h
pserver.o: /usr/include/Poco/Net/TCPServerConnectionFactory.h
pserver.o: /usr/include/Poco/Net/TCPServerParams.h
pserver.o: /usr/include/Poco/Net/DatagramSocket.h
pserver.o: /usr/include/Poco/Net/SocketStream.h
pserver.o: /usr/include/Poco/BufferedBidirectionalStreamBuf.h
pserver.o: /usr/include/Poco/BufferAllocator.h /usr/include/Poco/StreamUtil.h
pserver.o: /usr/include/Poco/DateTimeFormatter.h
pserver.o: /usr/include/Poco/DateTimeFormat.h
pserver.o: /usr/include/Poco/Util/ServerApplication.h
pserver.o: /usr/include/Poco/Util/Util.h /usr/include/Poco/Util/Application.h
pserver.o: /usr/include/Poco/Util/Subsystem.h
pserver.o: /usr/include/Poco/Util/LayeredConfiguration.h
pserver.o: /usr/include/Poco/Util/AbstractConfiguration.h
pserver.o: /usr/include/Poco/Util/OptionSet.h /usr/include/Poco/Util/Option.h
pserver.o: /usr/include/Poco/Util/OptionCallback.h /usr/include/Poco/Logger.h
pserver.o: /usr/include/Poco/Channel.h /usr/include/Poco/Configurable.h
pserver.o: /usr/include/Poco/Message.h /usr/include/Poco/Path.h
pserver.o: /usr/include/Poco/Util/HelpFormatter.h ppmesh.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
pserver.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
pserver.o: /usr/include/OpenMesh/Core/System/compiler.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
pserver.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
pserver.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
pserver.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
pserver.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
pserver.o: /usr/include/bits/mathcalls.h
pserver.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
pserver.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
pserver.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
pserver.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
pserver.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
pserver.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
pserver.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
pserver.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
pserver.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
pserver.o: /usr/include/OpenMesh/Core/System/omstream.hh
pserver.o: /usr/include/OpenMesh/Core/System/mostream.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
pserver.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
pserver.o: /usr/include/OpenMesh/Core/Utils/Property.hh
pserver.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
pserver.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
pserver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
pserver.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
pserver.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
pserver.o: /usr/include/inttypes.h /usr/include/GL/glu.h bitstring.hh
pserver.o: huffman.hh vertexid.hh commonType.hh packetid.hh
pvisiblepq.o: vertexid.hh bitstring.hh pvisiblepq.hh /usr/include/GL/glut.h
pvisiblepq.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
pvisiblepq.o: /usr/include/GL/glext.h /usr/include/inttypes.h
pvisiblepq.o: /usr/include/features.h /usr/include/sys/cdefs.h
pvisiblepq.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
pvisiblepq.o: /usr/include/gnu/stubs-32.h /usr/include/stdint.h
pvisiblepq.o: /usr/include/bits/wchar.h /usr/include/GL/glu.h
pvisiblepq.o: /usr/include/Poco/Thread.h /usr/include/Poco/Foundation.h
pvisiblepq.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
pvisiblepq.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
pvisiblepq.o: /usr/include/Poco/Types.h /usr/include/Poco/Mutex.h
pvisiblepq.o: /usr/include/Poco/Exception.h /usr/include/Poco/ScopedLock.h
pvisiblepq.o: /usr/include/Poco/Mutex_POSIX.h /usr/include/pthread.h
pvisiblepq.o: /usr/include/endian.h /usr/include/bits/endian.h
pvisiblepq.o: /usr/include/sched.h /usr/include/bits/types.h
pvisiblepq.o: /usr/include/bits/typesizes.h /usr/include/time.h
pvisiblepq.o: /usr/include/bits/sched.h /usr/include/signal.h
pvisiblepq.o: /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h
pvisiblepq.o: /usr/include/bits/setjmp.h /usr/include/errno.h
pvisiblepq.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pvisiblepq.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
pvisiblepq.o: /usr/include/asm-generic/errno-base.h
pvisiblepq.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
pvisiblepq.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
pvisiblepq.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
pvisiblepq.o: /usr/include/Poco/RefCountedObject.h
pvisiblepq.o: /usr/include/Poco/AutoPtr.h /usr/include/sys/select.h
pvisiblepq.o: /usr/include/bits/select.h /usr/include/bits/time.h
pvisiblepq.o: /usr/include/Poco/ThreadPool.h
pvisiblepq.o: /usr/include/Poco/RunnableAdapter.h ppmesh.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/System/config.hh
pvisiblepq.o: /usr/include/assert.h
pvisiblepq.o: /usr/include/OpenMesh/Core/System/compiler.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
pvisiblepq.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
pvisiblepq.o: /usr/include/bits/mathcalls.h
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
pvisiblepq.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
pvisiblepq.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/System/omstream.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/System/mostream.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Utils/Property.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
pvisiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
pvisiblepq.o: huffman.hh commonType.hh packetid.hh logger.hh
pvisiblepq.o: /usr/include/sys/time.h gfmesh.hh
receiver.o: /usr/include/Poco/Net/StreamSocket.h /usr/include/Poco/Net/Net.h
receiver.o: /usr/include/Poco/Foundation.h /usr/include/Poco/Config.h
receiver.o: /usr/include/Poco/Platform.h /usr/include/Poco/Platform_POSIX.h
receiver.o: /usr/include/Poco/Bugcheck.h /usr/include/Poco/Types.h
receiver.o: /usr/include/Poco/Net/Socket.h /usr/include/Poco/Net/SocketImpl.h
receiver.o: /usr/include/Poco/Net/SocketDefs.h /usr/include/unistd.h
receiver.o: /usr/include/features.h /usr/include/sys/cdefs.h
receiver.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
receiver.o: /usr/include/gnu/stubs-32.h /usr/include/bits/posix_opt.h
receiver.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
receiver.o: /usr/include/bits/confname.h /usr/include/getopt.h
receiver.o: /usr/include/errno.h /usr/include/bits/errno.h
receiver.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
receiver.o: /usr/include/asm-generic/errno.h
receiver.o: /usr/include/asm-generic/errno-base.h /usr/include/sys/types.h
receiver.o: /usr/include/time.h /usr/include/endian.h
receiver.o: /usr/include/bits/endian.h /usr/include/sys/select.h
receiver.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
receiver.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
receiver.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
receiver.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
receiver.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
receiver.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
receiver.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
receiver.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
receiver.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
receiver.o: /usr/include/sys/ttydefaults.h /usr/include/arpa/inet.h
receiver.o: /usr/include/netinet/in.h /usr/include/stdint.h
receiver.o: /usr/include/bits/wchar.h /usr/include/bits/in.h
receiver.o: /usr/include/bits/byteswap.h /usr/include/netinet/tcp.h
receiver.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
receiver.o: /usr/include/bits/netdb.h /usr/include/net/if.h
receiver.o: /usr/include/Poco/Net/SocketAddress.h
receiver.o: /usr/include/Poco/Net/IPAddress.h
receiver.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/Mutex.h
receiver.o: /usr/include/Poco/Exception.h /usr/include/Poco/ScopedLock.h
receiver.o: /usr/include/Poco/Mutex_POSIX.h /usr/include/pthread.h
receiver.o: /usr/include/sched.h /usr/include/bits/sched.h
receiver.o: /usr/include/signal.h /usr/include/bits/setjmp.h
receiver.o: /usr/include/Poco/Timespan.h /usr/include/Poco/Timestamp.h
receiver.o: /usr/include/Poco/Net/DatagramSocket.h
receiver.o: /usr/include/Poco/Net/SocketStream.h
receiver.o: /usr/include/Poco/BufferedBidirectionalStreamBuf.h
receiver.o: /usr/include/Poco/BufferAllocator.h
receiver.o: /usr/include/Poco/StreamUtil.h /usr/include/Poco/StreamCopier.h
receiver.o: /usr/include/Poco/Path.h ppmesh.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
receiver.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
receiver.o: /usr/include/OpenMesh/Core/System/compiler.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
receiver.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
receiver.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
receiver.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
receiver.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
receiver.o: /usr/include/bits/mathcalls.h
receiver.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
receiver.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
receiver.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
receiver.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
receiver.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
receiver.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
receiver.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
receiver.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
receiver.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
receiver.o: /usr/include/OpenMesh/Core/System/omstream.hh
receiver.o: /usr/include/OpenMesh/Core/System/mostream.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
receiver.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
receiver.o: /usr/include/OpenMesh/Core/Utils/Property.hh
receiver.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
receiver.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
receiver.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
receiver.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
receiver.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
receiver.o: /usr/include/inttypes.h /usr/include/GL/glu.h
receiver.o: /usr/include/Poco/Thread.h /usr/include/Poco/Thread_POSIX.h
receiver.o: /usr/include/Poco/Runnable.h /usr/include/Poco/SignalHandler.h
receiver.o: /usr/include/setjmp.h /usr/include/Poco/Event.h
receiver.o: /usr/include/Poco/Event_POSIX.h /usr/include/Poco/AutoPtr.h
receiver.o: /usr/include/Poco/ThreadPool.h bitstring.hh huffman.hh
receiver.o: vertexid.hh commonType.hh receiver.hh visiblepq.hh
receiver.o: /usr/include/Poco/RunnableAdapter.h gfmesh.hh render.hh
render.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
render.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
render.o: /usr/include/inttypes.h /usr/include/features.h
render.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
render.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
render.o: /usr/include/stdint.h /usr/include/bits/wchar.h
render.o: /usr/include/GL/glu.h /usr/include/Poco/Mutex.h
render.o: /usr/include/Poco/Foundation.h /usr/include/Poco/Config.h
render.o: /usr/include/Poco/Platform.h /usr/include/Poco/Platform_POSIX.h
render.o: /usr/include/Poco/Bugcheck.h /usr/include/Poco/Types.h
render.o: /usr/include/Poco/Exception.h /usr/include/Poco/ScopedLock.h
render.o: /usr/include/Poco/Mutex_POSIX.h /usr/include/pthread.h
render.o: /usr/include/endian.h /usr/include/bits/endian.h
render.o: /usr/include/sched.h /usr/include/bits/types.h
render.o: /usr/include/bits/typesizes.h /usr/include/time.h
render.o: /usr/include/bits/sched.h /usr/include/signal.h
render.o: /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h
render.o: /usr/include/bits/setjmp.h /usr/include/errno.h
render.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
render.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
render.o: /usr/include/asm-generic/errno-base.h render.hh ppmesh.hh
render.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
render.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
render.o: /usr/include/OpenMesh/Core/System/compiler.hh
render.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
render.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
render.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
render.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
render.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
render.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
render.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
render.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
render.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
render.o: /usr/include/bits/mathcalls.h
render.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
render.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
render.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
render.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
render.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
render.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
render.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
render.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
render.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
render.o: /usr/include/OpenMesh/Core/System/omstream.hh
render.o: /usr/include/OpenMesh/Core/System/mostream.hh
render.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
render.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
render.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
render.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
render.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
render.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
render.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
render.o: /usr/include/OpenMesh/Core/Utils/Property.hh
render.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
render.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
render.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
render.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
render.o: /usr/include/Poco/Thread.h /usr/include/Poco/Thread_POSIX.h
render.o: /usr/include/Poco/Runnable.h /usr/include/Poco/SignalHandler.h
render.o: /usr/include/setjmp.h /usr/include/Poco/Event.h
render.o: /usr/include/Poco/Event_POSIX.h
render.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
render.o: /usr/include/sys/select.h /usr/include/bits/select.h
render.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h
render.o: bitstring.hh huffman.hh vertexid.hh commonType.hh visiblepq.hh
render.o: /usr/include/Poco/RunnableAdapter.h gfmesh.hh
server.o: /usr/include/Poco/Net/TCPServer.h /usr/include/Poco/Net/Net.h
server.o: /usr/include/Poco/Foundation.h /usr/include/Poco/Config.h
server.o: /usr/include/Poco/Platform.h /usr/include/Poco/Platform_POSIX.h
server.o: /usr/include/Poco/Bugcheck.h /usr/include/Poco/Types.h
server.o: /usr/include/Poco/Net/ServerSocket.h /usr/include/Poco/Net/Socket.h
server.o: /usr/include/Poco/Net/SocketImpl.h
server.o: /usr/include/Poco/Net/SocketDefs.h /usr/include/unistd.h
server.o: /usr/include/features.h /usr/include/sys/cdefs.h
server.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
server.o: /usr/include/gnu/stubs-32.h /usr/include/bits/posix_opt.h
server.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
server.o: /usr/include/bits/confname.h /usr/include/getopt.h
server.o: /usr/include/errno.h /usr/include/bits/errno.h
server.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
server.o: /usr/include/asm-generic/errno.h
server.o: /usr/include/asm-generic/errno-base.h /usr/include/sys/types.h
server.o: /usr/include/time.h /usr/include/endian.h
server.o: /usr/include/bits/endian.h /usr/include/sys/select.h
server.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
server.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
server.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
server.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
server.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
server.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
server.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
server.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
server.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
server.o: /usr/include/sys/ttydefaults.h /usr/include/arpa/inet.h
server.o: /usr/include/netinet/in.h /usr/include/stdint.h
server.o: /usr/include/bits/wchar.h /usr/include/bits/in.h
server.o: /usr/include/bits/byteswap.h /usr/include/netinet/tcp.h
server.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
server.o: /usr/include/bits/netdb.h /usr/include/net/if.h
server.o: /usr/include/Poco/Net/SocketAddress.h
server.o: /usr/include/Poco/Net/IPAddress.h
server.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/Mutex.h
server.o: /usr/include/Poco/Exception.h /usr/include/Poco/ScopedLock.h
server.o: /usr/include/Poco/Mutex_POSIX.h /usr/include/pthread.h
server.o: /usr/include/sched.h /usr/include/bits/sched.h
server.o: /usr/include/signal.h /usr/include/bits/setjmp.h
server.o: /usr/include/Poco/Timespan.h /usr/include/Poco/Timestamp.h
server.o: /usr/include/Poco/Net/StreamSocket.h /usr/include/Poco/Runnable.h
server.o: /usr/include/Poco/Thread.h /usr/include/Poco/Thread_POSIX.h
server.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
server.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
server.o: /usr/include/Poco/AutoPtr.h /usr/include/Poco/ThreadPool.h
server.o: /usr/include/Poco/Net/TCPServerConnection.h
server.o: /usr/include/Poco/Net/TCPServerConnectionFactory.h
server.o: /usr/include/Poco/Net/TCPServerParams.h
server.o: /usr/include/Poco/Net/DatagramSocket.h
server.o: /usr/include/Poco/Net/SocketStream.h
server.o: /usr/include/Poco/BufferedBidirectionalStreamBuf.h
server.o: /usr/include/Poco/BufferAllocator.h /usr/include/Poco/StreamUtil.h
server.o: /usr/include/Poco/DateTimeFormatter.h
server.o: /usr/include/Poco/DateTimeFormat.h
server.o: /usr/include/Poco/Util/ServerApplication.h
server.o: /usr/include/Poco/Util/Util.h /usr/include/Poco/Util/Application.h
server.o: /usr/include/Poco/Util/Subsystem.h
server.o: /usr/include/Poco/Util/LayeredConfiguration.h
server.o: /usr/include/Poco/Util/AbstractConfiguration.h
server.o: /usr/include/Poco/Util/OptionSet.h /usr/include/Poco/Util/Option.h
server.o: /usr/include/Poco/Util/OptionCallback.h /usr/include/Poco/Logger.h
server.o: /usr/include/Poco/Channel.h /usr/include/Poco/Configurable.h
server.o: /usr/include/Poco/Message.h /usr/include/Poco/Path.h
server.o: /usr/include/Poco/Util/HelpFormatter.h ppmesh.hh
server.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
server.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
server.o: /usr/include/OpenMesh/Core/System/compiler.hh
server.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
server.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
server.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
server.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
server.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
server.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
server.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
server.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
server.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
server.o: /usr/include/bits/mathcalls.h
server.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
server.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
server.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
server.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
server.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
server.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
server.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
server.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
server.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
server.o: /usr/include/OpenMesh/Core/System/omstream.hh
server.o: /usr/include/OpenMesh/Core/System/mostream.hh
server.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
server.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
server.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
server.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
server.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
server.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
server.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
server.o: /usr/include/OpenMesh/Core/Utils/Property.hh
server.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
server.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
server.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
server.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
server.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
server.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
server.o: /usr/include/inttypes.h /usr/include/GL/glu.h bitstring.hh
server.o: huffman.hh vertexid.hh commonType.hh
testsplit.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
testsplit.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
testsplit.o: /usr/include/inttypes.h /usr/include/features.h
testsplit.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
testsplit.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
testsplit.o: /usr/include/stdint.h /usr/include/bits/wchar.h
testsplit.o: /usr/include/GL/glu.h view.hh ppmesh.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
testsplit.o: /usr/include/OpenMesh/Core/System/config.hh
testsplit.o: /usr/include/assert.h
testsplit.o: /usr/include/OpenMesh/Core/System/compiler.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
testsplit.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
testsplit.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
testsplit.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
testsplit.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
testsplit.o: /usr/include/bits/mathcalls.h
testsplit.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
testsplit.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
testsplit.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
testsplit.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
testsplit.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
testsplit.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
testsplit.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
testsplit.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
testsplit.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
testsplit.o: /usr/include/OpenMesh/Core/System/omstream.hh
testsplit.o: /usr/include/OpenMesh/Core/System/mostream.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
testsplit.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
testsplit.o: /usr/include/OpenMesh/Core/Utils/Property.hh
testsplit.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
testsplit.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
testsplit.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
testsplit.o: /usr/include/Poco/Mutex.h /usr/include/Poco/Foundation.h
testsplit.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
testsplit.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
testsplit.o: /usr/include/Poco/Types.h /usr/include/Poco/Exception.h
testsplit.o: /usr/include/Poco/ScopedLock.h /usr/include/Poco/Mutex_POSIX.h
testsplit.o: /usr/include/pthread.h /usr/include/endian.h
testsplit.o: /usr/include/bits/endian.h /usr/include/sched.h
testsplit.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
testsplit.o: /usr/include/time.h /usr/include/bits/sched.h
testsplit.o: /usr/include/signal.h /usr/include/bits/sigset.h
testsplit.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/setjmp.h
testsplit.o: /usr/include/errno.h /usr/include/bits/errno.h
testsplit.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
testsplit.o: /usr/include/asm-generic/errno.h
testsplit.o: /usr/include/asm-generic/errno-base.h /usr/include/Poco/Thread.h
testsplit.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
testsplit.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
testsplit.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
testsplit.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
testsplit.o: /usr/include/sys/select.h /usr/include/bits/select.h
testsplit.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h
testsplit.o: bitstring.hh huffman.hh vertexid.hh commonType.hh gfmesh.hh
testview.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
testview.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
testview.o: /usr/include/inttypes.h /usr/include/features.h
testview.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
testview.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
testview.o: /usr/include/stdint.h /usr/include/bits/wchar.h
testview.o: /usr/include/GL/glu.h view.hh ppmesh.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
testview.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
testview.o: /usr/include/OpenMesh/Core/System/compiler.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
testview.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
testview.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
testview.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
testview.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
testview.o: /usr/include/bits/mathcalls.h
testview.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
testview.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
testview.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
testview.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
testview.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
testview.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
testview.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
testview.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
testview.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
testview.o: /usr/include/OpenMesh/Core/System/omstream.hh
testview.o: /usr/include/OpenMesh/Core/System/mostream.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
testview.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
testview.o: /usr/include/OpenMesh/Core/Utils/Property.hh
testview.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
testview.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
testview.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
testview.o: /usr/include/Poco/Mutex.h /usr/include/Poco/Foundation.h
testview.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
testview.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
testview.o: /usr/include/Poco/Types.h /usr/include/Poco/Exception.h
testview.o: /usr/include/Poco/ScopedLock.h /usr/include/Poco/Mutex_POSIX.h
testview.o: /usr/include/pthread.h /usr/include/endian.h
testview.o: /usr/include/bits/endian.h /usr/include/sched.h
testview.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
testview.o: /usr/include/time.h /usr/include/bits/sched.h
testview.o: /usr/include/signal.h /usr/include/bits/sigset.h
testview.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/setjmp.h
testview.o: /usr/include/errno.h /usr/include/bits/errno.h
testview.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
testview.o: /usr/include/asm-generic/errno.h
testview.o: /usr/include/asm-generic/errno-base.h /usr/include/Poco/Thread.h
testview.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
testview.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
testview.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
testview.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
testview.o: /usr/include/sys/select.h /usr/include/bits/select.h
testview.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h
testview.o: bitstring.hh huffman.hh vertexid.hh commonType.hh gfmesh.hh
vertexid.o: vertexid.hh bitstring.hh
vertexid.sav.o: vertexid.hh bitstring.hh
view.o: /usr/include/GL/glut.h /usr/include/GL/freeglut_std.h
view.o: /usr/include/GL/gl.h /usr/include/GL/glext.h /usr/include/inttypes.h
view.o: /usr/include/features.h /usr/include/sys/cdefs.h
view.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
view.o: /usr/include/gnu/stubs-32.h /usr/include/stdint.h
view.o: /usr/include/bits/wchar.h /usr/include/GL/glu.h view.hh ppmesh.hh
view.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
view.o: /usr/include/OpenMesh/Core/System/config.hh /usr/include/assert.h
view.o: /usr/include/OpenMesh/Core/System/compiler.hh
view.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
view.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
view.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
view.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
view.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
view.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
view.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
view.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
view.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
view.o: /usr/include/bits/mathcalls.h
view.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
view.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
view.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
view.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
view.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
view.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
view.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
view.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
view.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
view.o: /usr/include/OpenMesh/Core/System/omstream.hh
view.o: /usr/include/OpenMesh/Core/System/mostream.hh
view.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
view.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
view.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
view.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
view.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
view.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
view.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
view.o: /usr/include/OpenMesh/Core/Utils/Property.hh
view.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
view.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
view.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
view.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
view.o: /usr/include/Poco/Mutex.h /usr/include/Poco/Foundation.h
view.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
view.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
view.o: /usr/include/Poco/Types.h /usr/include/Poco/Exception.h
view.o: /usr/include/Poco/ScopedLock.h /usr/include/Poco/Mutex_POSIX.h
view.o: /usr/include/pthread.h /usr/include/endian.h
view.o: /usr/include/bits/endian.h /usr/include/sched.h
view.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
view.o: /usr/include/time.h /usr/include/bits/sched.h /usr/include/signal.h
view.o: /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h
view.o: /usr/include/bits/setjmp.h /usr/include/errno.h
view.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
view.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
view.o: /usr/include/asm-generic/errno-base.h /usr/include/Poco/Thread.h
view.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
view.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
view.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
view.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
view.o: /usr/include/sys/select.h /usr/include/bits/select.h
view.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h bitstring.hh
view.o: huffman.hh vertexid.hh commonType.hh gfmesh.hh
visiblepq.o: vertexid.hh bitstring.hh visiblepq.hh /usr/include/GL/glut.h
visiblepq.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
visiblepq.o: /usr/include/GL/glext.h /usr/include/inttypes.h
visiblepq.o: /usr/include/features.h /usr/include/sys/cdefs.h
visiblepq.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
visiblepq.o: /usr/include/gnu/stubs-32.h /usr/include/stdint.h
visiblepq.o: /usr/include/bits/wchar.h /usr/include/GL/glu.h
visiblepq.o: /usr/include/Poco/Thread.h /usr/include/Poco/Foundation.h
visiblepq.o: /usr/include/Poco/Config.h /usr/include/Poco/Platform.h
visiblepq.o: /usr/include/Poco/Platform_POSIX.h /usr/include/Poco/Bugcheck.h
visiblepq.o: /usr/include/Poco/Types.h /usr/include/Poco/Mutex.h
visiblepq.o: /usr/include/Poco/Exception.h /usr/include/Poco/ScopedLock.h
visiblepq.o: /usr/include/Poco/Mutex_POSIX.h /usr/include/pthread.h
visiblepq.o: /usr/include/endian.h /usr/include/bits/endian.h
visiblepq.o: /usr/include/sched.h /usr/include/bits/types.h
visiblepq.o: /usr/include/bits/typesizes.h /usr/include/time.h
visiblepq.o: /usr/include/bits/sched.h /usr/include/signal.h
visiblepq.o: /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h
visiblepq.o: /usr/include/bits/setjmp.h /usr/include/errno.h
visiblepq.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
visiblepq.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
visiblepq.o: /usr/include/asm-generic/errno-base.h
visiblepq.o: /usr/include/Poco/Thread_POSIX.h /usr/include/Poco/Runnable.h
visiblepq.o: /usr/include/Poco/SignalHandler.h /usr/include/setjmp.h
visiblepq.o: /usr/include/Poco/Event.h /usr/include/Poco/Event_POSIX.h
visiblepq.o: /usr/include/Poco/RefCountedObject.h /usr/include/Poco/AutoPtr.h
visiblepq.o: /usr/include/sys/select.h /usr/include/bits/select.h
visiblepq.o: /usr/include/bits/time.h /usr/include/Poco/ThreadPool.h
visiblepq.o: /usr/include/Poco/RunnableAdapter.h ppmesh.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh
visiblepq.o: /usr/include/OpenMesh/Core/System/config.hh
visiblepq.o: /usr/include/assert.h
visiblepq.o: /usr/include/OpenMesh/Core/System/compiler.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/BaseMesh.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/Iterators/IteratorsT.hh
visiblepq.o: /usr/include/OpenMesh/Core/Attributes/Status.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/Iterators/CirculatorsT.hh
visiblepq.o: /usr/include/OpenMesh/Core/Attributes/Attributes.hh
visiblepq.o: /usr/include/OpenMesh/Core/Math/MathDefs.hh /usr/include/math.h
visiblepq.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
visiblepq.o: /usr/include/bits/mathcalls.h
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT.cc
visiblepq.o: /usr/include/OpenMesh/Core/Math/LoopSchemeMaskT.hh
visiblepq.o: /usr/include/OpenMesh/Core/Utils/SingletonT.hh
visiblepq.o: /usr/include/OpenMesh/Core/Utils/SingletonT.cc
visiblepq.o: /usr/include/OpenMesh/Core/Utils/vector_cast.hh
visiblepq.o: /usr/include/OpenMesh/Core/Utils/vector_traits.hh
visiblepq.o: /usr/include/OpenMesh/Core/Utils/GenProg.hh
visiblepq.o: /usr/include/OpenMesh/Core/Math/VectorT.hh
visiblepq.o: /usr/include/OpenMesh/Core/Math/VectorT_inc.hh
visiblepq.o: /usr/include/OpenMesh/Core/System/omstream.hh
visiblepq.o: /usr/include/OpenMesh/Core/System/mostream.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/PolyMeshT_assign.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/TriMeshT.cc
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/Traits.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/Handles.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayKernelT.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/AttribKernelT.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/BaseKernel.hh
visiblepq.o: /usr/include/OpenMesh/Core/Utils/Property.hh
visiblepq.o: /usr/include/OpenMesh/Core/IO/StoreRestore.hh
visiblepq.o: /usr/include/OpenMesh/Core/IO/SR_binary.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/ArrayKernel/ArrayItems.hh
visiblepq.o: /usr/include/OpenMesh/Core/Mesh/Kernels/Common/FinalMeshItemsT.hh
visiblepq.o: huffman.hh commonType.hh gfmesh.hh
