CC=gcc
CFLAGS=-O2 -fopenmp -pthread -Wall -Wextra -Werror -Wfatal-errors

DEFINES=-DFORCE_DIRECT_MAPPING -DOMP_NUM_THREADS=$(OMP_NUM_THREADS)
DEFINES_SPINLOCK=-DMP_USE_SPINLOCK
DEFINES_SPREAD=-DMP_USE_SPREAD
DEFINES_SINGLE_BROADCAST=-DMP_USE_SINGLE_BROADCAST
DEFINES_UNUSED_IN_NEIGHBOURS=-DMP_UNUSED_IN_NEIGHBOURS
DEFINES_UNUSED_OUT_NEIGHBOURS_VALUES=-DMP_UNUSED_OUT_NEIGHBOURS_VALUES
DEFINES_UNUSED_OUT_NEIGHBOURS=-DMP_UNUSED_OUT_NEIGHBOURS

SUFFIX_SPINLOCK=_spinlock
SUFFIX_SPREAD=_spread
SUFFIX_SINGLE_BROADCAST=_single_broadcast
SUFFIX_UNUSED_IN_NEIGHBOURS=_unused_in_neighbours
SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES=_unused_out_neighbours_values
SUFFIX_UNUSED_OUT_NEIGHBOURS=_unused_out_neighbours

SRC_DIRECTORY=src
BENCHMARKS_DIRECTORY=benchmarks
BIN_DIRECTORY=bin

default: all

all: verifications \
	 contiguouer \
	 contiguouerASCII \
	 graph_converter \
	 graph_generator \
	 all_hashmin \
	 all_pagerank \
	 all_sssp

verifications:
	@clear
	@echo "Verifications:";
	@if [ -z "${OMP_NUM_THREADS}" ]; then echo "- OMP_NUM_THREADS is not set, please set it to the number of threads usable by OpenMP with 'export OMP_NUM_THREADS=<#threads>'."; exit 1; else echo "- OMP_NUM_THREADS set to '${OMP_NUM_THREADS}'"; fi
	@if [ ! -d "${BIN_DIRECTORY}" ]; then echo "- Bin directory not existing, so it is created."; mkdir ${BIN_DIRECTORY}; else echo "- Bin directory already existing, good."; fi
	@echo ""

contiguouer:
	g++ -o $(BIN_DIRECTORY)/contiguouer $(SRC_DIRECTORY)/contiguouer.cpp -O2 -std=c++11

contiguouerASCII:
	g++ -o $(BIN_DIRECTORY)/contiguouerASCII $(SRC_DIRECTORY)/contiguouerASCII.cpp -O2 -std=c++11

graph_converter:
	g++ -o $(BIN_DIRECTORY)/graph_converter $(SRC_DIRECTORY)/graph_converter.cpp -O2

graph_generator:
	g++ -o $(BIN_DIRECTORY)/graph_generator $(SRC_DIRECTORY)/graph_generator.cpp -O2 -std=c++11

###########
# HASHMIN #
###########
all_hashmin: hashmin \
			 hashmin$(SUFFIX_UNUSED_IN_NEIGHBOURS) \
			 hashmin$(SUFFIX_SPREAD) \
			 hashmin$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS) \
			 hashmin$(SUFFIX_SPINLOCK) \
			 hashmin$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_IN_NEIGHBOURS) \
			 hashmin$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD) \
			 hashmin$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS) \
			 hashmin$(SUFFIX_SINGLE_BROADCAST) \
			 hashmin$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_SPREAD)

hashmin:
	$(CC) -o $(BIN_DIRECTORY)/hashmin $(BENCHMARKS_DIRECTORY)/hashmin.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(CFLAGS)

hashmin$(SUFFIX_UNUSED_IN_NEIGHBOURS):
	$(CC) -o $(BIN_DIRECTORY)/hashmin$(SUFFIX_UNUSED_IN_NEIGHBOURS) $(BENCHMARKS_DIRECTORY)/hashmin.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_UNUSED_IN_NEIGHBOURS) $(CFLAGS)

hashmin$(SUFFIX_SPREAD):
	$(CC) -o $(BIN_DIRECTORY)/hashmin$(SUFFIX_SPREAD) $(BENCHMARKS_DIRECTORY)/hashmin.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPREAD) $(CFLAGS)

hashmin$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS):
	$(CC) -o $(BIN_DIRECTORY)/hashmin$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS) $(BENCHMARKS_DIRECTORY)/hashmin.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPREAD) $(DEFINES_UNUSED_IN_NEIGHBOURS) $(CFLAGS)

