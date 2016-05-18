/** 
 *	Created by Jorismar Barbosa Meira (21/08/2014)
 *  email: jorismar.barbosa@gmail.com
 */

#include <Windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include "MyLinkedList.h"

int winWidth = 800;
int winHeight = 800;

/* Estrura tipo Cor RGB */
typedef struct {
	double R, G, B;
} tColor;

/* Estrutura tipo Objeto */
typedef struct {
	int iSize;			// Tamanho de indexList.
	int pSize;     		// Tamanho da pointList.
	double *iList;		// Ponteiro para o array dos indices.
	double *pList;		// Ponteiro para o array dos pontos.
	tColor color;		// Cor do objeto (R, G, B).
	double angle;		// Angulo de rotacao do objeto.
	double distance;	// Distancia do objeto em relacao ao eixo Z.
	double scale;		// Escala de tamanho do objeto (Tamanho / Escala).
} tObject;

/* Funcoes Graficas */
void display();							// Display.
void projParallel();					// Seleciona projeção paralela.
void projPerspective();					// Seleciona projeção perspectiva.
void objInit(const char*, tObject*);	// Inicia um objeto.
void drawObject(tObject obj);			// Desenha um Objeto.
void drawAxis();						// Desenha os eixos (x, y, z).
void rotObject(tObject, int);			// Rotaciona um objeto da Cena.

										/* Funcoes Auxiliares */
void keyboard(unsigned char, int, int);		// Controla os eventos de teclado.
void getFaces(const char*, tList*, tList*); // Coleta as faces do objeto.
void readFile(tList*, FILE*, const char*);	// Le o arquivo e coleta os dados.
double* lnkListToArray(tList);				// Copia dados de L. Enc. para Array.

											/* Funcoes do Exercicio */
void exerc1Init(const char*);	// Inicia a lista de objetos do Exercicio 1.
void exerc2Init(const char*);	// Inicia a lista de objetos do Exercicio 2.
void exercicio1();				// Executa o exercicio 1
void exercicio2();				// Executa o exercicio 2

								/* Variaveis Globais */
tObject gObjList[9];	// Array de Objetos
int gExercicio;			// Seletor de exercicio

int main(int argc, char **argv) {
	if (argc > 2) {
		if (argv[1][0] == '1') {		// Exercicio 1 Selecionado
			exerc1Init(argv[2]);
			gExercicio = 1;
		}
		else {					// Exercicio 2 Selecionado
			exerc2Init(argv[2]);
			gExercicio = 2;
		}

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowPosition(0, 0);
		glutInitWindowSize(winWidth, winHeight);
		glutCreateWindow("Controle por Teclado");
		glutKeyboardFunc(keyboard);

		glutDisplayFunc(display);
		glutMainLoop();
	}
	else {
		printf("\n(!) Warnning: Digite o numero do exercicio e o nome do arquivo .x3d.\n\n");
		return 1;
	}

	return 0;
}

/**
* Exibe objetos na tela com OpenGL.
*/
void display() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (gExercicio == 1) exercicio1();
	else exercicio2();

	glFlush();
}

/**
* Seleciona projeção paralela
*/
void projParallel() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, 1, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0, 0, -30);

	/* Rotacionar cena para uma melhor visualização */
	glRotated(-60.0, 1, 0, 0);
	glRotated(-120.0, 0, 0, 1);
	glRotated(30.0f, 0, 1, 0);
}

/**
* Seleciona projeção perspectiva
*/
void projPerspective() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (float)winWidth / winHeight, 1, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0, 0, -36);

	/* Rotacionar cena para uma melhor visualização */
	glRotated(-60.0, 1, 0, 0);
	glRotated(-120.0, 0, 0, 1);
	glRotated(30.0f, 0, 1, 0);
}

/**
* Inicia um objeto, lendo o arquivo de modelo e armazenando numa
* estrutura do tipo objeto.
* @param *filename - Nome do arquivo de modelo.
* @param *obj - Ponteiro de estrutura tObject para armazenamento.
*/
void objInit(const char *filename, tObject *obj) {
	tList iList = listInit();
	tList pList = listInit();
	double *ptr1, *ptr2;

	getFaces(filename, &iList, &pList);

	obj->iSize = getSize(iList);
	obj->pSize = getSize(pList);

	obj->iList = lnkListToArray(iList);
	obj->pList = lnkListToArray(pList);

	obj->angle = 0;
	obj->scale = 1;
	obj->distance = 0;
	obj->color.R = 0.0f;
	obj->color.G = 0.0f;
	obj->color.B = 0.0f;

	remList(&iList);
	remList(&pList);
}

