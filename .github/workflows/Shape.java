package finaltest;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

/*그림 도형 클래스들*/
class Shape extends JPanel{ //도형들의 상위 클래스
	int x;
	int y;
	boolean cpshow=false;
	ControlPoint cp;
	Shape(int x,int y){
		this.x=x;
		this.y=y;
	}
	public void draw(Graphics g) {} //메소드 오버라이딩을 위해 메소드 구현만
}
class Rectangle extends Shape{
	int width;
	int height;
	Rectangle(int x,int y,int width,int height){
		super(x,y);
		this.width=width;
		this.height=height;
		cp=new ControlPoint(x,y,x+width,y+height);
		MSListener mslistener=new MSListener();
		addMouseListener(mslistener);
		addMouseMotionListener(mslistener);
	}
	public void draw(Graphics g) {
		g.drawRect(super.x,super.y,width,height);
		if(cpshow) {
			cp.draw(g);
		}
	}
	private class MSListener extends MouseAdapter{
		public void mousePressed(MouseEvent e) {
			cpshow=true;
			repaint();
		}
	}
}
class Line extends Shape{
	int x2;
	int y2;
	Line(int x,int y,int x2,int y2){
		super(x,y);
		this.x2=x2;
		this.y2=y2;
		cp=new ControlPoint(x,y,x2,y2);
	}
	public void draw(Graphics g) {
		g.drawLine(super.x, super.y, x2, y2);
		cp.draw(g);
	}
}
class Oval extends Shape{
	int width;
	int height;
	Oval(int x,int y,int width,int height){
		super(x,y);
		this.width=width;
		this.height=height;
		cp=new ControlPoint(x,y,x+width,y+height);
	}
	public void draw(Graphics g) {
		g.drawOval(super.x, super.y, width, height);
		cp.draw(g);
	}
}
class ControlPoint extends Shape{
	int x2;
	int y2;
	ControlPoint(int x,int y,int x2,int y2){
		super(x,y);
		this.x2=x2;
		this.y2=y2;
	}
	public void draw(Graphics g) {
		g.drawRect(super.x-2,super.y-2,4,4);
		g.drawRect(x2-2,y2-2,4,4);
	}
}
