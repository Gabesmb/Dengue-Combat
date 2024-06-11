#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>

// Variáveis globais
GLfloat tx = 0, ty = -75;
GLfloat mosquitoX = 0.0f, mosquitoY[5], speed = 0.25f;
int mosquitoHitCount[5] = {0};
GLboolean tiroAtivo = GL_FALSE;
GLfloat tiroX, tiroY, direcaoX, direcaoY, magnitude;
GLboolean jogoIniciado = GL_FALSE;
GLfloat win = 100;
int pontos = 0;
int level = 1;
int mosquitosNoChao = 0;
GLboolean gameOver = GL_FALSE;

// Funções para o desenho base dos objetos, torso do personagem, torso do mosquito e a bala do tiro
void DesenhaObjeto() {
    glBegin(GL_QUADS);
        glVertex2f(0.0, 0.0);
        glVertex2f(1.0, 0.0);
        glVertex2f(1.0, 1.0);
        glVertex2f(0.0, 1.0);
    glEnd();
}

void DesenhaMosquitos() {
    GLfloat posicaoX = -100.0f;

    for (int quantidade = 0; quantidade < 5; quantidade++) {
        if (mosquitoHitCount[quantidade] < 2) {
            glPushMatrix();
            glTranslatef(mosquitoX + posicaoX, mosquitoY[quantidade], 0.0f);
            glScalef(5.0f, 5.0f, 1.0f);
            glColor3f(0.0f, 1.0f, 0.0f);
            DesenhaObjeto();
            glPopMatrix();
        } else if (mosquitoY[quantidade] == -200.0f) {
            mosquitoY[quantidade] = 250.0f + rand() % 150; // Renascimento do mosquito
            mosquitoHitCount[quantidade] = 0;
        }

        posicaoX += 40.0;
    }
}

// Função janela
void Inicializa(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    for (int i = 0; i < 5; i++) {
        mosquitoY[i] = 150.0f + rand() % 150;
        mosquitoHitCount[i] = 0; // Inicializa os contadores de tiros
    }
}

// Função para desenhar o menu
void DesenhaMenu() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-50, 50);
    char texto1[] = "Menu:";
    for (int i = 0; texto1[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto1[i]);
    }

    glRasterPos2f(-50, 30);
    char texto2[] = "Pressione Enter para iniciar";
    for (int i = 0; texto2[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto2[i]);
    }

    glRasterPos2f(-50, 10);
    char texto3[] = "Pressione Q para sair";
    for (int i = 0; texto3[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto3[i]);
    }
}

// Funções desenhar
void Desenha(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(-20, 0);
        char textoGameOver[] = "GAME OVER";
        for (int i = 0; textoGameOver[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, textoGameOver[i]);
        }
        glRasterPos2f(-40, -20);
        char texto4[] = "Pressione Enter para reiniciar";
    	for (int i = 0; texto4[i] != '\0'; i++) {
        	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto4[i]);
    	}
    } else {
        //Menu
        if (!jogoIniciado) {
            DesenhaMenu();
        } else {
            glColor3f(0.0f, 0.0f, 0.0f);
            glLineWidth(4);
            glBegin(GL_LINE_LOOP);
                glVertex2f(-win, -75);
                glVertex2f(win, -75);
            glEnd();
            
            //Desenha Mosquito
            glPushMatrix();
                glTranslatef(tx, ty, 0.0f);
                glScalef(10.0f, 20.0f, 1.0f);
                glColor3f(1.0f, 0.0f, 0.0f);
                DesenhaObjeto();
            glPopMatrix();
               
           
            //função para gereção dos demais Mosquitos
            DesenhaMosquitos();
            
            // Desenha o tiro
            if (tiroAtivo) {
                glPushMatrix();
                    glTranslatef(tiroX, tiroY, 0.0f);
                    glScalef(1.0f, 5.0f, 1.0f);
                    glColor3f(1.0f, 1.0f, 0.0f);
                    DesenhaObjeto();
                glPopMatrix();
            }

            // Exibe os pontos e o nível
            glRasterPos2f(-90, 90);
            char textoPontos[20];
            sprintf(textoPontos, "Pontos: %d", pontos);
            for (int i = 0; textoPontos[i] != '\0'; i++) {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, textoPontos[i]);
            }
        
            char textoLevel[20];
            sprintf(textoLevel, "Nivel: %d", level);
            glRasterPos2f(-90, 80);
            for (int i = 0; textoLevel[i] != '\0'; i++) {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, textoLevel[i]);
            }
        }
    }

    glFlush();
}

