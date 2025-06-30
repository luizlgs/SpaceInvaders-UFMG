#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>

//dimensoes da tela
const int SCREEN_W = 960;
const int SCREEN_H = 540;

//altura da grama
const int GRASS_H = 60;

//dimensoes da nave
const int NAVE_H = 50;
const int NAVE_w = 50;

//FPS
const float FPS = 100; 

//dimensoes do alienigena
const int ALIEN_W = 50;
const int ALIEN_H = 22;
//posicao x e y inicial dos alienigenas, pra eles nao iniciarem no canto da tela
const int INTIAL_X = 25; 
const int INTIAL_Y = 10;

//dimensoes tiro
const int TIRO_W = 2;
const int TIRO_H = 15;

typedef struct {
    float x;
    float velocidade;
    int dir, esq;
    ALLEGRO_COLOR cor;
} Nave;

void initNave(Nave *nave){
    nave->x = SCREEN_W/2;
    nave->velocidade = 3;
    nave->dir = 0;
    nave->esq = 0;
    nave->cor = al_map_rgb(0, 0, 255);
}

void draw_nave(Nave nave){
    float y_base = SCREEN_H - GRASS_H/2;
    al_draw_filled_triangle(nave.x, y_base - NAVE_H, 
                            nave.x - NAVE_w/2, y_base,
                            nave.x + NAVE_w/2, y_base, 
                            nave.cor);
}

void update_nave(Nave *nave){
    if(nave->dir && nave->x + nave->velocidade <= SCREEN_W){
        nave->x += nave->velocidade;
    }
    if(nave->esq && nave->x + nave->velocidade >= 0){
        nave->x -= nave->velocidade;
    }
}

typedef struct {
    float x, y;
    float velocidade;
    int dir, esq, cima;
    float velocidadeCima;
    ALLEGRO_COLOR cor;
    int voltar;
} Tiro;

void initTiro(Tiro *tiro){
    tiro->x = SCREEN_W/2;
    tiro->y = SCREEN_H - GRASS_H/2 - NAVE_H - TIRO_H + 40;
    tiro->velocidade = 3;
    tiro->velocidadeCima = 0;
    tiro->dir = 0;
    tiro->esq = 0;
    tiro->cima = 0;
    tiro->cor = al_map_rgb(255, 255, 255);
    tiro->voltar = 0;
}

void draw_tiro(Tiro tiro){
    float y_base = SCREEN_H - GRASS_H/2;
    al_draw_filled_rectangle(tiro.x - TIRO_W, tiro.y - TIRO_H ,
                             tiro.x + TIRO_W, tiro.y + TIRO_H , 
                             tiro.cor);
}



void update_tiro(Tiro *tiro, Nave nave){
    if(!(tiro->cima)){
        tiro->x = nave.x;
        return;
    }
    else{
        tiro->y -= tiro->velocidadeCima;
        if(tiro->y < 0 || tiro->voltar){ //volta o tiro para a nave se passar dos limites superiores da tela ou atingir um alien
            tiro->cima = 0;
            tiro->velocidadeCima = 0;
            tiro->x = nave.x;
            tiro->y = SCREEN_H - GRASS_H/2 - NAVE_H - TIRO_H + 40;
            tiro->voltar = 0;
        }

        return;
    }

}

