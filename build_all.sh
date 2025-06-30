#!/bin/bash

set -e
set -x

# Inclui as bibliotecas necessárias para áudio
CFLAGS="$(pkg-config --cflags allegro-5 allegro_main-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5)"
LDFLAGS="$(pkg-config --libs allegro-5 allegro_main-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5) -lm"

echo "Limpando executáveis antigos..."
rm -f *.exe *.out bola circles passaro_andante teclado bouncer louco frogger pong passaro_raivoso tela allegro_base invaders

echo "Compilando arquivos .c..."
for src in *.c; do
    obj="${src%.c}.o"
    gcc -c "$src" -o "$obj" $CFLAGS
done

echo "Linkando executáveis..."
for obj in *.o; do
    exe="${obj%.o}"
    gcc "$obj" -o "$exe" $LDFLAGS
done

echo "Removendo arquivos .o temporários..."
rm -f *.o

echo "Build completo."
