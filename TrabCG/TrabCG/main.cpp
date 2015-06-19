//
//  main.cpp
//  TrabCG
//
//  Created by Hariel Giacomuzzi on 6/11/15.
//  Copyright (c) 2015 Hariel Giacomuzzi. All rights reserved.
//
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "objLoader.h"

#define SENS_ROT	10.0
#define SENS_OBS	1.0
#define SENS_TRANSL	1.0

#define TAM 1000
#define D 100

struct inimigo{
    GLfloat posX;
    GLfloat posY;
    GLfloat posZ;
};


int balas[1];
int moveEsfera;

int x_ini,y_ini,bot;
GLfloat rotX, rotY, rotX_ini, rotY_ini;
GLfloat obsX, obsY=200, obsZ=400, obsX_ini, obsY_ini, obsZ_ini;
GLfloat fAspect = 1, angle = 45;

std::vector< glm::vec3 > vertices;
std::vector< glm::vec3 > uvs;
std::vector< glm::vec3 > normals;
std::vector<inimigo> inimigos;

GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};		 // "cor"
GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
GLfloat posicaoLuz[4]={0.0, 30.0, 120.0, 1.0};

void PosicionaObservador(void)
{
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    
    glTranslatef(-obsX,-obsY,-obsZ); //Outra opcao de camera
    //glRotatef(rotX,1,0,0);
    //glRotatef(rotY,0,1,0);
    
    //gluLookAt(obsX,obsY,obsZ, 0.0,0.0,0.0, 0.0,1.0,0.0);
}

// FunÁ„o usada para especificar o volume de visualizaÁ„o
void EspecificaParametrosVisualizacao(void)
{
    // Especifica sistema de coordenadas de projeÁ„o
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de projeÁ„o
    glLoadIdentity();
    
    // Especifica a projeÁ„o perspectiva
    gluPerspective(angle,fAspect,0.4,5000);
    
    // Especifica posiÁ„o do observador e do alvo
    PosicionaObservador();
}

void DesenhaChao()
{
    //Flags para determinar a cord de cada quadrado
    bool flagx, flagz;
    //Define a normal apontando para cima
    glNormal3f(0,1,0);
    
    glBegin(GL_QUADS);
    flagx = false;
    //X varia de -TAM a TAM, de D em D
    for(float x=-TAM; x<TAM; x+=D)
    {
        //Flagx determina a cor inicial
        if(flagx) flagz = false;
        else flagz = true;
        //Z varia de -TAM a TAM, de D em D
        for (float z=-TAM;z<TAM;z+=D)
        {
            //Escolhe cor
            if(flagz)
                glColor3f(0.4,0.4,0.4);
            else
                glColor3f(1,1,1);
            //E desenha o quadrado
            glVertex3f(x,-60,z);
            glVertex3f(x+D,-60,z);
            glVertex3f(x+D,-60,z+D);
            glVertex3f(x,-60,z+D);
            //Alterna cor
            flagz = !flagz;
        }
        //A cada coluna, alterna cor inicial
        flagx = !flagx;
    }
    glEnd();
}

