#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#define PI 3.1415

HWND hwnd;
HDC hdc;
  
#define ID_60_FPS 60
#define FPS60 1000/60

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

void DrawWorld();

//Variaveis de postura do boneco
float x_AntBraco_EsqCorrendo = 0;
float x_AntBraco_DireitoCorrendo = 0;
float x_Tronco_Correndo = 0;
float x_Pescoco_CorrendoVertice1 = 0;
float x_Pescoco_CorrendoVertice2 = 0;
float x_Boca_CorrendoVertice1 = 0.3;
float x_Boca_CorrendoVertice2 = 0.8;
float y_Boca_CorrendoVertice1 = 5.7;
float x_Bone_CorrendoVertice1 = -0.7;
float x_Bone_CorrendoVertice2 = 1.5;
float y_Bone_CorrendoVertice1 = 7.3;
float y_Bone_CorrendoVertice2 = 7.3;
float x_Cabeca_Correndo = 0;
float x_Olho_Correndo = 0.5;
float y_Olho_Correndo = 6.8;

//Velocidade do boneco
float velocidadeBraco1 = 0;
float velocidadeBraco2 = 0;
float velocidadeCanela1 = 0;
float velocidadeCanela2 = 0;
float velocidadePe = 0;
//Antebraco Esquerdo
float pos_x_AnteBracoEsq = 0.5;
float limiteAnteBracoEsq = -0.5;
//Braco Esquerdo 
float pos_y_BracoEsqVertice2 = 1; 
float pos_x_BracoEsqVertice1 = 0.5;
float pos_x_BracoEsqVertice2 = 0.9; 
float limiteBracoEsqVertice1 = -0.5;
float limiteBracoEsqVertice2 = -0.7;
//Braco Direito
float pos_y_BracoDireitoVertice2 = 1;
float pos_x_BracoDireitoVertice1 = -0.5;
float pos_x_BracoDireitoVertice2 = -0.7; 
float limiteBracoDireitoVertice1 = 0.5;
float limiteBracoDireitoVertice2 = 0.9;
//Antebraco Direito
float pos_x_AnteBracoDireito = -0.5;
float limiteAnteBracoDireito = 0.5;
//Coxa Esquerda
float pos_x_coxaEsq = 0.5; 
float limiteCoxaEsq = -0.3;
float pos_y_coxaEsqVertice1 = -1.5; 
//Coxa Direita
float pos_x_coxaDireita = -0.3; 
float limiteCoxaDireita = 0.5;
float pos_y_coxaDireitaVertice1 = -1.5; 
//Canela Esquerda
float pos_y_canelaEsqVertice1 = -2.8; 
float pos_x_canelaEsqVertice1 = 0.5; 
float pos_x_canelaEsqVertice2 = 0.8; 
float limiteCanelaEsqVertice1 = -0.3;
float limiteCanelaEsqVertice2 = -0.8;
//Canela Direita
float pos_y_canelaDireitaVertice1 = -2.8; 
float pos_x_canelaDireitaVertice1 = -0.3; 
float pos_x_canelaDireitaVertice2 = -0.8; 
float limiteCanelaDireitaVertice1 = 0.5;
float limiteCanelaDireitaVertice2 = 0.8;
//Pe Esquerdo
float pos_x_peEsqVertice1 = 0.8; 
float limitePeEsqVertice1 = -0.8;
//Pe Direito
float pos_x_peDireitoVertice1 = -0.8;
float limitePeDireitoVertice1 = 0.8;

