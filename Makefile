path := src/config/linux/

all : clipboad_handler pycopybox

clipboad_handler : $(path)cb.c
	gcc $(path)cb.c -o clipboard_handler -lX11

pycopybox : $(path)X11_events.c
	gcc -I$(path) $(path)X11_events.c -o pycopybox -lX11 -lXtst -lXfixes
	
clean:
	rm -f clipboard_handler pycopybox