// Função para animação
void Atualiza(int value) {
    if (!jogoIniciado) {
        glutPostRedisplay();
        glutTimerFunc(10, Atualiza, 10);
        return;
    }

    for (int i = 0; i < 5; i++) {
        mosquitoY[i] -= speed;

        // Verifica se o mosquito tocou a linha do jogador
        if (mosquitoY[i] <= -75.0f && mosquitoY[i] >= -77.0f) {
            mosquitoY[i] = 250.0f; // Reposiciona o mosquito acima da tela
            mosquitoHitCount[i] = 0; // Redefine o contador de tiros
            mosquitosNoChao++;
            if (mosquitosNoChao >= 5) {
                gameOver = GL_TRUE;
                jogoIniciado = GL_FALSE;
            }
        }
    }

    // Atualiza posição do tiro
    if (tiroAtivo) {
        tiroX += direcaoX;
        tiroY += direcaoY;

        // Verifica se o tiro saiu da tela
        if (tiroX > win || tiroX < -win || tiroY > win || tiroY < -win) {
            tiroAtivo = GL_FALSE; // Desativa o tiro quando sai da tela
        }

        // Verifica colisão do tiro com os mosquitos
        for (int i = 0; i < 5; i++) {
            if (mosquitoHitCount[i] < 2) { // Verifica colisão apenas se o mosquito não foi atingido 2 vezes
                GLfloat mosquitoXPos = mosquitoX - 100.0f + i * 40.0f;
                if (fabs(tiroX - mosquitoXPos) < 5.0f && fabs(tiroY - mosquitoY[i]) < 5.0f) {
                    mosquitoHitCount[i]++;
                    tiroAtivo = GL_FALSE; // Desativa o tiro após a colisão
                    if (mosquitoHitCount[i] == 2) {
                        // Faz o mosquito desaparecer se atingido 2 vezes
                        mosquitoY[i] = -200.0f;
                        pontos += 100; // Aumenta a pontuação quando o mosquito é atingido
                    }
                    break;
                }
            }
        }
    }

    // Verifica se avançou de nível
    int pontuacaoMinima = 200 * (1 << (level - 1));
    if (pontos >= pontuacaoMinima) {
        level++;
        speed += 0.2f; // Aumenta a velocidade dos mosquitos
    }

    glutPostRedisplay();
    glutTimerFunc(10, Atualiza, 10);
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (w <= h) {
        gluOrtho2D(-100.0f, 100.0f, -100.0f * h / w, 100.0f * h / w);
        win = 100.0f;
    } else {
        gluOrtho2D(-100.0f * w / h, 100.0f * w / h, -100.0f, 100.0f);
        win = 100.0f * w / h;
    }
}

void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (jogoIniciado) {
            tiroX = tx; // A posição do tiro é igual à posição atual do jogador
            tiroY = ty + 20.0f;
            tiroAtivo = GL_TRUE;

            // Calcula a direção do tiro
            GLfloat mouseX = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 200.0f - 100.0f;
            GLfloat mouseY = ((float)(glutGet(GLUT_WINDOW_HEIGHT) - y) / glutGet(GLUT_WINDOW_HEIGHT)) * 200.0f - 100.0f;

            // Normaliza a direção considerando a proporção da janela
            direcaoX = mouseX - tiroX;
            direcaoY = mouseY - tiroY;

            GLfloat aspectRatio = (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / (GLfloat)glutGet(GLUT_WINDOW_HEIGHT);
            direcaoX *= aspectRatio;

            // Normaliza a direção
            magnitude = sqrt(direcaoX * direcaoX + direcaoY * direcaoY);
            direcaoX /= magnitude;
            direcaoY /= magnitude;

            // Aumenta a velocidade do tiro
            direcaoX *= 5.0f;
            direcaoY *= 5.0f;
        }
    }
}

void TeclasEspeciais(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        tx -= 4;
        if (tx < -win) tx = -win;
    }
    if (key == GLUT_KEY_RIGHT) {
        tx += 4;
        if (tx > win) tx = win;
    }
}

void Teclado(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q') exit(0); // Tecla Q ou q: fecha o jogo
    
    if (key == 13 && (!jogoIniciado || gameOver)) { // Tecla Enter: inicia ou reinicia o jogo
        jogoIniciado = GL_TRUE;
        gameOver = GL_FALSE; // Resetar o estado do game over
        mosquitosNoChao = 0;
        pontos = 0;
        level = 1;
        speed = 0.25f;
        Inicializa();
        glutTimerFunc(10, Atualiza, 10); // Inicia a animação
    }
    
    if (key == 'a' || key == 'A') {
        tx -= 4;
        if (tx < -win) tx = -win;
    }
    if (key == 'd' || key == 'D') {
        tx += 4;
        if (tx > win) tx = win;
    }
}


// Função principal
int main(int argc, char **argv) {
    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Dengue Defense");

    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(Teclado);
    glutSpecialFunc(TeclasEspeciais);
    glutMouseFunc(Mouse); // Registra a função de callback para o mouse

    Inicializa();

    glutMainLoop();

    return 0;
}

