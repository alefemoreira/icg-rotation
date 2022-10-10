// HERIBERTO
// JACKSON
#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include <math.h>

void init();
void draw_test();

void mouse_test(GLint button, GLint action, GLint x, GLint y);
void mouse_test2(GLint x, GLint y);
void mouse_test3(GLint x, GLint y);

void keybord_test(GLubyte key, GLint x, GLint y);
void keybord_test2(GLint key, GLint x, GLint y);

GLint WINDOW_WIDTH = 500,
WINDOW_HEIGHT = 500;

//armazena os vértices de um objeto
struct VERTEX
{
    int x;
    int y;
};
//armazena a descrição geométrica de um objeto
struct OBJECT
{
    VERTEX* vertices;
    int nrvertices;
};

double tx = 0, ty = 0, sx = 1, sy = 1;

int angulo = 0;
int inc = 1;
double s = 0.0; // armazenar seno (da matriz de rotacao)
double c = 1.0; // armazenar cosseno (da matriz de rotacao)

OBJECT* object; //objeto global que será desenhado

OBJECT* create_object()
{
    OBJECT* obj = (OBJECT*)malloc(sizeof(OBJECT));
    obj->nrvertices = 5;
    obj->vertices = (VERTEX*)malloc(obj->nrvertices * sizeof(VERTEX));
    obj->vertices[0].x = 250;
    obj->vertices[0].y = 130;
    obj->vertices[1].x = 250;
    obj->vertices[1].y = 170;
    obj->vertices[2].x = 220;
    obj->vertices[2].y = 190;
    obj->vertices[3].x = 190;
    obj->vertices[3].y = 170;
    obj->vertices[4].x = 190;
    obj->vertices[4].y = 130;
    return obj;
}

VERTEX calculate_centroid(OBJECT* obj)
{
    int i;
    VERTEX cent;
    cent.x = 0;
    cent.y = 0;
    for (i = 0; i < obj->nrvertices; i++)
    {
        cent.x += obj->vertices[i].x;
        cent.y += obj->vertices[i].y;
    }
    cent.x /= obj->nrvertices;
    cent.y /= obj->nrvertices;
    return cent;
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);

    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    object = create_object(); //cria o objeto
}


// FUNÇÃO QUE REALIZA A TRANSLAÇÃO E ESCALA DO OBJETO
void aplicaTransformacoes(VERTEX vertex, VERTEX centro, double sx, double sy, double tx, double ty, int* matrizFinal) {   
    double s = sin(((angulo) * M_PI) / 180);
    double c = cos(((angulo) * M_PI) / 180);

    int matrizObjeto[3] = { vertex.x, vertex.y, 1 };
    double a = centro.x - centro.x * c + centro.y * s;
    double b = centro.y - centro.y * c - centro.x * s;
    double A = -sx * centro.x + centro.x + tx;
    double B = -sy * centro.y + centro.y + ty;
    double matrizTransformacao[3][3] = { sx * c,  -s * sy, a + A*c - B*s,
                                         s * sx ,  c * sy, b + B*c + A*s,
                                         0 ,  0, 1 };
    for (int j = 0; j < 3; j++)
    {
        for (int k = 0; k < 3; k++)
        {
            matrizFinal[j] += matrizObjeto[k] * matrizTransformacao[j][k];
        }
    }
}
void draw_object(OBJECT* obj)
{
    VERTEX centro = calculate_centroid(obj);
    int i,j,k;    
    glBegin(GL_POLYGON); //Poligono preenchido
    for (i = 0; i < obj->nrvertices; i++)
    {         
        int matrizFinal[3] = { 0,0,0 };
        aplicaTransformacoes(obj->vertices[i], centro, sx, sy, tx, ty, matrizFinal);
        glVertex2i(matrizFinal[0], matrizFinal[1]);
    }            
    glEnd();
}

void draw_test()
{
    glClear(GL_COLOR_BUFFER_BIT); //desenha o fundo (limpa a janela)
    glMatrixMode(GL_MODELVIEW); //garante que a matrix seja a ModelView
    glLoadIdentity(); //carrega a matrix identidade

    glColor3f(0.0, 0.0, 1.0); //altera o atributo de cor
    draw_object(object); //desenha o objeto

    VERTEX cent = calculate_centroid(object); //calcula o centróide

    glFlush(); //processa as rotinas OpenGL o mais rápido possível
}


void mouse_test(GLint button, GLint action, GLint x, GLint y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            angulo += inc;
            break;
        case GLUT_RIGHT_BUTTON:
            angulo -= inc;
            break;
    } 
    glutPostRedisplay();

}

// funcao para processar eventos de teclado
void keybord_test(GLubyte key, GLint x, GLint y)
{
    GLint m = glutGetModifiers();
    std::cout << m;

    if (m == GLUT_ACTIVE_SHIFT)
        std::cout << "Shift ou Caps ";
    else if (m == GLUT_ACTIVE_CTRL)
        std::cout << "Control ";
    else if (m == GLUT_ACTIVE_ALT)
        std::cout << "Alt ";

    //VERIFICAR TABELA ASCII QUANDO O CTRL ESTIVER PRECIONADO COM ALGUMA 
    //LETRA  
    if (m == GLUT_ACTIVE_CTRL && (GLint)key == 4)
        exit(EXIT_SUCCESS);

    std::cout << "Tecla: " << (GLint)key << " (x:" << x << ", y:" << y << ")\n";

    // Escala
    if (key == 119) {
        sy+=0.1; // tecla W
    }
    if (key == 115) {
        sy -= 0.1; // tecla S
    }
    if (key == 43) {
        inc++;
    }
    if (key == 45) {
        if (inc > 1) {
            inc--;
        }
    }
    if (key == 32) {
        tx = 0;
        ty = 0;
        sx = 1;
        sy = 1;
        angulo = 0;
    }
    if (key == 97) {
        sx -= 0.1; // tecla A
    }
    if (key == 100) {
        sx += 0.1; // tecla D
    }
    glutPostRedisplay();

    //ESC = 27
    if (key == 27)
        glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void keybord_test2(GLint key, GLint x, GLint y)
{
    //GLUT_KEY_F1 .. GLUT_KEY_F12
    //GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT
    //GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME etc.  
    if (key == GLUT_KEY_F11)
        glutFullScreen();

    // Movimento
    if (key == 100) {
        tx--; // tecla para esquerda
    }
    if (key == 102) {
        tx++; // tecla para direita
    }
    if (key == 103) {
        ty--; // tecla para baixo
    }
    if (key == 101) {
        ty++; // tecla para cima
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    GLint screen_width = glutGet(GLUT_SCREEN_WIDTH),
        screen_height = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitWindowPosition((screen_width - WINDOW_WIDTH) / 2, (screen_height - WINDOW_WIDTH) / 2);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_WIDTH);
    glutCreateWindow("OpenGL - Transformacoes");

    init();
    glutDisplayFunc(draw_test);

    glutMouseFunc(mouse_test);

    glutKeyboardFunc(keybord_test);
    glutSpecialFunc(keybord_test2);

    glutMainLoop();

    return EXIT_SUCCESS;
}