void DefineIluminacao()
{
    // Capacidade de brilho do material
    GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;
    
    // Habilita o modelo de colorizaÁ„o de Gouraud
    glShadeModel(GL_SMOOTH);
    
    // Define a reflet‚ncia do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // Define a concentraÁ„o do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
    
    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    
    // Define os par‚metros da luz de n˙mero 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
    
    //Desabilita iluminacao para desenhar a esfera
    glDisable(GL_LIGHTING);
    //Desenha esfera na posiÁ„o da fonte de luz
    glPushMatrix();
    glTranslatef(posicaoLuz[0], posicaoLuz[1], posicaoLuz[2]);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(5, 50, 50);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void movimentaInimigos(){
    for (int i = 0; i < inimigos.size(); i++) {
        inimigo a = inimigos[i];
        
        double x = rand() % 10;
        double y = rand() % 10;
        double z = rand() % 10;
        
        a.posX += x;
        a.posY += y;
        a.posZ += 0;
        
        glutSolidCube(50);
        glTranslatef(a.posX+x, a.posY+y, 0);

    }
}

void criaInimigos(int qtd){
    if (inimigos.size() > 0) {
        movimentaInimigos();
        
        
    }else{
        for (int i = 0; i < qtd; i++) {
            inimigo aux;
            aux.posX = rand() % 100;
            aux.posY = rand() % 100;
            aux.posZ = rand() % 100;
            
            inimigos.push_back(aux);
        }
        criaInimigos(qtd);
    }
}

// atiraEsfera
void atiraEsfera(void){
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    moveEsfera = moveEsfera - 10;
    glTranslated(obsX, obsY, moveEsfera);
    glutSolidSphere(1, 50, 50);
    printf("%d", moveEsfera);
    glPopMatrix();
}

// FunÁ„o callback chamada para fazer o desenho
void Desenha(void)
{
    // Limpa a janela e o depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    EspecificaParametrosVisualizacao();
    DesenhaChao();
    
    //DefineIluminacao();
    
    glColor3f(0.0f, 0.0f, 1.0f);
    
    //adiciona forma geometricas randomicas como inimigos xD HueHue
    criaInimigos(5);
    
    if(balas[0] == 1){
        atiraEsfera();
    }
    
    glutSwapBuffers();
}

// Inicializa par‚metros de rendering
void Inicializa (void)
{
    // Especifica que a cor de fundo da janela ser· preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    DefineIluminacao();
    
    // Habilita a definiÁ„o da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    //Habilita o uso de iluminaÁ„o
    glEnable(GL_LIGHTING);
    // Habilita a luz de n˙mero 0
    glEnable(GL_LIGHT0);
    // Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);
}

// FunÁ„o callback chamada quando o tamanho da janela È alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Para previnir uma divis„o por zero
    if ( h == 0 ) h = 1;
    
    // Especifica o tamanho da viewport
    glViewport(0, 0, w, h);
    
    // Calcula a correÁ„o de aspecto
    fAspect = (GLfloat)w/(GLfloat)h;
    
    EspecificaParametrosVisualizacao();
}

// FunÁ„o callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
    if(state==GLUT_DOWN){
        x_ini = x;
        y_ini = y;
        obsX_ini = obsX;
        obsY_ini = obsY;
        obsZ_ini = obsZ;
        rotX_ini = rotX;
        rotY_ini = rotY;
        bot = button;
    }
    else{
        bot = -1;
    }
}

void GerenciaMovim(int x, int y){
    if(bot==GLUT_LEFT_BUTTON){
        int deltax = x_ini - x;
        int deltay = y_ini - y;
        
        rotY = rotY_ini - deltax/SENS_ROT;
        rotX = rotX_ini - deltay/SENS_ROT;
    }
    
    else if(bot==GLUT_RIGHT_BUTTON){
        int deltaz = y_ini - y;
        
        obsZ = obsZ_ini + deltaz/SENS_OBS;
    }
    
    else if(bot==GLUT_MIDDLE_BUTTON){
        int deltax = x_ini - x;
        int deltay = y_ini - y;
        
        obsX = obsX_ini + deltax/SENS_TRANSL;
        obsY = obsY_ini - deltay/SENS_TRANSL;
    }
    PosicionaObservador();
    glutPostRedisplay();
}

void GerenciaTeclado(unsigned char key,int,int){
    if (key == 27) exit(0);
    if (key == 'w') {
        obsZ -= 10;
    }
    if (key == 's') {
        obsZ += 10;
    }
    if (key == 'a') {
        obsX -= 10;
    }
    if (key == 'd') {
        obsX += 10;
    }
    if (key == 32){
        if(balas[0] == 0)
            balas[0] = 1;
    }
    glutPostRedisplay();
}

void GerenciaTecladoEspecial(int key, int x,int y){
    glutPostRedisplay();
}

void Timer(int iUnused){
    glutPostRedisplay();
    glutTimerFunc(30, Timer, 0);
}

// Programa Principal
int main(void){
    int argc = 0;
    char *argv[] = { (char *)"gl", 0 };
    
    balas[0] = 0;
    moveEsfera = obsZ;
    glutInit(&argc,argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("Trabalho II CGI");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMotionFunc(GerenciaMovim);
    glutMouseFunc(GerenciaMouse);
    glutKeyboardFunc(GerenciaTeclado);
    glutSpecialFunc(GerenciaTecladoEspecial);
    Timer(10);
    Inicializa();
    
    glutMainLoop();
    
    return 0;
}
