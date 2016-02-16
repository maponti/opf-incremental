bases = ["yeast1","yeast3","yeast4"]
#bases = ["mpeg7_FOURIER","mpeg7_BAS","cone-torus","saturn"]
#bases = ["CTG", "CircleGaussian", "L", "base1", "base2", "base3", "ntl", "SpamBase2", "parkinsons", "produce"]
iterations = range(1,11)

import statistics

def isNumber(s):
    try:
        float(s)
        return True
    except ValueError:
        return False

for base in bases:
	Iaccuracies = []; #Itimes = []
	RTaccuracies = []; #RTtimes = []
	Daccuracies = []; #Dtimes = []
	IMEAN_accuracies = []; #IMEAN_times = []
	RTMEAN_accuracies = []; #RTMEAN_times = []
	DMEAN_accuracies = []; #DMEAN_times = []
	ISTDEVaccuracies = []; #ISTDEVtimes = []
	RTSTDEVaccuracies = []; #RTSTDEVtimes = []
	DSTDEVaccuracies = []; #DSTDEVtimes = []
	for iteration in iterations:
		accFile = open(base+str(iteration)+"/acc10.txt", "r")
		#timeFile = open(base+str(iteration)+"/time100.txt", "r")

		iterationIaccuracies = []
		iterationRTaccuracies = []
		iterationDaccuracies = []
		#iterationItimes = []
		#iterationRTtimes = []
		#iterationDtimes = []

		# reading accuracies
		readingState = 'X'; readingStates = ['S','I','RT','D']; i =0
		for line in accFile:
			if isNumber(line):
				if readingState is 'S':
					iterationIaccuracies.append(float(line))
					iterationRTaccuracies.append(float(line))
					iterationDaccuracies.append(float(line))
				elif readingState is 'I':
					iterationIaccuracies.append(float(line))
				elif readingState is 'RT':
					iterationRTaccuracies.append(float(line))
				elif readingState is 'D':
					iterationDaccuracies.append(float(line))
			else:
				readingState = readingStates[i]
				i = i + 1

		# reading times
		#readingState = 'X'; readingStates = ['S','I','RT','D']; i =0
		#for line in accFile:
	#		if isNumber(line):
	#			if readingState is 'S':
	#				iterationItimes.append(float(line))
	#				iterationRTtimes.append(float(line))
	#				iterationDtimes.append(float(line))
	#			elif readingState is 'I':
	#				iterationItimes.append(float(line))
	#			elif readingState is 'RT':
	#				iterationRTtimes.append(float(line))
	#			elif readingState is 'D':
	#				iterationDtimes.append(float(line))
	#		else:
	#			readingState = readingStates[i]
	#			i = i + 1
		Iaccuracies.append(iterationIaccuracies)
		RTaccuracies.append(iterationRTaccuracies)
		Daccuracies.append(iterationDaccuracies)
	#	Itimes.append(iterationItimes)
	#	RTtimes.append(iterationRTtimes)
	#	Dtimes.append(iterationDtimes)

	# Read everything, now find means and stddevs
	for line in range(len(Iaccuracies[0])):
		meanIAccList = []; meanRTAccList = []; meanDAccList = []
		#meanITimeList = []; meanRTTimeList = []; meanDTimeList = []
		for i in iterations:
			meanIAccList.append(Iaccuracies[i-1][line])
			meanRTAccList.append(RTaccuracies[i-1][line])
			meanDAccList.append(Daccuracies[i-1][line])
	#		meanITimeList.append(Itimes[i-1][line])
	#		meanRTTimeList.append(RTtimes[i-1][line])
	#		meanDTimeList.append(Dtimes[i-1][line])

		IMEAN_accuracies.append(statistics.mean(meanIAccList))
		ISTDEVaccuracies.append(statistics.stdev(meanIAccList))
		RTMEAN_accuracies.append(statistics.mean(meanRTAccList))
		RTSTDEVaccuracies.append(statistics.stdev(meanRTAccList))
		DMEAN_accuracies.append(statistics.mean(meanDAccList))
		DSTDEVaccuracies.append(statistics.stdev(meanDAccList))

	#	IMEAN_times[i] = statistics.mean(meanITimeList)
	#	ISTDEVtimes[i] = statistics.stdev(meanITimeList)
	#	RTMEAN_times[i] = statistics.mean(meanRTTimeList)
	#	RTSTDEVtimes[i] = statistics.stdev(meanRTTimeList)
	#	DMEAN_times[i] = statistics.mean(meanDTimeList)
	#	DSTDEVtimes[i] = statistics.stdev(meanDTimeList)

	outAccFile = open(base+"accuracy.csv","w+")
	#outTimeFile = open(base+"time.csv","w+")
	outAccFile.write("mean_accI,mean_accRT,mean_accD,stdev_accI,stdev_accRT,stdev_accD\n")
	for i in range(len(IMEAN_accuracies)):
		outAccFile.write(str(IMEAN_accuracies[i])+","+str(RTMEAN_accuracies[i])+","+str(DMEAN_accuracies[i])+","
						+str(ISTDEVaccuracies[i])+","+str(RTSTDEVaccuracies[i])+","+str(DSTDEVaccuracies[i])+"\n")