hashmin$(SUFFIX_SPINLOCK):
	$(CC) -o $(BIN_DIRECTORY)/hashmin$(SUFFIX_SPINLOCK) $(BENCHMARKS_DIRECTORY)/hashmin.c -I$(SRC_DIRECTORY) -std=gnu99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPINLOCK) $(CFLAGS)

hashmin$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_IN_NEIGHBOURS):
	$(CC) -o $(BIN_DIRECTORY)/hashmin$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_IN_NEIGHBOURS) $(BENCHMARKS_DIRECTORY)/hashmin.c -I$(SRC_DIRECTORY) -std=gnu99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPINLOCK) $(DEFINES_UNUSED_IN_NEIGHBOURS) $(CFLAGS)

hashmin$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD):
	$(CC) -o $(BIN_DIRECTORY)/hashmin$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD) $(BENCHMARKS_DIRECTORY)/hashmin.c -I$(SRC_DIRECTORY) -std=gnu99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPINLOCK) $(DEFINES_SPREAD) $(CFLAGS)

hashmin$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS):
	$(CC) -o $(BIN_DIRECTORY)/hashmin$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS) $(BENCHMARKS_DIRECTORY)/hashmin.c -I$(SRC_DIRECTORY) -std=gnu99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPINLOCK) $(DEFINES_SPREAD) $(DEFINES_UNUSED_IN_NEIGHBOURS) $(CFLAGS)

hashmin$(SUFFIX_SINGLE_BROADCAST):
	$(CC) -o $(BIN_DIRECTORY)/hashmin$(SUFFIX_SINGLE_BROADCAST) $(BENCHMARKS_DIRECTORY)/hashmin.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SINGLE_BROADCAST) $(CFLAGS)

hashmin$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_SPREAD):
	$(CC) -o $(BIN_DIRECTORY)/hashmin$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_SPREAD) $(BENCHMARKS_DIRECTORY)/hashmin.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SINGLE_BROADCAST) $(DEFINES_SPREAD) $(CFLAGS)

############
# PAGERANK #
############
all_pagerank: pagerank \
			 pagerank$(SUFFIX_UNUSED_IN_NEIGHBOURS) \
			 pagerank$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES) \
			 pagerank$(SUFFIX_SPINLOCK) \
			 pagerank$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_IN_NEIGHBOURS) \
			 pagerank$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES) \
			 pagerank$(SUFFIX_SINGLE_BROADCAST) \
			 pagerank$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES)

pagerank:
	$(CC) -o $(BIN_DIRECTORY)/pagerank $(BENCHMARKS_DIRECTORY)/pagerank.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(CFLAGS)

pagerank$(SUFFIX_UNUSED_IN_NEIGHBOURS):
	$(CC) -o $(BIN_DIRECTORY)/pagerank$(SUFFIX_UNUSED_IN_NEIGHBOURS) $(BENCHMARKS_DIRECTORY)/pagerank.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_UNUSED_IN_NEIGHBOURS) $(CFLAGS)

pagerank$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES):
	$(CC) -o $(BIN_DIRECTORY)/pagerank$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES) $(BENCHMARKS_DIRECTORY)/pagerank.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_UNUSED_OUT_NEIGHBOURS_VALUES) $(CFLAGS)

pagerank$(SUFFIX_SPINLOCK):
	$(CC) -o $(BIN_DIRECTORY)/pagerank$(SUFFIX_SPINLOCK) $(BENCHMARKS_DIRECTORY)/pagerank.c -I$(SRC_DIRECTORY) -std=gnu99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPINLOCK) $(CFLAGS)

pagerank$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_IN_NEIGHBOURS):
	$(CC) -o $(BIN_DIRECTORY)/pagerank$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_IN_NEIGHBOURS) $(BENCHMARKS_DIRECTORY)/pagerank.c -I$(SRC_DIRECTORY) -std=gnu99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPINLOCK) $(DEFINES_UNUSED_IN_NEIGHBOURS) $(CFLAGS)

pagerank$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES):
	$(CC) -o $(BIN_DIRECTORY)/pagerank$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES) $(BENCHMARKS_DIRECTORY)/pagerank.c -I$(SRC_DIRECTORY) -std=gnu99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPINLOCK) $(DEFINES_UNUSED_OUT_NEIGHBOURS_VALUES) $(CFLAGS)

pagerank$(SUFFIX_SINGLE_BROADCAST):
	$(CC) -o $(BIN_DIRECTORY)/pagerank$(SUFFIX_SINGLE_BROADCAST) $(BENCHMARKS_DIRECTORY)/pagerank.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SINGLE_BROADCAST) $(CFLAGS)

