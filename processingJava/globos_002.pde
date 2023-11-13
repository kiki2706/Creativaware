import processing.serial.*;
ArrayList<Globo> globos;
Serial myPort;
PImage img, pueblo;
float colorRojo = 0;
float colorVerde = 0;
float colorAzul = 0;
float posicionX = width/2;



void setup()
{
  fullScreen();
  background(0,0,0);
  img = loadImage("cuboTrans.png");
  pueblo = loadImage("pueblo.jpg");
  globos = new ArrayList<Globo>();
  
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil(10);
  myPort.clear();
}

void draw()
{
  for (int i = 0; i<globos.size(); i++)
  {
    Globo g = globos.get(i);
    g.update();
    g.draw();

    if(g.estoyFuera() == 1)
      globos.remove(i);
      
    if(g.limpiaPantalla() == 1){
      background(0,0,0);
      for(int j = 0; j<globos.size(); j++)
      globos.remove(j);
    }
    
    //Colores de relleno de las figuras
    if(g.cambiaColor() == 0){
       colorRojo = random(10,255);
       colorVerde = random(10,255);
       colorAzul = random(10,255);
    }
    else if(g.cambiaColor() == 1){
       colorRojo = random(50,200);
       colorVerde = 0;
       colorAzul = 0;
    }
    else if(g.cambiaColor() == 2){
       colorRojo = 0;
       colorVerde = random(50,200);
       colorAzul = 0;
    }
    else if(g.cambiaColor() == 3){
       colorRojo = 0;
       colorVerde = 0;
       colorAzul = random(50,200);
    }
    
  }
  
  //image(img, width/2, height/1.87);
  //line(765, 690,825, height-50);
  fill(0,0,0);
  //rect(300, 0, 10, width);
  //rect(width-300, 0, 10, width);
}

void mousePressed()
{
  creaGlobo(mouseX, mouseY);
}

void serialEvent(Serial p)
{
  String datoS = p.readString();
  datoS = datoS.trim();
  //println(datoS);
  if (datoS.equals("Knock!")){
    creaGlobo(int(random(300, width-300)), int(random(0, height/2)));
  }
}

void creaGlobo(int x, int y)
{
  float v = random(-5, 5);
  if(v>0)
    v+=2;
  else
    v-=2;
    
  globos.add(new Globo(random(100, 150),

    color(colorRojo, colorVerde, colorAzul),
    x,
    y,
    v,
    random(2, 3)));
  println(globos.size());
}
