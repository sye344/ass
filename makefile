default:
	gcc demo1.c -o demo1
	./demo1
	gnuplot gnucode.txt
clear:
	rm -rf demo1 histogram.pdf