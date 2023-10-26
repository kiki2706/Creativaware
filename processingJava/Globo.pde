class Globo
{
  float _d, _x, _y, _vx, _vy;
  color _col;
  int[] numLados = {3,4,5,6,7,8,9,10,11,12,13,14};
  int tipoPoligono = (int)random(0,10);
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
    _vy -= random(0.02, 0.05);
    _vx += random(-0.1, 0.1);
    _x = _x + _vx;
    _y = _y + _vy;
  }
  void draw()
  {
    push();
    fill(_col);
    translate(_x, _y);
    polygon(tipoPoligono,100.0);
    translate(0,_d/2+5);
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
