all:
	gcc source/dynamic_list.c source/c_string.c source/benchmark.c main.c -o dynamic -w 
debug:
	gcc source/dynamic_list.c source/c_string.c source/benchmark.c main.c -o dynamic -w -g 
	