pagerank$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES):
	$(CC) -o $(BIN_DIRECTORY)/pagerank$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES) $(BENCHMARKS_DIRECTORY)/pagerank.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SINGLE_BROADCAST) $(DEFINES_UNUSED_OUT_NEIGHBOURS_VALUES) $(CFLAGS)

########
# SSSP #
########
all_sssp: sssp \
		 sssp$(SUFFIX_UNUSED_IN_NEIGHBOURS) \
		 sssp$(SUFFIX_SPREAD) \
		 sssp$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS) \
		 sssp$(SUFFIX_SPINLOCK) \
		 sssp$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_IN_NEIGHBOURS) \
		 sssp$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD) \
		 sssp$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS) \
		 sssp$(SUFFIX_SINGLE_BROADCAST) \
		 sssp$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES) \
		 sssp$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_SPREAD)

sssp:
	$(CC) -o $(BIN_DIRECTORY)/sssp $(BENCHMARKS_DIRECTORY)/sssp.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(CFLAGS)

sssp$(SUFFIX_UNUSED_IN_NEIGHBOURS):
	$(CC) -o $(BIN_DIRECTORY)/sssp$(SUFFIX_UNUSED_IN_NEIGHBOURS) $(BENCHMARKS_DIRECTORY)/sssp.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_UNUSED_IN_NEIGHBOURS) $(CFLAGS)

sssp$(SUFFIX_SPREAD):
	$(CC) -o $(BIN_DIRECTORY)/sssp$(SUFFIX_SPREAD) $(BENCHMARKS_DIRECTORY)/sssp.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPREAD) $(CFLAGS)

sssp$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS):
	$(CC) -o $(BIN_DIRECTORY)/sssp$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS) $(BENCHMARKS_DIRECTORY)/sssp.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPREAD) $(DEFINES_UNUSED_IN_NEIGHBOURS) $(CFLAGS)

sssp$(SUFFIX_SPINLOCK):
	$(CC) -o $(BIN_DIRECTORY)/sssp$(SUFFIX_SPINLOCK) $(BENCHMARKS_DIRECTORY)/sssp.c -I$(SRC_DIRECTORY) -std=gnu99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPINLOCK) $(CFLAGS)

sssp$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_IN_NEIGHBOURS):
	$(CC) -o $(BIN_DIRECTORY)/sssp$(SUFFIX_SPINLOCK)$(SUFFIX_UNUSED_IN_NEIGHBOURS) $(BENCHMARKS_DIRECTORY)/sssp.c -I$(SRC_DIRECTORY) -std=gnu99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPINLOCK) $(DEFINES_UNUSED_IN_NEIGHBOURS) $(CFLAGS)

sssp$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD):
	$(CC) -o $(BIN_DIRECTORY)/sssp$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD) $(BENCHMARKS_DIRECTORY)/sssp.c -I$(SRC_DIRECTORY) -std=gnu99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPINLOCK) $(DEFINES_SPREAD) $(CFLAGS)

sssp$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS):
	$(CC) -o $(BIN_DIRECTORY)/sssp$(SUFFIX_SPINLOCK)$(SUFFIX_SPREAD)$(SUFFIX_UNUSED_IN_NEIGHBOURS) $(BENCHMARKS_DIRECTORY)/sssp.c -I$(SRC_DIRECTORY) -std=gnu99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPINLOCK) $(DEFINES_SPREAD) $(DEFINES_UNUSED_IN_NEIGHBOURS) $(CFLAGS)

sssp$(SUFFIX_SINGLE_BROADCAST):
	$(CC) -o $(BIN_DIRECTORY)/sssp$(SUFFIX_SINGLE_BROADCAST) $(BENCHMARKS_DIRECTORY)/sssp.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SINGLE_BROADCAST) $(CFLAGS)

sssp$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES):
	$(CC) -o $(BIN_DIRECTORY)/sssp$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_UNUSED_OUT_NEIGHBOURS_VALUES) $(BENCHMARKS_DIRECTORY)/sssp.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SINGLE_BROADCAST) $(DEFINES_UNUSED_OUT_NEIGHBOURS_VALUES) $(CFLAGS)

sssp$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_SPREAD):
	$(CC) -o $(BIN_DIRECTORY)/sssp$(SUFFIX_SINGLE_BROADCAST)$(SUFFIX_SPREAD) $(BENCHMARKS_DIRECTORY)/sssp.c -I$(SRC_DIRECTORY) -std=c99 $(DEFINES) $(DEFINES_COMBINER) $(DEFINES_SPREAD) $(DEFINES_SINGLE_BROADCAST) $(CFLAGS)

#########
# CLEAN #
#########
clean:
	rm -rf $(BIN_DIRECTORY)
