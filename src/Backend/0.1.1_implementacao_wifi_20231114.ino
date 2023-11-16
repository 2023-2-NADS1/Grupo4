#include <ESP8266WiFi.h>           // Inclui a biblioteca do wifi
#include <Servo.h>           //Inclui a bilioteca do servo


  /*---------------------------------------------------*/


// Funcionamento WiFi

const char* ssid     = "Girassol";           // SSID da rede (nome da rede)
const char* password = "Girassol123";           // Senha de acesso

//Define um IP fixo

IPAddress ip(192,168,0,175);           //IP Disponível
IPAddress gateway(192,168,0,1);           //Gateway de conexão
IPAddress subnet(255,255,255,0);           //Mascara de rede

WiFiServer server(80);           // Define a porta do webservice


  /*---------------------------------------------------*/


// Funcionamento servo, LDR'S e placas

Servo Horizontal;           // Inicia o servo horizontal
Servo Vertical;             // Inicia o servo vertical

int ServoHorizontal = 90;   // Estabelece valor fixo à ServoHorizontal
int ServoVertical = 90;     // Estabelece valor fixo à ServoVertical

int LimiteServoHorizontalMax = 180;  // Estabele os limites de rotação
int LimiteServoHorizontalMin = 65;   // Estabele os limites de rotação

int LimiteServoVerticalMax = 120;    // Estabele os limites de rotação
int LimiteServoVerticalMin = 15;     // Estabele os limites de rotação

int LDR2 = 12;             // Porta do LDR2 D6
int LDR4 = 15;             // Porta do LDR4 D8
int LDR1 = 13;             // Porta do LDR1 D7
int LDR3 = 14;             // Porta do LDR3 D5
int  painel = 9;             // Porta do Painel SD2
int  leituraPainel =0;             // Definimos algumas constantes para o funcionamento do codigo


void setup()  {
  Serial.begin(115200);     // Inicia a leitura dos dados


// Configuração WiFi

  // Imprime as informações do WiFi
  Serial.print("Conectando a ");
  Serial.print(ssid);             //Escreve o nome da rede

  WiFi.begin(ssid, password);             // Passa os parâmetros para a função que vai fazer a conexão com a rede sem fio
  WiFi.config(ip, gateway, subnet);             // Passa os parâmetros para a função que vai setar o IP fixo

  while (WiFi.status() != WL_CONNECTED) {             // Enquanto status for diferente de conectado
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a rede sem fio ");             // Escreve o texto na serial
  Serial.println(ssid);             // Escreve o nome da rede na serial
  server.begin();             // Inicia o servidor para receber dados na porta definida em "WiFiServer server(porta);"
  Serial.println("Servidor iniciado");             // Escreve o texto na serial
  
  Serial.print("IP para se conectar ao NodeMCU: ");             // Escreve o texto na serial
  Serial.print("http://");             // Escreve o texto na serial
  Serial.println(WiFi.localIP());             // Escreve na serial o IP recebido dentro da rede sem fio (o IP nessa prática é recebido de forma automática)



  /*---------------------------------------------------*/
  
// Iniciação servo

  Horizontal.attach(2);     // Inicia servo horizontal na porta 05
  Vertical.attach(0);       // Inicia servo vertical na porta SK

  Horizontal.write(180);    // Inicia servo horizontal na posição 180
  Vertical.write(45);       // Inicia servo vertical na posição 45
  delay(3000);              // Aguarda 3 segundos
  
}

void loop() {
  leituraPainel = analogRead(painel);      // Leitura das placas solares
  Serial.println(leituraPainel);      // Enviamos a leitura do painel para o monitor serial

  int LDC = analogRead(LDR2);      // Leitura Analógica do LDR Direita Cima
  int LEC = analogRead(LDR1);      // Leitura Analógica do LDR Esquerda Cima
  int LDB = analogRead(LDR4);      // Leitura Analógica do LDR Direita Baixo
  int LEB = analogRead(LDR3);      // Leitura Analógica do LDR Esquerda Baixo

  int tol = 50;

  // Imprime as informações dos LDR's
  
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

  int DifSupInf = ValorSup - ValorInf;      // Diferença entre LDR superior e inferior
  int DifDirEsq = ValorDir - ValorEsq;      // Diferença entre LDR direita e esquerda

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


  /*---------------------------------------------------*/

  // Executa os comandos do WiFi

  WiFiClient client = server.available();      // Verifica se algum cliente está conectado no servidor
  if (!client) {      // Se não existir cliente conectado, faz
    return;      // Reexecuta o processo até que algum cliente se conecte ao servidor
  }
  Serial.println("Novo cliente se conectou!");      // Escreve o texto na serial
  while(!client.available()){      // Enquanto cliente estiver conectado
    delay(1);
  }
  String request = client.readStringUntil('\r'); // Faz a leitura da primeira linha da requisição
  Serial.println(request);      // Escreve a requisição na serial
  client.flush();      // Aguarda até que todos os dados de saída sejam enviados ao cliente
  
  client.println("HTTP/1.1 200 OK");      // Escreve para o cliente a versão do http
  client.println("Content-Type: text/html");      // Escreve o tipo de conteudo (texto/html)
  client.println("");
  client.println("<!DOCTYPE HTML>");      // Informa ao navegador a especificação do html
  client.println("<html>");      // Abre a tag "html"
  client.println("<h1><center>Grupo Girassol</center></h1>");      // Escreve na página
  client.println("</html>");      // Fecha a tag "html"
  delay(1);
  Serial.println("Cliente desconectado");      // Escreve texto na serial
  Serial.println("");

}
