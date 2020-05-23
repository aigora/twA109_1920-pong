#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>

static const unsigned char pong []PROGMEM = {
0xff,0xe0,0x0,0x3f,0x80,0x7,0xe0,0x7,0xc0,0x3,0xfc,0x0,
0xff,0xf8,0x1,0xff,0xe0,0x7,0xf0,0x7,0xc0,0x1f,0xff,0x0,
0xff,0xfc,0x3,0xff,0xf0,0x7,0xf0,0x7,0xc0,0x3f,0xff,0x0,
0xff,0xfe,0x7,0xff,0xf8,0x7,0xf8,0x7,0xc0,0xff,0xff,0x0,
0xf8,0x7f,0xf,0xff,0xfc,0x7,0xfc,0x7,0xc0,0xff,0xff,0x0,
0xf8,0x3f,0xf,0xe0,0xfe,0x7,0xfc,0x7,0xc1,0xfc,0x7,0x0,
0xf8,0x1f,0x1f,0x80,0x7e,0x7,0xfe,0x7,0xc3,0xf8,0x1,0x0,
0xf8,0x1f,0x1f,0x0,0x3e,0x7,0xfe,0x7,0xc3,0xf0,0x0,0x0,
0xf8,0x1f,0x3f,0x0,0x3f,0x7,0xdf,0x7,0xc7,0xe0,0x0,0x0,
0xf8,0x1f,0x3e,0x0,0x1f,0x7,0xdf,0x87,0xc7,0xc0,0x0,0x0,
0xf8,0x3f,0x3e,0x0,0x1f,0x7,0xcf,0x87,0xc7,0xc1,0xff,0x80,
0xf8,0x7e,0x3e,0x0,0x1f,0x7,0xc7,0xc7,0xc7,0xc1,0xff,0x80,
0xff,0xfe,0x3e,0x0,0x1f,0x7,0xc7,0xe7,0xc7,0xc1,0xff,0x80,
0xff,0xfc,0x3e,0x0,0x1f,0x7,0xc3,0xe7,0xc7,0xc1,0xff,0x80,
0xff,0xf8,0x3e,0x0,0x1f,0x7,0xc1,0xf7,0xc7,0xc0,0xf,0x80,
0xff,0xe0,0x3f,0x0,0x3f,0x7,0xc1,0xf7,0xc7,0xe0,0xf,0x80,
0xf8,0x0,0x1f,0x0,0x3e,0x7,0xc0,0xff,0xc3,0xe0,0xf,0x80,
0xf8,0x0,0x1f,0x80,0x7e,0x7,0xc0,0x7f,0xc3,0xf0,0xf,0x80,
0xf8,0x0,0x1f,0xc0,0xfc,0x7,0xc0,0x7f,0xc3,0xfc,0xf,0x80,
0xf8,0x0,0xf,0xff,0xfc,0x7,0xc0,0x3f,0xc1,0xff,0xff,0x80,
0xf8,0x0,0x7,0xff,0xf8,0x7,0xc0,0x3f,0xc0,0xff,0xff,0x80,
0xf8,0x0,0x3,0xff,0xf0,0x7,0xc0,0x1f,0xc0,0x7f,0xff,0x80,
0xf8,0x0,0x1,0xff,0xe0,0x7,0xc0,0xf,0xc0,0x3f,0xff,0x0,
0xf8,0x0,0x0,0x7f,0x0,0x7,0xc0,0xf,0xc0,0x7,0xf8,0x0
};

#define bot_1_up 22 
#define bot_1_dw 23 

#define bot_2_up 26 
#define bot_2_dw 27 

/// Definimos constantes para el color de la pantalla///
#define NEGRO           0x0000 
#define AZUL            0x001F 
#define ROJO             0xF800 
#define VERDE           0x07E0 
#define CIAN            0x07FF 
#define MAGENTA         0xF81F 
#define AMARILLO          0xFFE0   
#define BLANCO           0xFFFF 

/// Definimos los pins de la pantalla a color ///
#define sclk 7
#define mosi 5
#define cs   4
#define rst  3
#define dc   2
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);

///Display 7 Segmentos Catodo Común///
int contador[10]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67};
byte a=32;
byte b=33;
byte c=34;
byte d=35;
byte e=36;
byte f=37;
byte g=38;


byte h=42; 
byte i=43; 
byte j=44; 
byte k=45; 
byte l=46; 
byte m=47; 
byte n=48; 

///Modificando estas contstantes podremos cambiar la dificultad///
const unsigned long VEL_PALA = 33;
const unsigned long VEL_PELOT= 50;
const uint8_t ALT_PALA = 24;
int PUNT_MAX = 2;

///Definimos las funciones que vamos a usar///
void choqueParedHorizontal ();
void choqueParedVertical ();
void choquePala ();

