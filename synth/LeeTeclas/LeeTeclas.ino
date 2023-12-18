//CODIGO EN EL QUE SE HACE UN BARRIDO POR TODAS LAS TECLAS Y SE ALMACENAN EN UN ARRAY PARA SU POSTERIOR TRATAMIENTO
//HECHO POR JUAN CARLOS GOMEZ Y NOE PANIAGUA PARA LA ASIGNATURA DE ELECTRONICA CREATIVA

//PINES QUE VAN A USARSE
#define SALIDA1_TECLAS 10
#define SALIDA2_TECLAS 12
#define SELECTOR2 6
#define SELECTOR1 5
#define SELECTOR0 4


<<<<<<< HEAD
  //VARIABLES PARA EL CODIGO
uint8_t array_teclas[15]; //Array en el que se almacenan los datos
unsigned long tiempo = 0; // Almacen del tiempo del programa
int periodo = 100; //Cada cuanto se toma una muestra
=======
//VARIABLES PARA EL CODIGO
uint8_t array_teclas[15]; //Array en el que se almacenan los datos
unsigned long tiempo = 0; // Almacen del tiempo del programa
int periodo = 500; //Cada cuanto se toma una muestra
>>>>>>> de11f4aa627609dbbf87cfe337fc0cafefd90944
int ADOS = 0; //Variable A2 del multiplexor A[3:0]
int AUNO = 0; //Variable A1 del multiplexor A[3:0]
int ACERO = 0; //Variable A0 del multiplexor A[3:0]
bool semaforo = false; //Semaforo para la depuracion

//SETUP
void setup() {
  //Configuracion de los pines en los modos correspondientes
  pinMode(SALIDA1_TECLAS, INPUT);
  pinMode(SALIDA2_TECLAS, INPUT);
  pinMode(SELECTOR2, OUTPUT);
  pinMode(SELECTOR1, OUTPUT);
  pinMode(SELECTOR0, OUTPUT);
<<<<<<< HEAD
=======
  
>>>>>>> de11f4aa627609dbbf87cfe337fc0cafefd90944
}

