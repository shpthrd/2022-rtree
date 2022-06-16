#!/bin/bash
current=0
con=0
cont=0
printf "" > output.csv
printf "\t10000\t100000\t1000000\t10000000\n" > indices.csv
for j in $(seq 1 8);
do
	printf "numero de threads %d\n" $j >> output.csv
	printf "t%d\t" $j >> indices.csv
	let current=current+1
	for n in 10000 100000 1000000 10000000;
	do
		printf "%d\tRTREESEARCH3\tRTREESEARCH4\tRTREESEARCH5\n" $n >> output.csv
		let current=current+1
		let con=current+1
		for i in $(seq 1 10);
		do
			./run $n $j>> output.csv
			((current=current+1))

		done
		((cont=current))
		printf "MEDIA\n=MÉDIA(A%d:A%d)\t=MÉDIA(B%d:B%d)\t=MÉDIA(C%d:C%d)\t=MÉDIA(D%d:D%d)\n" $con $cont $con $cont $con $cont $con $cont >> output.csv
		printf "=int.confiança(0.95,desvpad(A%d:A%d),10)\t=int.confiança(0.95,desvpad(B%d:B%d),10)\t=int.confiança(0.95,desvpad(C%d:C%d),10)\t=int.confiança(0.95,desvpad(D%d:D%d),10)\n" $con $cont $con $cont $con $cont $con $cont >> output.csv
		((current=current+2))
		printf "SLOWDOWN\t=B%d/A%d\t=C%d/A%d\t=D%d/A%d\n" $current $current $current $current $current $current >> output.csv
		((current=current+2))
		printf "=D%d\t" $current >> indices.csv
		printf "\n" >> output.csv
		((current=current+1))

	done
	printf "\n" >> indices.csv

done
