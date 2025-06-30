ALLEGRO_VERSION=5.2.10.1
MINGW_VERSION=4.7.0
FOLDER=/mnt/c

FOLDER_NAME=/allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=/lib/liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=/include

all: bola.exe circles.exe passaro_andante.exe teclado.exe bouncer.exe louco.exe frogger.exe pong.exe passaro_raivoso.exe tela.exe allegro_base.exe

bola.exe: bola.o
	gcc -o bola.exe bola.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

bola.o: bola.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c bola.c		

circles.exe: circles.o
	gcc -o circles.exe circles.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

circles.o: circles.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c circles.c		

passaro_andante.exe: passaro_andante.o
	gcc -o passaro_andante.exe passaro_andante.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

passaro_andante.o: passaro_andante.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c passaro_andante.c

passaro_raivoso.exe: passaro_raivoso.o
	gcc -o passaro_raivoso.exe passaro_raivoso.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

passaro_raivoso.o: passaro_raivoso.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c passaro_raivoso.c	

bouncer.exe: bouncer.o
	gcc -o bouncer.exe bouncer.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

bouncer.o: bouncer.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c bouncer.c	

teclado.exe: teclado.o
	gcc -o teclado.exe teclado.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

teclado.o: teclado.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c teclado.c	

louco.exe: louco.o
	gcc -o louco.exe louco.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

louco.o: louco.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c louco.c		

frogger.exe: frogger.o
	gcc -o frogger.exe frogger.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

frogger.o: frogger.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c frogger.c		

pong.exe: pong.o
	gcc -o pong.exe pong.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

pong.o: pong.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c pong.c	

tela.exe: tela.o
	gcc -o tela.exe tela.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

tela.o: tela.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c tela.c	

allegro_base.exe: allegro_base.o
	gcc -o allegro_base.exe allegro_base.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

allegro_base.o: allegro_base.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c allegro_base.c	

clean:
	rm -f bola.o bola.exe
	rm -f circles.o circles.exe
	rm -f passaro_andante.o passaro_andante.exe
	rm -f passaro_raivoso.o passaro_raivoso.exe
	rm -f louco.o louco.exe
	rm -f teclado.o teclado.exe
	rm -f bouncer.o bouncer.exe
	rm -f frogger.o frogger.exe
	rm -f pong.o pong.exe
	rm -f tela.o tela.exe
	rm -f allegro_base.o allegro_base.exe
