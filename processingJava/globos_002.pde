// Luis Molina-Tanco - 2021.
//
// Ejemplo uso puerto serie.
//
// Este programa está pensado para funcionar con un arduino
// que esté ejecutando el ejemplo Knock, que se puede
// ver aquí:
//
// https://www.arduino.cc/en/Tutorial/BuiltInExamples/Knock

import processing.serial.*;
ArrayList<Globo> globos;
Serial myPort;
PImage img;

void setup()
{
  fullScreen();
  background(255, 255, 255);
  img = loadImage("cuboTrans.png");
  globos = new ArrayList<Globo>();
  //printArray(Serial.list());
  //myPort = new Serial(this, Serial.list()[0], 9600);
  //myPort.bufferUntil(10);
  //myPort.clear();
}

void draw()
{
  background(255, 255, 255);
  for (int i = 0; i<globos.size(); i++)
  {
    Globo g = globos.get(i);
    g.update();
    g.draw();

    
  }
  
  image(img, 750.0, 575.0);
  //line(765, 690,825, height-50);
  fill(0,0,0);
  rect(300, 0, 10, width);
  rect(width-300, 0, 10, width);
}

void mousePressed()
{
  creaGlobo(mouseX, mouseY);
}

//void serialEvent(Serial p)
//{
//  String datoS = p.readString();
//  datoS = datoS.trim();
  //println(datoS);
//  if (datoS.equals("Knock!"))
 // {
   // creaGlobo(int(random(0, width)), int(random(height/2, height)));
  //}
//}

void creaGlobo(int x, int y)
{
  globos.add(new Globo(random(100, 150),
    color(random(0,255), random(0,255), random(0,255)),
    x,
    y,
    random(-5, 5),
    random(0.2, 3)));
  println(globos.size());
}