void Reset ();
void resetPelota ();
void resetJugadores();
void setJugadores ();

void dibujaPelota();
void dibujarPista();

void finJuego();
void marcador();


///Usaremos struct para facilitar el código///
/*Vector2D nos permite usar unas 
//coordenadas x,y para la posición de la pelota y el jugador*/
struct Vector2D {
  uint8_t x;
  uint8_t y;
};

/*Jugador tiene 3 variables, la posición, la puntuación 
 y el refrresco, que equivale a la velocidad que lleva.*/
struct jugador 
{
  struct Vector2D pos;
  int punt;
  long refre; 
};

//Añadimos la variable dirección a la pelota
struct pelota
{
  struct Vector2D pos;
  struct Vector2D dir;
  long refre; 
};

//Inicializamos las estructuras
//Creamos dos pelotas;
pelota pel_ini; //{{64,32},{0,0},0}; //Se crea al ppio y cada vez que hacemos un punto
pelota pel; // {{0,0},{1,1},0}; //Pelota que usamos mientras estamos jugando

//Creamos dos jugadores 
jugador jug1 {{12,16},0,0}; //Jugador de la izda
jugador jug2 {{85,16},0,0}; //dcha

///Haremos uso de dos variables booleanas para saber en que momento del juego nos encontramos///      
boolean jugando = true; //gameisOn
//boolean resetPelota = false; //resetPelota


///Configuramos los pines y llamamos a las funciones iniciales///
void setup() 
{
    //Configuramos los 16 Pines digitales que usamos para los display de 7 segmentos como SALIDAS
    for(int i=a;i<=n;i++)
    {
      pinMode(i,OUTPUT);
    }

    //Configuramos los botones que usamos para controlar los movimientos
    pinMode(bot_1_up, INPUT_PULLUP);
    pinMode(bot_1_dw, INPUT_PULLUP);
    pinMode(bot_2_up, INPUT_PULLUP);
    pinMode(bot_2_dw, INPUT_PULLUP);

    //Inicamos la pantalla y ponemos el mensaje inicial
    display.begin();
    display.fillScreen(NEGRO);
    display.drawBitmap(3,20,pong,89,24,BLANCO);

    //dejamos un tiempo de margen para iniciar el juego
    while(digitalRead(bot_2_up) == HIGH && digitalRead(bot_2_dw) == HIGH)  
    {
      delay(100);
    }
    //Creamos una variable a la cual le asignamos el tiempo que llevamos   
    unsigned long comienzo = millis();

    //Llamamos a la funcion dibujarPista.
    display.fillScreen(NEGRO);
    dibujarPista();

    //Damos un tiempo hasta que empiece
    while(millis() - comienzo < 2000);

    /*Le asignamos a la pelota y a los jugadores una velocidad igual al tiempo,
    en realidad la variable vel es como una tasa de refresco, para que tanto pelota
    como jugador vayan a la misma velocidad*/
    pel.refre = millis();
    jug1.refre = pel.refre;
    jug2.refre = pel.refre;
    
    //Tras hacer todo esto llamamos a las funciones setJuego y Reset para comenzar
    setJuego();
    Reset();    
}

//Comenzamos con la función loop que se va a repetir constantemente
void loop() 
{
  //Nos interesea saber el estado de los botones, por ahora le asignamos un falso booelano
  unsigned long time = millis(); 
  
  //Mientras estemos jugando queremos llamar a las siguientes funciones
  if(time > pel.refre && jugando)
  {
    choqueParedHorizontal ();
    choqueParedVertical ();
    choquePala ();
    dibujaPelota (); 
  }

  //Llamamos a las funciones que nos permiten mover a los jugadores
  if (time > jug1.refre && jugando) 
  {
    moverJug1();
  }
  if (time > jug2.refre && jugando)
  {
    moverJug2();  
  }
}

/////////////////////////////////////////////////////////////////
    ////////////////DEFINICIÓN DE FUNCIONES////////////////
/////////////////////////////////////////////////////////////////

////////////////FUNCIONES SET////////////////

void setJuego ()
{
  setJugadores ();
  setPelotas ();
}

//Le damos valores inciales a los jugadores
void setJugadores ()
{
  jug1.pos.x = 12;
  jug1.pos.y = 16;

  jug2.pos.x = 85;
  jug2.pos.y = 16;

}

//Y también a la pelota_inicial
void setPelotas ()
{
  pel_ini.pos.x = random(25,70); 
  pel_ini.pos.y = random(3,63);
}

////////////////FUNCIONES RESET////////////////
///Llamamos a resetPelota y volvemos a dibujarPista
void Reset ()
{
  resetPelota ();
  dibujarPista();
}

