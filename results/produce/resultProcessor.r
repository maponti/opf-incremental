processResults <- function(filename, rangebegin=0, rangeend) {
	dataset = t(read.csv(filename, header=F))

	dev.new()
	plot(dataset[,1], type="l", main="Acuracia", col=2, ylab="Porcentagem", xlab="Iteracoes")
	lines(dataset[,2], type="l", col=3)

	dev.new()
	plot(dataset[,3], type="l", main="Tempo", col=2, ylab="Segundos", xlab="Iteracoes")
	lines(dataset[,4], type="l", col=3)

	dev.new()
	if (missing(rangeend)) orgcumsum = cumsum(dataset[,3]) else orgcumsum = cumsum(dataset[rangebegin:rangeend,3])
	if (missing(rangeend)) inccumsum = cumsum(dataset[,4]) else inccumsum = cumsum(dataset[rangebegin:rangeend,4])
	plot(orgcumsum, type="l", main="Tempo Acumulado", col=2, ylab="Segundos", xlab="Iteracoes")
	lines(inccumsum, type="l", col=3)
}