/**
* Desenha um Objeto.
* @param obj - Estrutura do tipo tObject.
*/
void drawObject(tObject obj) {
	int i = 0, ind;

	glColor3f(obj.color.R, obj.color.G, obj.color.B);

	do {
		//glBegin(GL_POLYGON);				// Desenhar faces.
		glBegin(GL_LINE_LOOP);				// Desenhar grade(aramado).
		while (obj.iList[i] != -1) {
			ind = obj.iList[i] * 3;

			glVertex3f(obj.pList[ind] / obj.scale + obj.distance, obj.pList[ind + 1] / obj.scale + obj.distance, obj.pList[ind + 2] / obj.scale);

			i++;
		}
		glEnd();
	} while (++i < obj.iSize);
}

/**
* Desenha os eixos (x, y, z).
*/
void drawAxis() {
	glBegin(GL_LINES);
	// Eixo X (Vermelho)
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(100, 0, 0);

	// Eixo Y (Verde)
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 100, 0);

	// Eixo Z (Azul)
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 100);
	glEnd();
}

/**
* Rotaciona um objeto da Cena.
* @param obj - Objeto a ser rotacionado.
* @para axix - Exibir eixos (1-Sim, 0-Nao).
*/
void rotObject(tObject obj, int axis) {
	glPushMatrix();
	glRotated(obj.angle, 0, 0, 1);

	if (axis) drawAxis();

	drawObject(obj);
	glPopMatrix();
}

/**
* Trata os eventos de teclado
* @param key é a tecla pressionada
* @param x e y são as coordenadas
*/
void keyboard(unsigned char key, int x, int y) {
	if (key == '1') gObjList[0].angle = gObjList[0].angle < 360 ? gObjList[0].angle + 1 : 1; // Sol
	else if (key == '2') gObjList[1].angle = gObjList[1].angle < 360 ? gObjList[1].angle + 1 : 1; 	 // Mercurio
	else if (key == '3') gObjList[2].angle = gObjList[2].angle < 360 ? gObjList[2].angle + 1 : 1; // Venus
	else if (key == '4') gObjList[3].angle = gObjList[3].angle < 360 ? gObjList[3].angle + 1 : 1; // Terra
	else if (key == '5') gObjList[4].angle = gObjList[4].angle < 360 ? gObjList[4].angle + 1 : 1; // Marte
	else if (key == '6') gObjList[5].angle = gObjList[5].angle < 360 ? gObjList[5].angle + 1 : 1; // Jupiter
	else if (key == '7') gObjList[6].angle = gObjList[6].angle < 360 ? gObjList[6].angle + 1 : 1; // Saturno
	else if (key == '8') gObjList[7].angle = gObjList[7].angle < 360 ? gObjList[7].angle + 1 : 1; // Urano
	else if (key == '9') gObjList[8].angle = gObjList[8].angle < 360 ? gObjList[8].angle + 1 : 1; // Netuno

	glutPostRedisplay();
}

/**
* Coleta as faces do objeto
* @param filename é o nome do arquivo .x3d onde esta armazenados os dados do objeto.
*/
void getFaces(const char *filename, tList *iList, tList *pList) {
	FILE *stream;
	int i;

	/* Abre o arquivo, e chama funcao de leitura e coleta dos dados */
	if (stream = fopen(filename, "rt")) {
		readFile(iList, stream, "coordIndex=");
		readFile(pList, stream, "point=");

		fclose(stream);
	}
	else {
		printf("\n(!) ERRO: Não foi possivel ler o arquivo x3d!\n          Verifique se o nome digitado esta\n          correto, e tente novamente.\n\n");
		exit(1);
	}
}

/**
* Le o arquivo e coleta os dados.
* @param list é a lista encadeada para armazenamento dos dados coletados.
* @param stream é o ponteiro para uma estrutura de arquivo aberto em modo texto.
* @param marker é o marcador que indica o inicio da coleta dos dados.
*/
void readFile(tList *list, FILE *stream, const char *marker) {
	tList aux = listInit();
	int i, size;
	char *str, c;

	size = strlen(marker);

	/* Procura a posição inicial dos dados a serem coletados */
	do {
		c = fgetc(stream);

		if (c == marker[0]) {
			i = 0;

			while (i < size && c == marker[i]) {
				c = fgetc(stream);
				i++;
			}

			if (i == size) {
				c = fgetc(stream);
				break;
			}
		}
	} while (c != EOF);

	/* Coleta os dados */
	while (c != EOF && c != '"') {
		while (c != ' ' && c != '"' && c != EOF) {
			addElement(&aux, c);
			c = fgetc(stream);
		}

		if (aux) {
			size = getSize(aux) + 1;
			str = (char*)malloc(sizeof(char) * size);

			for (i = 0; i < size - 1; i++)
				str[i] = dataOf(aux, i);

			str[i] = '\0';

			addElement(list, atof(str));
			free(str);
			remList(&aux);
		}

		c = fgetc(stream);
	}
}