void resetarBoneco();
void Postura();

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow){

  //Window Class
  WNDCLASS wc = {};
  wc.lpszClassName = "My Window Class";
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.lpfnWndProc = WindowProcedure;
  wc.hInstance = hInst;  
  RegisterClass(&wc);
  
  //Window
  hwnd = CreateWindow(
    wc.lpszClassName,
    "My OpenGL App",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    0, 0, 800, 600,
    NULL, NULL, NULL, NULL
  );
    
  //Device Context
  hdc = GetDC(hwnd);
  
  //Pixel Format Descriptor
  PIXELFORMATDESCRIPTOR pfd;
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
   
  //Pixel Format
  int pf;
  pf = ChoosePixelFormat(hdc, &pfd);
  if(pf==0){
  	MessageBox(NULL, "ChoosePixelFormat() failed", "Error", MB_OK);
  }
  if(SetPixelFormat(hdc, pf, &pfd) == FALSE){
  	MessageBox(NULL, "SetPixelFormat() failed", "Error", MB_OK);	
  }
  DescribePixelFormat(hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  
  //OpenGL context
  HGLRC hrc;
  hrc = wglCreateContext(hdc);
  wglMakeCurrent(hdc, hrc);
  
  SetTimer(hwnd, ID_60_FPS, FPS60, NULL);
  
  ShowWindow(hwnd, ncmdshow);
    
  MSG msg = {};
  
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  
  return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
  
	PAINTSTRUCT ps;
		
	switch (msg){
		
		case WM_TIMER:
			if(wp == ID_60_FPS){
				DrawWorld();
				SwapBuffers(hdc);
			}
			break;
	
		case WM_PAINT:
			gluOrtho2D(-10, 10, -10, 10);
			DrawWorld();
			BeginPaint(hwnd, &ps);			
			EndPaint(hwnd, &ps);
			break;
		
		case WM_SIZE:
			glViewport(0, 0, LOWORD(lp), HIWORD(lp));
			PostMessage(hwnd, WM_PAINT, 0, 0);
			break;
	
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		
		case WM_CHAR:
			//w - andar
			if(GetAsyncKeyState(87)||GetAsyncKeyState(119)){
				resetarBoneco();
				velocidadeBraco1 = 0.025;
				velocidadeBraco2 = 0.040;
				velocidadeCanela1 = 0.03;
				velocidadeCanela2 = 0.06;
				velocidadePe = 0.06;
				Postura();
			}
			//r - correr
			if(GetAsyncKeyState(82)||GetAsyncKeyState(114)){
				resetarBoneco();
				velocidadeBraco1 = 0.045;
				velocidadeBraco2 = 0.072;
				velocidadeCanela1 = 0.06;
				velocidadeCanela2 = 0.12;
				velocidadePe = 0.12;
				Postura();
			}
			//s - parar
			if(GetAsyncKeyState(83)||GetAsyncKeyState(115)){
				resetarBoneco();
			}
			break;
}

  	return DefWindowProc(hWnd, msg, wp, lp);
}

void DrawWorld(){
	glClear(GL_COLOR_BUFFER_BIT);
		
	//Chão
	glBegin(GL_LINES);
		glVertex2f(-10, -3);
		glVertex2f(10, -3);	
	glEnd();
	
	//Sol
	GLint verticesSol = 100;	
	GLfloat anguloSol;
	//se utilizar o gl_polygon ele preenche
	glBegin(GL_POLYGON);
	for(int i = 0; i<verticesSol; i++){
		anguloSol = 2* PI*i/verticesSol;
		glVertex2f((cos(anguloSol)* 1) + 8.5, (sin(anguloSol)* 1.4) + 8);
	};
	glEnd();
	
	//Árvore - tronco
	glBegin(GL_LINE_STRIP);
		glVertex2f(7.3, -3);
		glVertex2f(7.3, -1);
		glVertex2f(7.8, -1);
		glVertex2f(7.8, -3);
	glEnd();

	//Árvore - galhos
	glBegin(GL_TRIANGLES);
		glVertex2f(6.5, -1);
		glVertex2f(7.5, 6);
		glVertex2f(8.5, -1);		
	glEnd();
		
	//Casa - Telhado
	glBegin(GL_LINES);
		glVertex2f(-10, 10);
		glVertex2f(-5, 3);
	glEnd();
	
	//Casa - Telhado Acabamento
	glBegin(GL_LINES);
		glVertex2f(-10, 4);
		glVertex2f(-6, 4);
	glEnd();
	
	//Casa - Parede
	glBegin(GL_LINES);
		glVertex2f(-6, -3);
		glVertex2f(-6, 4);
	glEnd();
	
	//Casa - Porta
	glBegin(GL_LINE_STRIP);
		glVertex2f(-9, -3);
		glVertex2f(-9, 0);
		glVertex2f(-7, 0);
		glVertex2f(-7, -3);
	glEnd();
	
	//Cabeça
	GLint vertices = 100;	
	GLfloat angulo;
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i<vertices; i++){
		angulo = 2* PI*i/vertices;
		glVertex2f((cos(angulo)* 1) + x_Cabeca_Correndo, (sin(angulo)* 1.4) + 6.4);
	};
	glEnd();
	
	//Boné
	glBegin(GL_LINES);
		glVertex2f(x_Bone_CorrendoVertice1, y_Bone_CorrendoVertice1);
		glVertex2f(x_Bone_CorrendoVertice2, y_Bone_CorrendoVertice2);	
	glEnd();
	
	//Olho
	GLint verticesOlho = 100;	
	GLfloat anguloOlho;
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i<verticesOlho; i++){
		anguloOlho = 2* PI*i/verticesOlho;
		glVertex2f((cos(anguloOlho)* 0.2) + x_Olho_Correndo, (sin(anguloOlho)* 0.3) + y_Olho_Correndo);
	};
	glEnd();
	
	//Boca
	glBegin(GL_LINES);
		glVertex2f(x_Boca_CorrendoVertice1, 5.7);
		glVertex2f(x_Boca_CorrendoVertice2, y_Boca_CorrendoVertice1);	
	glEnd();
	
	//Pescoço
	glBegin(GL_LINES);
		glVertex2f(x_Pescoco_CorrendoVertice2, 4.5);
		glVertex2f(x_Pescoco_CorrendoVertice1, 5);	
	glEnd();

	//braço esquerdo
	glBegin(GL_LINES);
		glVertex2f(pos_x_AnteBracoEsq, 2);
		glVertex2f(pos_x_BracoEsqVertice2, pos_y_BracoEsqVertice2);	
		
		if(limiteBracoEsqVertice2 == (float)-0.7){
			if(pos_x_BracoEsqVertice2 > limiteBracoEsqVertice2){
				pos_x_BracoEsqVertice2 -= velocidadeBraco2;					
			} else {
				limiteBracoEsqVertice2 = 0.9;
			}
		} else {
			if(pos_x_BracoEsqVertice2  < limiteBracoEsqVertice2){
				pos_x_BracoEsqVertice2 += velocidadeBraco2;	
			} else {
				limiteBracoEsqVertice2 = -0.7;
			}	
		}		
		
		if(limiteBracoEsqVertice1 == (float)-0.5){
			if(pos_x_BracoEsqVertice1 > limiteBracoEsqVertice1){
				pos_x_BracoEsqVertice1 -= velocidadeBraco1;	
			} else {
				limiteBracoEsqVertice1 = 0.5;
			}
		} else {
			if(pos_x_BracoEsqVertice1  < limiteBracoEsqVertice1){
				pos_x_BracoEsqVertice1 += velocidadeBraco1;	
			} else {
				limiteBracoEsqVertice1 = 0.5;
			}	
		}
	glEnd();
	
	//Antebraço esquerdo
	glBegin(GL_LINES);
		glVertex2f(pos_x_AnteBracoEsq, 2);
		glVertex2f(x_AntBraco_EsqCorrendo, 4);	
		if(limiteAnteBracoEsq == (float)0.5){
			if(pos_x_AnteBracoEsq < limiteAnteBracoEsq){
				pos_x_AnteBracoEsq += velocidadeBraco1;	
			} else {
				limiteAnteBracoEsq = -0.5;
			}
		} else {
			if(pos_x_AnteBracoEsq  > limiteAnteBracoEsq){
				pos_x_AnteBracoEsq -= velocidadeBraco1;	
			} else {
				limiteAnteBracoEsq = 0.5;
			}	
		}		
	glEnd();

	//Antebraço direito
	glBegin(GL_LINES);
		glVertex2f(pos_x_AnteBracoDireito, 2);
		glVertex2f(x_AntBraco_DireitoCorrendo, 4);		
		if(limiteAnteBracoDireito == (float)0.5){
			if(pos_x_AnteBracoDireito < limiteAnteBracoDireito){
				pos_x_AnteBracoDireito += velocidadeBraco1;	
			} else {
				limiteAnteBracoDireito = -0.5;
			}
		} else {
			if(pos_x_AnteBracoDireito  > limiteAnteBracoDireito){
				pos_x_AnteBracoDireito -= velocidadeBraco1;	
			} else {
				limiteAnteBracoDireito = 0.5;
			}	
		}	
	glEnd();
	
	//braço direito
	glBegin(GL_LINES);
		glVertex2f(pos_x_BracoDireitoVertice1, 2);
		glVertex2f(pos_x_BracoDireitoVertice2, pos_y_BracoDireitoVertice2);	

		if(limiteBracoDireitoVertice2 == (float)0.9){
			if(pos_x_BracoDireitoVertice2 < limiteBracoDireitoVertice2){
				pos_x_BracoDireitoVertice2 += velocidadeBraco2;					
			} else {
				limiteBracoDireitoVertice2 = -0.7;
			}
		} else {
			if(pos_x_BracoDireitoVertice2  > limiteBracoDireitoVertice2){
				pos_x_BracoDireitoVertice2 -= velocidadeBraco2;	
			} else {
				limiteBracoDireitoVertice2 = 0.9;
			}	
		}		
		
		if(limiteBracoDireitoVertice1 == (float)0.5){
			if(pos_x_BracoDireitoVertice1 < limiteBracoDireitoVertice1){
				pos_x_BracoDireitoVertice1 += velocidadeBraco1;	
			} else {
				limiteBracoDireitoVertice1 = -0.5;
			}
		} else {
			if(pos_x_BracoDireitoVertice1  > limiteBracoDireitoVertice1){
				pos_x_BracoDireitoVertice1 -= velocidadeBraco1;	
			} else {
				limiteBracoDireitoVertice1 = 0.5;
			}	
		}			
	glEnd();
			
	//Tronco
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(x_Tronco_Correndo, 4.5);	
	glEnd();	
	
	//Coxa Esquerda
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(pos_x_coxaEsq, pos_y_coxaEsqVertice1);		
		if(limiteCoxaEsq == (float)-0.3){
			if(pos_x_coxaEsq > limiteCoxaEsq){
				pos_x_coxaEsq -= velocidadeCanela1;	
			} else {
				limiteCoxaEsq = 0.5;
			}
		} else {
			if(pos_x_coxaEsq  < limiteCoxaEsq){
				pos_x_coxaEsq += velocidadeCanela1;
			} else {
				limiteCoxaEsq = -0.3;
			}	
		}		
	glEnd();

	//Canela Esquerda
	glBegin(GL_LINES);
		glVertex2f(pos_x_canelaEsqVertice1, pos_y_coxaEsqVertice1);
		glVertex2f(pos_x_canelaEsqVertice2, pos_y_canelaEsqVertice1);	

		if(limiteCanelaEsqVertice2 == (float)-0.8){
			if(pos_x_canelaEsqVertice2 > limiteCanelaEsqVertice2){
				pos_x_canelaEsqVertice2 -= velocidadeCanela2;	
				
			} else {
				limiteCanelaEsqVertice2 = 0.8;
			}
		} else {
			if(pos_x_canelaEsqVertice2  < limiteCanelaEsqVertice2){
				pos_x_canelaEsqVertice2 += velocidadeCanela2;
					
			} else {
				limiteCanelaEsqVertice2 = -0.8;
			}	
		}		
		
		if(limiteCanelaEsqVertice1 == (float)-0.3){
			if(pos_x_canelaEsqVertice1 > limiteCanelaEsqVertice1){
				pos_x_canelaEsqVertice1 -= velocidadeCanela1;	
			} else {
				limiteCanelaEsqVertice1 = 0.5;
			}
		} else {
			if(pos_x_canelaEsqVertice1  < limiteCanelaEsqVertice1){
				pos_x_canelaEsqVertice1 += velocidadeCanela1;	
			} else {
				limiteCanelaEsqVertice1 = -0.3;
			}	
		}			
	glEnd();


	//Pé Esquerdo
	glBegin(GL_LINES);
		glVertex2f(pos_x_peEsqVertice1, pos_y_canelaEsqVertice1);
		glVertex2f(pos_x_peEsqVertice1 + 0.35, pos_y_canelaEsqVertice1); //Mudei
		
		if(limitePeEsqVertice1 == (float)-0.8){
			if(pos_x_peEsqVertice1 > limitePeEsqVertice1){
				pos_x_peEsqVertice1 -= velocidadePe;	
			} else {
				limitePeEsqVertice1 = 0.8;
			}
		} else {
			if(pos_x_peEsqVertice1  < limitePeEsqVertice1){
				pos_x_peEsqVertice1 += velocidadePe;	
			} else {
				limitePeEsqVertice1 = -0.8;
			}
		}
	glEnd();

		
	//Coxa Direita
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(pos_x_coxaDireita, pos_y_coxaDireitaVertice1);		
	
		if(limiteCoxaDireita == (float)0.5){
			if(pos_x_coxaDireita < limiteCoxaDireita){
				pos_x_coxaDireita += velocidadeCanela1;	
			} else {
				limiteCoxaDireita = -0.3;
			}
		} else {
			if(pos_x_coxaDireita  > limiteCoxaDireita){
				pos_x_coxaDireita -= velocidadeCanela1;
			} else {
				limiteCoxaDireita = 0.5;
			}	
		}				
	glEnd();	

	//Canela Direita
	glBegin(GL_LINES);
		glVertex2f(pos_x_canelaDireitaVertice1, pos_y_coxaDireitaVertice1);
		glVertex2f(pos_x_canelaDireitaVertice2, pos_y_canelaDireitaVertice1);	

		if(limiteCanelaDireitaVertice2 == (float)0.8){
			if(pos_x_canelaDireitaVertice2 < limiteCanelaDireitaVertice2){
				pos_x_canelaDireitaVertice2 += velocidadeCanela2;		
			} else {
				limiteCanelaDireitaVertice2 = -0.8;
			}
		} else {
			if(pos_x_canelaDireitaVertice2  > limiteCanelaDireitaVertice2){
				pos_x_canelaDireitaVertice2 -= velocidadeCanela2;		
			} else {
				limiteCanelaDireitaVertice2 = 0.8;
			}	
		}		
		
		if(limiteCanelaDireitaVertice1 == (float)0.5){
			if(pos_x_canelaDireitaVertice1 < limiteCanelaDireitaVertice1){
				pos_x_canelaDireitaVertice1 += velocidadeCanela1;	
			} else {
				limiteCanelaDireitaVertice1 = -0.3;
			}
		} else {
			if(pos_x_canelaDireitaVertice1  > limiteCanelaDireitaVertice1){
				pos_x_canelaDireitaVertice1 -= velocidadeCanela1;	
			} else {
				limiteCanelaDireitaVertice1 = 0.5;
			}	
		}			
	glEnd();
	

	//Pé Direito
	glBegin(GL_LINES);
		glVertex2f(pos_x_peDireitoVertice1, pos_y_canelaDireitaVertice1);
		glVertex2f(pos_x_peDireitoVertice1 + 0.35, pos_y_canelaDireitaVertice1); //Mudei
		
		if(limitePeDireitoVertice1 == (float)0.8){
			if(pos_x_peDireitoVertice1 < limitePeDireitoVertice1){
				pos_x_peDireitoVertice1 += velocidadePe;	
			} else {
				limitePeDireitoVertice1 = -0.8;
			}
		} else {
			if(pos_x_peDireitoVertice1  > limitePeDireitoVertice1){
				pos_x_peDireitoVertice1 -= velocidadePe;	
			} else {
				limitePeDireitoVertice1 = 0.8;
			}
		}	
	glEnd();

}

