#include <Servo.h> //Inclui a bilioteca do servo
#define BLYNK_TEMPLATE_ID "TMPL2bJ74D0ts"
#define BLYNK_TEMPLATE_NAME "Girasol"
#define BLYNK_AUTH_TOKEN "rDJSWl0Nuhh9olSXXwEnCVEjUXgLlR9C"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Magal Spindola";
char pass[] = "Podeusar";


Servo Horizontal;           // Inicia o servo horizontal
Servo Vertical;             // Inicia o servo vertical

int ServoHorizontal = 90;   // Estabelece valor fixo à ServoHorizontal
int ServoVertical = 90;     // Estabelece valor fixo à ServoVertical

int LimiteServoHorizontalMax = 180;  // Estabele os limites de rotação
int LimiteServoHorizontalMin = 65;   // Estabele os limites de rotação

int LimiteServoVerticalMax = 90;    // Estabele os limites de rotação
int LimiteServoVerticalMin = 15;     // Estabele os limites de rotação

int LDR2 = 15;             // Porta do LDR2 D6
int LDR4 = 13;             // Porta do LDR4 D8
int LDR1 = 12;             // Porta do LDR1 D7
int LDR3 = 14;             // Porta do LDR3 D5
int  painel = 9;             // Porta do Painel SD2
int  leituraPainel =0;             // Definimos algumas constantes para o funcionamento do codigo


void setup() 
{
 Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);



  Serial.begin(115200);     // Inicia a leitura dos dados da placa solar

  Horizontal.attach(2);     // Inicia servo horizontal na porta D4
  Vertical.attach(0);       // Inicia servo vertical na porta D3

  Horizontal.write(180);    // Inicia servo horizontal na posição 180
  Vertical.write(90);       // Inicia servo vertical na posição 90
  delay(3000);              // Aguarda 3 segundos
}

void loop() 
{


  leituraPainel = analogRead(painel);      // Leitura das placas solares
  Serial.println(leituraPainel);      // Enviamos a leitura do painel para o monitor serial

  int LDC = analogRead(LDR2);      // Leitura Analógica do LDR Direita Cima
  int LEC = analogRead(LDR1);      // Leitura Analógica do LDR Esquerda Cima
  int LDB = analogRead(LDR4);      // Leitura Analógica do LDR Direita Baixo
  int LEB = analogRead(LDR3);      // Leitura Analógica do LDR Esquerda Baixo

  Blynk.run(); // colocar no loop


Blynk.virtualWrite(V0, LDC);
Blynk.virtualWrite(V1, LEC);
Blynk.virtualWrite(V2, LDB);
Blynk.virtualWrite(V3, LEB);
Blynk.virtualWrite(V4, leituraPainel);

  int tol = 50;
  Serial.println("LDR Direita Cima");
  Serial.println(LDC);
  Serial.println("LDR Esquerda Cima");
  Serial.println(LEC);
  Serial.println("LDR Direita Baixo");
  Serial.println(LDB);
  Serial.println("LDR Esquerda Baixo");
  Serial.println(LEB);

  int ValorSup = (LDC + LEC) / 2;   // Média da leitura dos LDR superior
  int ValorInf = (LDB + LEB) / 2;   // Média da leitura dos LDR inferior
  int ValorDir = (LDC + LDB) / 2;   // Média da leitura dos LDR da direita
  int ValorEsq = (LEC + LEB) / 2;   // Média da leitura dos LDR da esquerda

  int DifSupInf = ValorSup - ValorInf;      // Diferença entre LED superior e inferior
  int DifDirEsq = ValorDir - ValorEsq;      // Diferença entre LED direita e esquerda
  Serial.println("Superior");
  Serial.println(DifSupInf);
  Serial.println("Direita");
  Serial.println(DifDirEsq);
  Serial.println("Tol");
  Serial.println(-1*tol);
  /*---------------------------------------------------*/

  // Realiza a leitura e executa os movimentos referente ao servo vertical
  if (-1 * tol > DifSupInf || DifSupInf > tol)  {
    if (ValorSup > ValorInf)  {
      ServoVertical = ++ServoVertical;
      if (ServoVertical > LimiteServoVerticalMax)  {
        ServoVertical = LimiteServoVerticalMax;
      }
    }
    else if (ValorSup < ValorInf)  {
      ServoVertical = --ServoVertical;
      if (ServoVertical < LimiteServoVerticalMin)  {
        ServoVertical = LimiteServoVerticalMin;
      }
    }
    Vertical.write(ServoVertical);
  }

  // Realiza a leitura e executa os movimentos referente ao servo horizontal
  if (-1 * tol > DifDirEsq || DifDirEsq > tol) {
    if (ValorDir > ValorEsq)  {
      ServoHorizontal = --ServoHorizontal;
      if (ServoHorizontal < LimiteServoHorizontalMin)  {
        ServoHorizontal = LimiteServoHorizontalMin;
      }
    }
    else if (ValorDir < ValorEsq)  {
      ServoHorizontal = ++ServoHorizontal;
      if (ServoHorizontal > LimiteServoHorizontalMax)  {
        ServoHorizontal = LimiteServoHorizontalMax;
      }
    }
    else if (ValorDir = ValorEsq)  {
    }
    Horizontal.write(ServoHorizontal);
  }

  delay(100);   // Aguarda 0,1 segundo
}