///Cada vez que marquemos un punto o se inicie el juego
void resetPelota ()
{ 
  do
  {
    pel.pos.x = 42;// en el centro de la pantalla
    pel.dir.x = random(-1,2);
    }while(pel.dir.x==0);
    do
    {
      pel.pos.y = 32; //a media altura
      pel.dir.y = random(-1,2);
    }while(pel.dir.y==0);
}

///////////COMPROBAMOS CHOQUES CON PAREDES///////////

///Esta funcion comprueba si hemos chocado con el suelo o el techo///
void choqueParedHorizontal ()
{
  //Se va cambiando la posicion y de la pelota
  pel.pos.y = pel_ini.pos.y + pel.dir.y;
  
  if(pel.pos.y == 0 || pel.pos.y == 63) 
  {
    pel.dir.y = -pel.dir.y;
    pel.pos.y += pel.dir.y + pel.dir.y;
  } //Simplimente le cambiamos el sigo a la dirección que llevaba
  dibujarPista();
}

///Vemos si la pelota llega a los limites verticales///
void choqueParedVertical ()
{
  //Se va cambiando la posicion x de la pelota
  pel.pos.x = pel_ini.pos.x + pel.dir.x;
  
  if(pel.pos.x == 1) //Ha llegado a la izda --> Jug2 tiene un punto
  {
    jug2.punt++;
    //Llamamos a la función puerto que muestra el display de 7 segmentos del jug2
    puerto(contador[jug2.punt],h,n);

    //Vemos si hemos alcanzado la puntuación máxima
    if(jug2.punt==PUNT_MAX)
    {
      finJuego();
    }
    else
    {
      marcador();
      resetPelota();
    }
      
  }
  
  if(pel.pos.x == 95) //Ha llegado a la dcha --> Jug1 tiene un punto
  {
    jug1.punt++; 
    puerto(contador[jug1.punt],a,g);
    if(jug1.punt==PUNT_MAX)
      finJuego();
    else
    {
      marcador();
      resetPelota();
    }
      
  }
}

///////////COMPROBAMOS CHOQUES CON LAS PALAS///////////
void choquePala ()
{
  if(pel.pos.x == jug1.pos.x //Pala de la izda
  && pel.pos.y >= jug1.pos.y  //tenemos que comprobar tanto las x como las y
  && pel.pos.y <= jug1.pos.y + ALT_PALA) //También hay que tener en cuenta la altura de la pala
  {
    pel.dir.x = -pel.dir.x; //en el caso de que choquen cambiamos de direccion
    pel.pos.x += pel.dir.x + pel.dir.x;
  }

  // De forma análoga, para el jug2:     
  if(pel.pos.x == jug2.pos.x 
  && pel.pos.y >= jug2.pos.y 
  && pel.pos.y <= jug2.pos.y + ALT_PALA)
  {
    pel.dir.x = -pel.dir.x;
    pel.pos.x += pel.dir.x + pel.dir.x;
  }   
}

///////////FUNCIONES DE MOVIMIENTO///////////

///Función que define el moviento de los jugadores, análogo para el 1 y el 2///
void moverJug1 ()
{
  //Nos interesea saber el estado de los botones, por ahora le asignamos un falso booelano
  static bool arriba_1 = false;
  static bool abajo_1 = false;

  //Igualamos esta variable a la lectura de los botones
  arriba_1 |= (digitalRead(bot_1_up) == LOW);
  abajo_1 |= (digitalRead(bot_1_dw) == LOW);

  /*Igualamos la tasa de refresco del jugador a la velocidad de la pala, para que vayan los dos 
  a la misma velocidad y evitar problemas*/
  jug1.refre += VEL_PALA;
  //Vamos "borrando" la pala, para ello la pintamos de negro que es el color de fondo
  display.drawFastVLine(jug1.pos.x, jug1.pos.y,ALT_PALA, NEGRO);

  //Que hacemos cuando pulsamos cada botón
  if(arriba_1) 
    jug1.pos.y += 1;
  if(abajo_1) 
    jug1.pos.y -= 1;
    
  //Le ponemos un tope para que no desaparezca de la pantalla  
  arriba_1 = abajo_1 = false;
  if(jug1.pos.y < 1) 
    jug1.pos.y = 1;
  if(jug1.pos.y + ALT_PALA > 63) 
    jug1.pos.y = 63 - ALT_PALA;

  //Dibujamos la pala, en este caso de color rojo
  display.drawFastVLine(jug1.pos.x, jug1.pos.y, ALT_PALA, ROJO);  
}
///Hacemos lo mismo para el jugador2///
void moverJug2 ()
{
  static bool arriba_2 = false;
  static bool abajo_2 = false;
  arriba_2 |= (digitalRead(bot_2_up) == LOW);
  abajo_2 |= (digitalRead(bot_2_dw) == LOW);

  jug2.refre += VEL_PALA;
  display.drawFastVLine(jug2.pos.x, jug2.pos.y,ALT_PALA, NEGRO);
  
  if(arriba_2) 
    jug2.pos.y += 1;
  if(abajo_2)
    jug2.pos.y -= 1; 
      
  arriba_2 = abajo_2 = false;
  if(jug2.pos.y < 1) 
    jug2.pos.y = 1;
  if(jug2.pos.y + ALT_PALA > 63) 
    jug2.pos.y = 63 - ALT_PALA;

  display.drawFastVLine(jug2.pos.x, jug2.pos.y, ALT_PALA, VERDE);
}