void resetarBoneco(){	
	//Velocidade do boneco
	velocidadeBraco1 = 0;
	velocidadeBraco2 = 0;
	velocidadeCanela1 = 0;
	velocidadeCanela2 = 0;
	velocidadePe = 0;
	//Antebraco Esquerdo
	pos_x_AnteBracoEsq = 0.5;
	limiteAnteBracoEsq = -0.5;	
	//Braco Esquerdo
	pos_y_BracoEsqVertice2 = 1;
	pos_x_BracoEsqVertice1 = 0.5;
	pos_x_BracoEsqVertice2 = 0.9;
	limiteBracoEsqVertice1 = -0.5;
	limiteBracoEsqVertice2 = -0.7;	
	//Braco Direito
	pos_y_BracoDireitoVertice2 = 1;
	pos_x_BracoDireitoVertice1 = -0.5;
	pos_x_BracoDireitoVertice2 = -0.7;
	limiteBracoDireitoVertice1 = 0.5;
	limiteBracoDireitoVertice2 = 0.9;
	//Antebraco Direito
	pos_x_AnteBracoDireito = -0.5;
	limiteAnteBracoDireito = 0.5; 
	//Coxa Esquerda
	pos_x_coxaEsq = 0.5;
	limiteCoxaEsq = -0.3;
	pos_y_coxaEsqVertice1 = -1.5;
	//Coxa Direita
	pos_x_coxaDireita = -0.3;
	limiteCoxaDireita = 0.5;
	pos_y_coxaDireitaVertice1 = -1.5;
	//Canela Esquerda
	pos_y_canelaEsqVertice1 = -2.8;
	pos_x_canelaEsqVertice1 = 0.5;
	pos_x_canelaEsqVertice2 = 0.8;
	limiteCanelaEsqVertice1 = -0.3;
	limiteCanelaEsqVertice2 = -0.8;
	//Canela Direita
	pos_y_canelaDireitaVertice1 = -2.8;
	pos_x_canelaDireitaVertice1 = -0.3;
	pos_x_canelaDireitaVertice2 = -0.8;
	limiteCanelaDireitaVertice1 = 0.5;
	limiteCanelaDireitaVertice2 = 0.8;
	//Pe Esquerdo
	pos_x_peEsqVertice1 = 0.8; //Mudei
	limitePeEsqVertice1 = -0.8;
	//Pe Direito
	pos_x_peDireitoVertice1 = -0.8;
	limitePeDireitoVertice1 = 0.8;
	//limite para verificar se está andando
	Postura();
}

void Postura(){
	x_AntBraco_EsqCorrendo = 0;
	x_AntBraco_DireitoCorrendo = 0;
	x_Tronco_Correndo = 0;
	x_Pescoco_CorrendoVertice1 = 0;
	x_Pescoco_CorrendoVertice2 = 0;
	x_Boca_CorrendoVertice1 = 0.3;
	x_Boca_CorrendoVertice2 = 0.8;
	y_Boca_CorrendoVertice1 = 5.7;
	x_Bone_CorrendoVertice1 = -0.7;
	x_Bone_CorrendoVertice2 = 1.5;
	y_Bone_CorrendoVertice1 = 7.3;
	y_Bone_CorrendoVertice2 = 7.3;
	x_Cabeca_Correndo = 0;
	x_Olho_Correndo = 0.5;
	y_Olho_Correndo = 6.8;
}



