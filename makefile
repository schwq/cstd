all:
	gcc source/dynamic_list.c source/smart_ptr.c source/cmap.c source/c_string.c source/benchmark.c main.c -o dynamic -w 
debug:
	gcc source/dynamic_list.c source/smart_ptr.c source/cmap.c source/c_string.c source/benchmark.c main.c -o dynamic -w -g 
	
shared: 
	gcc -shared -o cstdlib.so -fPIC  source/dynamic_list.c source/c_string.c -w -g