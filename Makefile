LIB=./lib
INCLUDE=./include
SRC=./src
OBJ=./obj
UTIL=util

CC=gcc

FLAGS=  -O3 -g -Wall


INCFLAGS = -I$(INCLUDE) -I$(INCLUDE)/$(UTIL)

all: libOPF opf_split opf_accuracy_with_plot opf_classify_diversity opf_train_and_classify opf_incremental_split opf_accuracy opf_train opf_classify opf_classifyscore opf_learn opf_distance opf_info opf_fold opf_merge opf_cluster opf_knn_classify bayes statistics txt2opf base_creator result_organizer opf_includenew opf_includedift

libOPF: libOPF-build
	echo "libOPF.a built..."

libOPF-build: \
util \
$(OBJ)/OPF.o \
$(OBJ)/OPFincremental.o \

	ar csr $(LIB)/libOPF.a \
$(OBJ)/common.o \
$(OBJ)/set.o \
$(OBJ)/diversity.o \
$(OBJ)/gqueue.o \
$(OBJ)/realheap.o \
$(OBJ)/sgctree.o \
$(OBJ)/subgraph.o \
$(OBJ)/OPF.o \
$(OBJ)/OPFincremental.o \

$(OBJ)/OPF.o: $(SRC)/OPF.c
	$(CC) $(FLAGS) -g -c $(SRC)/OPF.c $(INCFLAGS) \
	-o $(OBJ)/OPF.o

$(OBJ)/OPFincremental.o: $(SRC)/OPFincremental.c
	$(CC) $(FLAGS) -Wall -g -c $(SRC)/OPFincremental.c $(INCFLAGS) \
	-o $(OBJ)/OPFincremental.o

opf_split: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_split.c  -L./lib -o bin/opf_split -lOPF -lm

opf_accuracy_with_plot: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_accuracy_with_plot.c  -L./lib -o bin/opf_accuracy_with_plot -lOPF -lm

opf_classify_diversity: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_classify_diversity.c  -L./lib -o bin/opf_classify_diversity -lOPF -lm

opf_train_and_classify: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_train_and_classify.c  -L./lib -o bin/opf_train_and_classify -lOPF -lm

opf_incremental_split: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_incremental_split.c  -L./lib -o bin/opf_incremental_split -lOPF -lm

opf_accuracy: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_accuracy.c  -L./lib -o bin/opf_accuracy -lOPF -lm

opf_train: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_train.c  -L./lib -o bin/opf_train -lOPF -lm

opf_classify: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_classify.c  -L./lib -o bin/opf_classify -lOPF -lm

opf_classifyscore: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_classifyscore.c  -L./lib -o bin/opf_classifyscore -lOPF -lm

opf_learn: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_learn.c  -L./lib -o bin/opf_learn -lOPF -lm

opf_distance: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_distance.c  -L./lib -o bin/opf_distance -lOPF -lm

opf_info: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_info.c  -L./lib -o bin/opf_info -lOPF -lm

opf_fold: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_fold.c  -L./lib -o bin/opf_fold -lOPF -lm

opf_merge: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_merge.c  -L./lib -o bin/opf_merge -lOPF -lm

opf_cluster: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_cluster.c  -L./lib -o bin/opf_cluster -lOPF -lm

opf_knn_classify: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) src/opf_knn_classify.c  -L./lib -o bin/opf_knn_classify -lOPF -lm

opf_includenew: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) -g src/opf_includenew.c -L./lib -o bin/opf_includenew -lOPF -lm

opf_includedift: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) -g src/opf_includedift.c -L./lib -o bin/opf_includedift -lOPF -lm

bayes:
	$(CC) $(FLAGS) $(INCFLAGS) src/bayes.c  -L./lib -o bin/bayes -lOPF -lm

statistics:
	$(CC) $(FLAGS) tools/src/statistics.c  -o tools/statistics -lm

txt2opf: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) tools/src/txt2opf.c  -L./lib -o tools/txt2opf -lOPF -lm

opf2txt: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) tools/src/opf2txt.c  -L./lib -o tools/opf2txt -lOPF -lm

result_organizer: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) tools/src/result_organizer.c  -L./lib -o tools/result_organizer -lOPF -lm

base_creator: libOPF
	$(CC) $(FLAGS) $(INCFLAGS) tools/src/base_creator.c  -L./lib -o tools/base_creator -lOPF -lm

util: $(SRC)/$(UTIL)/common.c $(SRC)/$(UTIL)/set.c $(SRC)/$(UTIL)/diversity.c $(SRC)/$(UTIL)/gqueue.c $(SRC)/$(UTIL)/realheap.c $(SRC)/$(UTIL)/sgctree.c $(SRC)/$(UTIL)/subgraph.c
	$(CC) $(FLAGS) $(INCFLAGS) -c $(SRC)/$(UTIL)/common.c -o $(OBJ)/common.o
	$(CC) $(FLAGS) $(INCFLAGS) -c $(SRC)/$(UTIL)/set.c -o $(OBJ)/set.o
	$(CC) $(FLAGS) $(INCFLAGS) -c $(SRC)/$(UTIL)/diversity.c -o $(OBJ)/diversity.o
	$(CC) $(FLAGS) $(INCFLAGS) -c $(SRC)/$(UTIL)/gqueue.c -o $(OBJ)/gqueue.o
	$(CC) $(FLAGS) $(INCFLAGS) -c $(SRC)/$(UTIL)/realheap.c -o $(OBJ)/realheap.o
	$(CC) $(FLAGS) $(INCFLAGS) -c $(SRC)/$(UTIL)/sgctree.c -o $(OBJ)/sgctree.o
	$(CC) $(FLAGS) $(INCFLAGS) -c $(SRC)/$(UTIL)/subgraph.c -o $(OBJ)/subgraph.o


## Compiling LibOPF with LibIFT

opf-ift: libOPF-ift

libOPF-ift: libOPF-ift-build
	echo "libOPF.a built with IFT..."

libOPF-ift-build: \
OPF-ift.o \

	ar csr $(LIB)/libOPF.a \
$(OBJ)/OPF.o \

OPF-ift.o: $(SRC)/OPF.c
	$(CC) $(FLAGS) -c $(SRC)/OPF.c -I$(INCLUDE) -I$(IFT_DIR)/include \
	-o $(OBJ)/OPF.o

## Cleaning-up

clean:
	rm -f $(LIB)/lib*.a; rm -f $(OBJ)/*.o bin/opf_split bin/opf_incremental_split bin/opf_accuracy bin/opf_includedift bin/opf_train bin/opf_classify bin/opf_learn bin/opf_distance bin/opf_info bin/opf_cluster bin/opf_fold bin/opf_merge bin/opf_knn_classify tools/statistics tools/txt2opf bin/opf_includenew bin/opf_classifyscore

clean_results:
	rm -f *.out *.opf *.acc *.time *.opf training.dat evaluating.dat testing.dat

clean_results_in_examples:
	rm -f examples/*.out examples/*.opf examples/*.acc examples/*.time examples/*.opf examples/training.dat examples/evaluating.dat examples/testing.dat


