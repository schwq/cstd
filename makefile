SOURCE_SO := source/dynamic_list.c source/smart_ptr.c source/exception.c source/cmap.c source/c_string.c source/benchmark.c
SOURCE_MAIN := ${SOURCE_SO} main.c 
OBJS := benchmark.o c_string.o cmap.o exception.o smart_ptr.o dynamic_list.o

all:
	gcc ${SOURCE_MAIN} -o dynamic -w 
debug:
	gcc ${SOURCE_MAIN} -o dynamic -w -g 
shared: 
	gcc -c -w -fpic ${SOURCE_SO} && gcc -shared -o libcstd.so ${OBJS} && make clean
clean:
	rm ${OBJS}
install:
	cp libcstd.so /usr/lib &&  mkdir /usr/include/cstd && cp include/*.h /usr/include/cstd 
uninstall:
	rm /usr/lib/libcstd.so && rm -r /usr/include/cstd 