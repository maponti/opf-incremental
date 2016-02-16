#!/bin/bash
for i in base1 base2 base3 CircleGaussian CTG L ntl parkinsons produce SpamBase2 mpeg7_FOURIER mpeg7_BAS saturn cone-torus skin
do
	mkdir $i

	for (( j=1; j<=10; j++ ))
	do
		cp -f bases/$i.opf $i.dat
		./run_experiment_on_base.sh $i
		cp -rf $i $i$j
	done


	rm *dat*
done
