# this script requires an argument, the base name (i.e. if running tests for 'base1.dat', the base name is 'base1')
echo "Simulating experiment for $1"

cp -f $1.opf $1.dat

# outer loop: splits the base in 'i' segments ('i' being 10, 100, 500, 1000, 5000)
for i in 100
do
	# Cleaning the split and test files from last experiment
	rm -f $1I* $1T* test*
	rm -f *time *diversity *acc
	
	echo "Begin test: $i segments"
	echo "	Splitting..."
	./opf_incremental_split $1.dat 0.5 0.5 0.1 $i 1
	echo "		Split ok."

	echo "	Initial training on I_0000.dat..."
	{
		./opf_train $1I_0000.dat
		cp $1T.dat testI_0000.dat
		./opf_classify testI_0000.dat
		./opf_accuracy testI_0000.dat
	} &> log.txt
	echo "		Initial train ok."

	cp classifier.opf initial_classifier.opf

	# inner loop: iterates through all of i segments
	for (( j=1; j<$i; j++ ))
	do
		echo "	Currently inc_training on I_$(printf "%04d" $j)"
		{	
			cp classifier.opf classifier_last.opf
			./opf_includenew $1I_$(printf "%04d" $j).dat $1incremented.dat
			cp $1T.dat testI_$(printf "%04d" $j).dat
			cp $1incremented.dat $1$(printf "%04d" $j)incremented.dat
			#./opf_classify testI_$(printf "%04d" $j).dat
			./opf_classify_diversity testI_$(printf "%04d" $j).dat classifier.opf classifier_last.opf 1
			mv testI_$(printf "%04d" $j).dat2.out testI_$(printf "%04d" $j).dat.out
			./opf_accuracy testI_$(printf "%04d" $j).dat

		} &> log.txt
	done

	cp initial_classifier.opf classifier.opf

	for (( j=1; j<$i; j++ ))
	do
		echo "	Currently retraining on I_$(printf "%04d" $j)"
		{	
			cp classifier.opf classifier_last.opf
			./opf_train $1$(printf "%04d" $j)incremented.dat
			cp $1T.dat testRT_I_$(printf "%04d" $j).dat
			#./opf_classify testRT_I_$(printf "%04d" $j).dat
			./opf_classify_diversity testRT_I_$(printf "%04d" $j).dat classifier.opf classifier_last.opf 1
			mv testRT_I_$(printf "%04d" $j).dat2.out testRT_I_$(printf "%04d" $j).dat.out
			./opf_accuracy testRT_I_$(printf "%04d" $j).dat
		} &> log.txt
	done

	cp initial_classifier.opf classifier.opf

	for (( j=1; j<$i; j++ ))
	do
		echo "	Currently dift_training on I_$(printf "%04d" $j)"
		{	
			cp classifier.opf classifier_last.opf
			cp $1I_$(printf "%04d" $j).dat $1D_$(printf "%04d" $j).dat
			./opf_includedift $1D_$(printf "%04d" $j).dat $1incremented.dat
			cp $1T.dat testD_$(printf "%04d" $j).dat
			cp $1incremented.dat $1$(printf "%04d" $j)incremented.dat
			#./opf_classify testD_$(printf "%04d" $j).dat
			./opf_classify_diversity testD_$(printf "%04d" $j).dat classifier.opf classifier_last.opf 1
			mv testD_$(printf "%04d" $j).dat2.out testD_$(printf "%04d" $j).dat.out
			./opf_accuracy testD_$(printf "%04d" $j).dat

		} &> log.txt
	done


	rm classifier_last.opf initial_classifier.opf

	sleep 0.2

	echo "Outputting acc to acc$i file..."
	{
		echo "Balanced Accuracy"
		cat testI_0000.dat.acc

		echo "Accuracy - I"
		for (( j=1; j<$i; j++ ))
		do
			cat testI_$(printf "%04d" $j).dat.acc
		done

		echo "Accuracy - RT"
		for (( j=1; j<$i; j++ ))
		do
			cat testRT_I_$(printf "%04d" $j).dat.acc
		done

		echo "Accuracy - D"
		for (( j=1; j<$i; j++ ))
		do
			cat testD_$(printf "%04d" $j).dat.acc
		done


	} &> $1/acc$i.txt


	echo "Outputting time to time$i file..."
	{
		echo "Time (seconds)"
		cat $1I_0000.dat.time

		echo "Time - I"
		for (( j=1; j<$i; j++ ))
		do
			cat $1I_$(printf "%04d" $j).dat.time
		done

		echo "Time - RT"
		for (( j=1; j<$i; j++ ))
		do
			cat $1$(printf "%04d" $j)incremented.dat.time
		done

		echo "Time - D"
		for (( j=1; j<$i; j++ ))
		do
			cat $1D_$(printf "%04d" $j).dat.time
		done
	} &> $1/time$i.txt


	echo "Outputting diversity to diversity$i file..."
	{
		echo "Diversity - I"
		for (( j=1; j<$i; j++ ))
		do
			cat testI_$(printf "%04d" $j).dat.diversity
		done
		
		echo "Diversity - RT"
		for (( j=1; j<$i; j++ ))
		do
			cat testRT_I_$(printf "%04d" $j).dat.diversity
		done

		echo "Diversity - D"
		for (( j=1; j<$i; j++ ))
		do
			cat testD_$(printf "%04d" $j).dat.diversity
		done
		
	} &> $1/diversity$i.txt
done