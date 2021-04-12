
default: myls.c
	gcc -g -o myls myls.c 


demo: infodemo.c
	gcc -g -o infodemo infodemo.c



clean:
	-rm infodemo
	-rm myls
	-rm *.o

# val: val
# 	valgrind --leak-check=full ./myls -R ~