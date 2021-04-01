
default: myls.c
	gcc -g -o myls myls.c 


demo: infodemo.c
	gcc -g -o infodemo infodemo.c



clean:
	-rm infoDemo
	-rm myls
	-rm *.o