void draw_scenario(){
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

void draw_grass(){
    al_draw_filled_rectangle(0, SCREEN_H-GRASS_H, 
                             SCREEN_W, SCREEN_H,
                             al_map_rgb(0, 255, 0));
}

typedef struct {
   float x, y;
   float x_velocidade, y_velocidade; 
   ALLEGRO_COLOR cor;
   int hitbox;
} Alien;

void initAlien(Alien *alien, float x, float y){
    alien->x = x;
    alien->y = y;
    alien->x_velocidade = 2.4;
    alien->y_velocidade = ALIEN_H;
    alien->cor = al_map_rgb(rand()%256, rand()%256, rand()%256);
    alien->hitbox = 1;
}

void draw_alien(Alien alien){
    al_draw_filled_rectangle(alien.x, alien.y, 
                             alien.x+ALIEN_W, alien.y+ALIEN_H,
                             alien.cor);
}

void update_alien(Alien alien[4][5], Tiro *tiro, Nave nave, int *score){
    int x1, y1;
    int x2, y2;
    x1 = y1 = x2 = y2 = -1;

    //verifica os aliens que ainda esta vivos na extrema esquerda
    for(int j=0; j<5; j++){
        for(int i=0; i<4 && x1==-1; i++){
            if(alien[i][j].hitbox){
                x1 = i;
                y1 = j;
                break;
            }
        }
    }


    //verifica os aliens que ainda esta vivos na extrema direita
    for(int j=4; j>=0; j--){
        for(int i=0; i<5 && x2==-1; i++){
            if(alien[i][j].hitbox){
                x2 = i;
                y2 = j;
                break;
            }
        }
    }

    //muda a posicao de todos os aliens
    for(int i=0; i<4; i++){
        for(int j=0; j<5; j++){
            alien[i][j].x += alien[i][j].x_velocidade;
        }
    }
    
    //muda a direcao dos aliens se atingirem a borda direita da tela
    if(alien[x2][y2].x + ALIEN_W > SCREEN_W){
        for(int i=0; i<4; i++){
            for(int j=0; j<5; j++){
                alien[i][j].y += alien[i][j].y_velocidade;
                alien[i][j].x_velocidade *= -1;
            }
        }
    }
    
    //muda a direcao dos aliens se atingirem a borda esquerda da tela
    else if(alien[x1][y1].x <= 0 && alien[x1][y1].y !=0){
        for(int i=0; i<4; i++){
            for(int j=0; j<5; j++){
                    alien[i][j].y += alien[i][j].y_velocidade;
                    alien[i][j].x_velocidade *= -1;
            }
        }
    }

    //destroi a nave do alien caso seja atingido pelo tiro e ela ainda esteja ativa
    for(int i=0; i<4; i++){
        for(int j=0; j<5; j++){
            if((tiro->x > alien[i][j].x && tiro->x < alien[i][j].x+ALIEN_W) && (tiro->y > alien[i][j].y - ALIEN_H && tiro->y < alien[i][j].y + ALIEN_H) && alien[i][j].hitbox){
                alien[i][j].cor = al_map_rgb(0, 0, 0);
                alien[i][j].hitbox = 0;
                tiro->voltar = 1;
                *score += 10;
            }
        }
    }
}
    

int colisao_alien_solo(Alien *alien){
    if(alien->y + ALIEN_H >= SCREEN_H - GRASS_H && alien->hitbox){
        return 1;
    }
    return 0;
} 

int colisao_alien_nave(Alien *alien, Nave *nave){
    if((alien->y + ALIEN_H > SCREEN_H - GRASS_H/2 - NAVE_H) && ((alien->x+ALIEN_W > nave->x  && alien->x+ALIEN_W < nave->x+NAVE_w/2) || (alien->x < nave->x && alien->x+ALIEN_W > nave->x)) && alien->hitbox){
        return 1;
    }
    return 0;
}

void encerrar_sessao(int score, int score_maximo){
    if(score >= score_maximo){
        FILE *file = fopen("recorde.txt", "w");
        fprintf(file, "%d\n", score);
        fclose(file);
        printf("\nNovo recorde: %d\n", score);
    }
    exit(0);
}


int main(){
    srand(time(NULL));
    int score = 0, score_maximo=0;
    FILE *score_max = fopen("recorde.txt", "r");
    fscanf(score_max, "%d", &score_maximo);
    fclose(score_max);
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    ALLEGRO_TIMER *timer = NULL;


    //inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

    //cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

    //incializa addon para a fonte do score
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_FONT *fonte = al_load_ttf_font("arial.ttf", 20, 0);
    if (!fonte) {
        fprintf(stderr, "Erro ao carregar fonte!\n");
        return -1;
    }

    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }

    //cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

    //cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

    //instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
	//instala o mouse
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize mouse!\n");
		return -1;
	}
    
    //registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
    //registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//registra na fila os eventos de mouse (ex: clicar em um botao do mouse)
	al_register_event_source(event_queue, al_get_mouse_event_source());
    //registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer)); 

    //atualiza a tela (quando houver algo para mostrar)
    al_flip_display();

    
    //inicializa o sistema de áudio
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(2);

    ALLEGRO_SAMPLE *musicadefundo = al_load_sample("space-invaders-classic-arcade-game.wav");
    if (!musicadefundo) {
        printf("Erro ao carregar música!\n");
        return -1;
    }
    al_play_sample(musicadefundo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

    ALLEGRO_SAMPLE *somdetiro = al_load_sample("somDeTiro.wav");
    if (!somdetiro) {
        printf("Erro ao carregar música!\n");
        return -1;
    }



    Nave nave;
    initNave(&nave);

    float x = INTIAL_X * (ALIEN_W + 10);
    float y = INTIAL_Y * (ALIEN_H + 10);

    //incializa todos os aliens
    Alien aliens[4][5];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 5; j++){
            //espacamento de ao menos 30px, optei por 50
            x = INTIAL_X + j * (ALIEN_W + 50);
            y = INTIAL_Y + i * (ALIEN_H + 50);
            initAlien(&aliens[i][j], x, y);
    }
}

    Tiro tiro;
    initTiro(&tiro);

    int playing = 1;
    //inicia o temporizador
	al_start_timer(timer);

    while(playing){
        ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

        //se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {
            //atualiza a nave
            update_nave(&nave);
            //atualiza o tiro
            update_tiro(&tiro, nave);
            //atualiza os aliens
            update_alien(aliens, &tiro, nave, &score);

        }

            //desenha todos os aliens
            /*
            for(int i=0; i<4; i++){
                for(int j=0; j<5; j++){
                    draw_alien(aliens[i][j]);
                }

            } 
            */
        
        if (al_is_event_queue_empty(event_queue)) {

            draw_scenario();

            for(int i=0; i<4; i++){
                for(int j=0; j<5; j++){
                    draw_alien(aliens[i][j]);
                    if(colisao_alien_solo(&aliens[i][j])){
                        encerrar_sessao(score, score_maximo);
                    }
                    else if(colisao_alien_nave(&aliens[i][j], &nave)){
                        encerrar_sessao(score, score_maximo);
                    }   
                }
            }

            draw_grass();
            draw_tiro(tiro);
            draw_nave(nave);

            //desenha o score na tela
            char score_text[15];
            char score_max_text[20];
            sprintf(score_text, "Score: %d", score);
            al_draw_text(fonte, al_map_rgb(0, 0, 0), 10, 507, 0, score_text);

            if(score > score_maximo)
                score_maximo = score;
            sprintf(score_max_text, "Recorde: %d", score_maximo);
            al_draw_text(fonte, al_map_rgb(0, 0, 0), 750, 507, 0, score_max_text);

            //atualiza a tela (quando houver algo para mostrar)
			al_flip_display();
        }


			
		    if(al_get_timer_count(timer)%(int)FPS == 0)
			    printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));

        //se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			encerrar_sessao(score, score_maximo);
		}

        //se o tipo de evento for um clique de mouse
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			printf("\nmouse clicado em: %d, %d", ev.mouse.x, ev.mouse.y);
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//imprime qual tecla foi pressionada
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);

            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_SPACE:
                    tiro.velocidadeCima = 7;
                    tiro.cima = 1;
                    if(tiro.y == SCREEN_H - GRASS_H/2 - NAVE_H - TIRO_H + 40){
                        al_play_sample(somdetiro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); //efeito sonoro so funciona durante o disparo
                    }
                    break;
                case ALLEGRO_KEY_A:
                    nave.esq = 1;
                    if(tiro.x != SCREEN_W/2 && tiro.y != SCREEN_H - GRASS_H/2 - NAVE_H - TIRO_H && !tiro.cima){
                        tiro.esq = 1;
                    }
                    break;
                case ALLEGRO_KEY_D:
                    nave.dir = 1;
                    if(tiro.x != SCREEN_W/2 && tiro.y != SCREEN_H - GRASS_H/2 - NAVE_H - TIRO_H && !tiro.cima){
                        tiro.esq = 1;
                    }
                    break;
                    
            }
		}

        else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			//imprime qual tecla foi solta
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);

            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_A:
                    nave.esq = 0;
                    if(tiro.x != SCREEN_W/2 && tiro.y != SCREEN_H - GRASS_H/2 - NAVE_H - TIRO_H && !tiro.cima){
                        tiro.esq = 0;
                    }
                    break;
                case ALLEGRO_KEY_D:
                    nave.dir = 0;
                    if(tiro.x != SCREEN_W/2 && tiro.y != SCREEN_H - GRASS_H/2 - NAVE_H - TIRO_H && !tiro.cima){
                        tiro.esq = 0;
                    }
                    break;
            }
		}

        //verifica se todos os aliens foram destruidos
        int somaDestruidos=0;
        for(int i=0; i<4; i++){
            for(int j=0; j<5; j++){
                if(aliens[i][j].hitbox){
                    somaDestruidos++;
                }
            }
        }
        if(somaDestruidos == 0)
            encerrar_sessao(score, score_maximo);
    }

    al_destroy_display(display);
	al_destroy_event_queue(event_queue);
    return 0;
}
