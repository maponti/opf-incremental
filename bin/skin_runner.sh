echo "Simulating experiment for skin"

cp -f skin.opf skin.dat

# outer loop: splits the base in 'i' segments ('i' being 10, 100, 500, 1000, 5000)
for i in 100000
do
	# Cleaning the split and test files from last experiment
	rm skinI* skinT* test*
	
	echo "Begin test: $i segments"
	echo "	Splitting..."
	./opf_incremental_split skin.dat 0.92 0.08 0.1 $i 1
	echo "		Split ok."

	echo "	Initial training on I_0000.dat..."
	{
		./opf_train skinI_0000.dat
		cp skinT.dat testI_0000.dat
		./opf_classify testI_0000.dat
		./opf_accuracy testI_0000.dat
	} &> /dev/null
	echo "		Initial train ok."

	# inner loop: iterates through all of i segments
	for (( j=1; j<$i; j++ ))
	do
		echo "	Currently inc_training on I_$(printf "%07d" $j)"
		{	
			cp classifier.opf classifier_last.opf
			./opf_includenew skinI_$(printf "%07d" $j).dat skinincremented.dat
			cp skinT.dat testI_$(printf "%07d" $j).dat
			cp skinincremented.dat skin$(printf "%07d" $j)incremented.dat
			#./opf_classify testI_$(printf "%07d" $j).dat
			./opf_classify_diversity testI_$(printf "%07d" $j).dat classifier.opf classifier_last.opf 1
			mv testI_$(printf "%07d" $j).dat2.out testI_$(printf "%07d" $j).dat.out
			./opf_accuracy testI_$(printf "%07d" $j).dat

		} &> /dev/null
	done

	for (( j=1; j<$i; j++ ))
	do
		echo "	Currently retraining on I_$(printf "%07d" $j)"
		{	
			cp classifier.opf classifier_last.opf
			./opf_train skin$(printf "%07d" $j)incremented.dat
			cp skinT.dat testRT_I_$(printf "%07d" $j).dat
			#./opf_classify testRT_I_$(printf "%07d" $j).dat
			./opf_classify_diversity testRT_I_$(printf "%07d" $j).dat classifier.opf classifier_last.opf 1
			mv testRT_I_$(printf "%07d" $j).dat2.out testRT_I_$(printf "%07d" $j).dat.out
			./opf_accuracy testRT_I_$(printf "%07d" $j).dat
		} &> /dev/null
	done

	rm classifier_last.dat

	sleep 0.2

	echo "Outputting acc to acc$i file..."
	{
		echo "Balanced Accuracy"
		cat testI_0000.dat.acc

		echo "Accuracy - I"
		for (( j=1; j<$i; j++ ))
		do
			cat testI_$(printf "%07d" $j).dat.acc
		done

		echo "Accuracy - RT"
		for (( j=1; j<$i; j++ ))
		do
			cat testRT_I_$(printf "%07d" $j).dat.acc
		done


	} &> skin/acc$i.txt


	echo "Outputting time to time$i file..."
	{
		echo "Time (seconds)"
		cat skinI_0000.dat.time

		echo "Time - I"
		for (( j=1; j<$i; j++ ))
		do
			cat skinI_$(printf "%07d" $j).dat.time
		done

		echo "Time - RT"
		for (( j=1; j<$i; j++ ))
		do
			cat skin$(printf "%07d" $j)incremented.dat.time
		done
	} &> skin/time$i.txt


	echo "Outputting diversity to diversity$i file..."
	{
		echo "Diversity - I"
		for (( j=1; j<$i; j++ ))
		do
			cat testI_$(printf "%07d" $j).dat.diversity
		done
		
		echo "Diversity - RT"
		for (( j=1; j<$i; j++ ))
		do
			cat testRT_I_$(printf "%07d" $j).dat.diversity
		done
		
	} &> skin/diversity$i.txt
done