//LOOP
void loop() {
<<<<<<< HEAD
=======


>>>>>>> de11f4aa627609dbbf87cfe337fc0cafefd90944
  //FUNCION QUE ALMACENA LOS DATOS EN EL ARRAY DESIGNADO PARA ELLO
  if(millis() >tiempo + periodo){
    if(ADOS == 0){
      digitalWrite(SELECTOR2,LOW); //Pongo A2 a 0
<<<<<<< HEAD
//      Serial.println(AUNO);
//      Serial.println(ACERO);
=======
>>>>>>> de11f4aa627609dbbf87cfe337fc0cafefd90944
      if(AUNO == 0){
        digitalWrite(SELECTOR1,LOW); //Pongo A1 a 0
        if(ACERO == 0){
          digitalWrite(SELECTOR0,LOW); //Pongo A0 a 0
<<<<<<< HEAD
          
          //Lectura de los valores de salida del multiplexor.
          bool entrada1 = digitalRead(SALIDA1_TECLAS);
          bool entrada2 = digitalRead(SALIDA2_TECLAS);
          
=======
          //Lectura de los valores de salida del multiplexor.
          bool entrada1 = digitalRead(SALIDA1_TECLAS);
          bool entrada2 = digitalRead(SALIDA2_TECLAS);

>>>>>>> de11f4aa627609dbbf87cfe337fc0cafefd90944
          ACERO = 1;
          tiempo = millis(); //Cojo el tiempo que lleva el programa en ejecucion
          //Compruebo valores obtenidos y los guardo en el array
          if(entrada1 == HIGH){
            array_teclas[0] = 1;
          }
          else{
            array_teclas[0] = 0;
          }
          if(entrada2 == HIGH){
            array_teclas[8] = 1;
          }
          else{
            array_teclas[8] = 0;
          }
        } //if A0 == 0 [0,0,0]
        else{
          digitalWrite(SELECTOR0,HIGH); // A0 a 1
          bool entrada1 = digitalRead(SALIDA1_TECLAS);
          bool entrada2 = digitalRead(SALIDA2_TECLAS);
          ACERO = 0;
          AUNO = 1;
          tiempo = millis();
          if(entrada1 == HIGH){
            array_teclas[2] = 1;
          }
          else{
            array_teclas[2] = 0;
          }
          if(entrada2 == HIGH){
            array_teclas[10] = 1;
          }
          else{
            array_teclas[10] = 0;
          }
        } //else A0 == 1 [0,0,1]
      } //if A1 == 0 
      else{
        digitalWrite(SELECTOR1,HIGH); //A1 a 1
        if(ACERO == 0){
          digitalWrite(SELECTOR0,LOW);
          bool entrada1 = digitalRead(SALIDA1_TECLAS);
          bool entrada2 = digitalRead(SALIDA2_TECLAS);
          ACERO = 1;
          tiempo = millis();
          if(entrada1 == HIGH){
            array_teclas[4] = 1;
          }
          else{
            array_teclas[4] = 0;
          }
          if(entrada2 == HIGH){
            array_teclas[12] = 1;
          }
          else{
            array_teclas[12] = 0;
          }
        } //if A0 == 0 [0,1,0]
        else{
          digitalWrite(SELECTOR0,HIGH);
          bool entrada1 = digitalRead(SALIDA1_TECLAS);
          bool entrada2 = digitalRead(SALIDA2_TECLAS);
          ACERO = 0;
          AUNO = 0;
          ADOS = 1;
          tiempo = millis();
          if(entrada1 == HIGH){
            array_teclas[6] = 1;
          }
          else{
            array_teclas[6] = 0;
          }
          if(entrada2 == HIGH){
            array_teclas[14] = 1;
          }
          else{
            array_teclas[14] = 0;
          }
        } //else A0 == 1 [0,1,1] 
      }//else A1 = 1
    } //if A2 == 0 
    else{
      digitalWrite(SELECTOR2,HIGH); // A2 a 1
      if(AUNO == 0){
        digitalWrite(SELECTOR1,LOW);
        if(ACERO == 0){
          digitalWrite(SELECTOR0,LOW);
<<<<<<< HEAD
          
=======
>>>>>>> de11f4aa627609dbbf87cfe337fc0cafefd90944
          bool entrada1 = digitalRead(SALIDA1_TECLAS);
          bool entrada2 = digitalRead(SALIDA2_TECLAS);
          ACERO = 1;
          tiempo = millis();
          if(entrada1 == HIGH){
            array_teclas[1] = 1;
          }
          else{
            array_teclas[1] = 0;
          }
          if(entrada2 == HIGH){
            array_teclas[9] = 1;
          }
          else{
            array_teclas[9] = 0;
          }
        } //if A0 == 0 [1,0,0]
        else{
          digitalWrite(SELECTOR0,HIGH);
          bool entrada1 = digitalRead(SALIDA1_TECLAS);
          bool entrada2 = digitalRead(SALIDA2_TECLAS);
          ACERO = 0;
          AUNO = 1;
          tiempo = millis();
          if(entrada1 == HIGH){
            array_teclas[3] = 1;
          }
          else{
            array_teclas[3] = 0;
          }
          if(entrada2 == HIGH){
            array_teclas[11] = 1;
          }
          else{
            array_teclas[11] = 0;
          }
        } //else A0 == 1 [1,0,1]
      } //if A1 == 0 
      else{
        digitalWrite(SELECTOR1,HIGH);
        if(ACERO == 0){
          digitalWrite(SELECTOR0,LOW);
          bool entrada1 = digitalRead(SALIDA1_TECLAS);
          bool entrada2 = digitalRead(SALIDA2_TECLAS);
          ACERO = 1;
          tiempo = millis();
          if(entrada1 == HIGH){
            array_teclas[5] = 1;
          }
          else{
            array_teclas[5] = 0;
          }
          if(entrada2 == HIGH){
            array_teclas[13] = 1;
          }
          else{
            array_teclas[13] = 0;
          }
        }//if A0 == 0 [1,1,0]
        else{
          digitalWrite(SELECTOR0,HIGH);
          bool entrada1 = digitalRead(SALIDA1_TECLAS);
          bool entrada2 = digitalRead(SALIDA2_TECLAS);
          ACERO = 0;
          AUNO = 0;
          ADOS = 0;
          semaforo = true; // Pongo semaforo activo porque array lleno entonces puedo leer todos los valores sin error
          if(entrada1 == HIGH){
            array_teclas[7] = 1;
          }
          else{
            array_teclas[7] = 0;
          }
          if(entrada2 == HIGH){
            array_teclas[15] = 1;
          }
          else{
            array_teclas[15] = 0;
          }
        } //else A0 == 1 [1,1,1]
      } //else A1 == 1 
    } //else A2 == 1 
  }//if milis

//FUNCION PARA MOSTRAR LOS VALORES DEL ARRAY POR PANTALLA PARA COMPROBAR EL CORRECTO FUNCIONAMIENTO
  if(semaforo == true){
    for(int i = 0; i<13; i++){
      if(array_teclas[i] == 1){
        Serial.println("Tecla pulsada");
      }
      else{
         Serial.println("Tecla no pulsada");
      }
    }
<<<<<<< HEAD
    Serial.println("------------------------------------------------------------");
    semaforo = false;
    tiempo = millis();
    ACERO = 0;
    AUNO = 0;
    ADOS = 0;
=======
    //Serial.println("------------------------------------------------------------");
    semaforo = false;
    tiempo = millis();
>>>>>>> de11f4aa627609dbbf87cfe337fc0cafefd90944
  }
  
}