/**
* Copia os dados de um Lista Encadeada para um Array (Melhorar Desempenho).
* @param lnk - Lista Encadeada.
* @param *ar - Array.
* @return - Tamanho do novo array.
*/
double* lnkListToArray(tList lnk) {
	int i, size = getSize(lnk);
	double *ar;

	ar = (double*)malloc(size * sizeof(double));

	if (ar)
		for (i = 0; i < size; i++)
			ar[i] = dataOf(lnk, i);

	return ar;
}

/**
* Inicia todos os objetos do Exercicio 1
* @param *filename - Nome do arquivo modelo
*/
void exerc1Init(const char* filename) {
	int i;

	objInit(filename, &gObjList[0]);

	for (i = 0; i < 4; i++) {
		gObjList[i].angle = 0;
		gObjList[i].scale = 1;
		gObjList[i].distance = 0;
		gObjList[i].color.R = 1.0f;
		gObjList[i].color.G = 0.0f;
		gObjList[i].color.B = 0.0f;
		gObjList[i].iList = gObjList[0].iList;
		gObjList[i].pList = gObjList[0].pList;
		gObjList[i].iSize = gObjList[0].iSize;
		gObjList[i].pSize = gObjList[0].pSize;
	}
}

/**
* Executa o exercicio 1
*/
void exercicio1() {
	glPushMatrix();
	projParallel();
	glMatrixMode(GL_VIEWPORT);

	glViewport(0, 400, 400, 400);
	rotObject(gObjList[0], 1);

	glViewport(400, 400, 400, 400);
	rotObject(gObjList[1], 1);
	glPopMatrix();

	glPushMatrix();
	projPerspective();
	glMatrixMode(GL_VIEWPORT);

	glViewport(0, 0, 400, 400);
	rotObject(gObjList[2], 1);

	glViewport(400, 0, 400, 400);
	rotObject(gObjList[3], 1);
	glPopMatrix();
}

/**
* Inicia todos os objetos do Exercicio 2
* @param *filename - Nome do arquivo modelo
*/
void exerc2Init(const char *filename) {
	int i;
	objInit(filename, &(gObjList[0]));

	for (i = 1; i < 9; i++) {
		gObjList[i].angle = 0;
		gObjList[i].distance = i + 1;
		gObjList[i].iList = gObjList[0].iList;
		gObjList[i].pList = gObjList[0].pList;
		gObjList[i].iSize = gObjList[0].iSize;
		gObjList[i].pSize = gObjList[0].pSize;
	}

	// Sol
	gObjList[0].scale = 1;
	gObjList[0].color.R = 1.0f;
	gObjList[0].color.G = 0.8f;
	gObjList[0].color.B = 0.0f;

	// Mercurio
	gObjList[1].scale = 10;
	gObjList[1].color.R = 0.56f;
	gObjList[1].color.G = 0.3f;
	gObjList[1].color.B = 0.19f;

	// Venus
	gObjList[2].scale = 8;
	gObjList[2].color.R = 0.98f;
	gObjList[2].color.G = 0.8f;
	gObjList[2].color.B = 0.46f;

	// Terra
	gObjList[3].scale = 8;
	gObjList[3].color.R = 0.18f;
	gObjList[3].color.G = 0.54f;
	gObjList[3].color.B = 0.92f;

	// Marte
	gObjList[4].scale = 7;
	gObjList[4].color.R = 0.94f;
	gObjList[4].color.G = 0.32f;
	gObjList[4].color.B = 0.11f;

	// Jupiter
	gObjList[5].scale = 3;
	gObjList[5].color.R = 1.0f;
	gObjList[5].color.G = 0.7f;
	gObjList[5].color.B = 0.3f;

	// Saturno
	gObjList[6].scale = 5;
	gObjList[6].color.R = 1.0f;
	gObjList[6].color.G = 0.37f;
	gObjList[6].color.B = 0.16f;

	// Urano
	gObjList[7].scale = 6;
	gObjList[7].color.R = 0.15f;
	gObjList[7].color.G = 0.48f;
	gObjList[7].color.B = 1.0f;

	// Netuno
	gObjList[8].scale = 6;
	gObjList[8].color.R = 0.07f;
	gObjList[8].color.G = 0.42f;
	gObjList[8].color.B = 1.0f;
}

/**
* Executa o exercicio 2
*/
void exercicio2() {
	int i;

	projPerspective();

	// Sol
	glRotated(gObjList[0].angle, 0, 0, 1);	// Rotaciona a cena
	drawObject(gObjList[0]);				// Desenha o objeto

											// Planetas
	for (i = 1; i < 9; i++)
		rotObject(gObjList[i], 0);
}
