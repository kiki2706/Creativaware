class Globo
{
  float _d, _x, _y, _vx, _vy;
  color _col;
  int[] numLados = {3,4,5,6,7,8,9,10,11,12,13,14};
  int tipoPoligono = (int)random(0,10);
  float gravedad = 0.3, rozy =0.005;
  PImage img = loadImage("cubo.jpg");
  Globo(float diametro, color col, float x, float y, float vx, float vy)
  {
    _d = diametro;
    _col = col;
    _x = x;
    _y = y;
    _vx = vx;
    _vy = vy;
  }
  void update()
  { 
    //CONTROL DE LA VELOCIDAD Y
    _vy += gravedad;//Añadimos la gravedad
    
      if(_vy > 0)//Aplicamos el rozamiento
      _vy -= rozy;
      else 
      _vy += rozy;
      if(_y > height){//Cada vez que rebotamos incrementamos el roz y cambiamos la direccion
        _vy *= -1; 
        rozy *=1.5;      }
      
    //CONTROL DE LA VELOCIDAD DE X
    if(_x > width || _x < 0)
      _vx *= -1;
    
    //SETEAMOS LAS POSCICIONES CON LAS VELOCIDADES
    _x = _x + _vx;
    _y = _y + _vy ;
    
    //CREACION DE VIENTOS PULSANDO TECLAS
//    if(keyPressed){
//      if(key == 'w'){//ARRIBA
//        for(int i= 0;i< 10; i++){
//        _vy = _vy - 0.09;
//        }
//      }
//     else if(key == 'd'){//DERECHA
//        for(int i= 0;i< 10; i++){
//        _vx = _vx + 0.03;
//        }
//      }
//      else if(key == 'a'){//IZQUIERDA
//        for(int i= 0;i< 10; i++){
//        _vx = _vx - 0.03;
//        }
//     }
//      else if(key == 's'){//ABAJO
//        for(int i= 0;i< 10; i++){
//        _vy = _vy + 0.09;
//        }
//      }
//   }
  

  }
  void draw()
  {
    push();
    fill(_col);
    translate(_x, _y);
    polygon(numLados[tipoPoligono],40.0);
    
    pop();
  }
  
  void polygon(int sideCount, float radio){
    float theta = 0.0;
    float x = 0.0;
    float y = 0.0;
  
    beginShape();
    for(int i = 0; i < sideCount; i++){
      x = cos(theta) * radio;
      y = sin(theta) * radio;
      vertex(x,y);
      theta += TWO_PI/sideCount;
    }
    endShape(CLOSE);
  }
}