///////////FUNCIONES DE DIBUJO///////////


void dibujaPelota ()
{
  display.drawPixel(pel_ini.pos.x, pel_ini.pos.y, NEGRO); //Borramos la pelota, pintándola de negro
  display.drawPixel(pel.pos.x, pel.pos.y, BLANCO); //Se refresca y pintamos la nueva pos de blanco

  //Vamos cambiando la posición de la pelota
  pel_ini.pos.x = pel.pos.x; 
  pel_ini.pos.y = pel.pos.y;

  pel.refre += VEL_PELOT;
  
}
//Un simple reborde que delimita la pista
void dibujarPista() 
{
    display.drawRect(0, 0, 96, 64, CIAN);
}

///////////FUNCIONES PARA EL MARCADOR///////////
/*Función que coloca en el puerto de salida los bits comenzando
desde el pin ini hasta el pin fin*/
void puerto(int bits,int ini,int fin)
{
  for(int i=ini;i<=fin;i++)
  {
    digitalWrite(i,bitRead(bits,i-ini));
  }
}

//Dibujamos tanto el marcador en la pantalla como en el display de 7seg
void marcador()
{
  //Llamamos a la función puerto de arriba
  puerto(contador[jug1.punt],a,g);
  puerto(contador[jug2.punt],h,n);
  
  //Paramos el juego mientras mostramos el marcador
  jugando = false;
  display.fillScreen(NEGRO);
  dibujarPista();

  //Funciones de la libreria Adafruit.h que nos permiten dibujar en la pantalla
  display.setCursor(8,4);
  display.setTextColor(BLANCO);
  display.setTextSize(1);
  display.print("  Puntuacion");

  display.setCursor(20,30);
  display.setTextColor(ROJO);
  display.setTextSize(3);
  display.print(String(jug1.punt));

  display.setCursor(60,30);
  display.setTextColor(VERDE);
  display.setTextSize(3);
  display.print(String(jug2.punt));

  //Lo mostramos durante 2segs y volvemos al comienzo
  delay(2000);
  unsigned long comienzo = millis();

  //Llamamos a la funcion dibujarPista.
  display.fillScreen(NEGRO);
  dibujarPista();
  
  //Damos un tiempo hasta que empiece
  while(millis() - comienzo < 2000);
  
  /*Le asignamos a la pelota y a los jugadores una velocidad igual al tiempo,
    en realidad la variable vel es como una tasa de refresco, para que tanto pelota
    como jugador vayan a la misma velocidad*/
  pel.refre = millis();
  jug1.refre = pel.refre;
  jug2.refre = pel.refre;    
  jugando = true;
  resetPelota();
}
//////////////FUNCION FIN DE JUEGO/////////////////////

void finJuego()
{
  //Paramos el juego
  jugando = false;
  display.fillScreen(NEGRO);
  dibujarPista();

  //Si ha ganado el jugador 2
  if(jug2.punt>jug1.punt)
  {
    display.setCursor(15,4); 
    display.setTextColor(VERDE);
    display.setTextSize(2);
    display.print("Gana 2");  
  }
  else //ha ganado el jug1
  {
    display.setCursor(15,4);
    display.setTextColor(ROJO);
    display.setTextSize(2);
    display.print("Gana 1"); 
  }

  //Mostramos la puntuación:
  display.setCursor(20,30);
  display.setTextColor(ROJO);
  display.setTextSize(3);
  display.print(String(jug1.punt));

  display.setCursor(60,30);
  display.setTextColor(VERDE);
  display.setTextSize(3);
  display.print(String(jug2.punt));

  //Esperamos 2segs
  delay(2000);

  //dejamos un tiempo de margen para iniciar el juego
  while(digitalRead(bot_2_up) == HIGH && digitalRead(bot_2_dw) == HIGH)  
  {
    delay(100);
  }
  
  //Volvemos a inicial el bool de jugando
  jugando = true;

  //Reseteamos las puntaciones
  jug1.punt = jug2.punt = 0;

  //Creamos una variable a la cual le asignamos el tiempo que llevamos   
  unsigned long comienzo = millis();

  //Llamamos a la funcion dibujarPista.
  display.fillScreen(NEGRO);
  dibujarPista();

  //Damos un tiempo hasta que empiece
  while(millis() - comienzo < 2000);
  
  pel.refre = millis(); 
  
  resetPelota();
}
