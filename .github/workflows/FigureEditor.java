/*자바 프로그래밍 기말과제*/
package finaltest;

/*독립된 클래스들이지만 한 클래스내에 작성하였음*/
//자바 이벤트 관련 라이브러리 불러옴
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
/*FigureEditor은 마우스로 도형그리기 이벤트를 실행하는 메인클래스 입니다.*/
public class FigureEditor{  
	public static void main(String[] args) {
		new FigureEditorFrame();
	}
}
//FigureEditorFrame 클래스는 메인 컨테이너로, 패널들을 포함, 배치하는 클래스입니다.
class FigureEditorFrame extends JFrame{
	PanelA panelA; //PanelA에서 주된 이벤트가 발생하니 멤버변수로 하여 정보전달 가능하게
	FigureEditorFrame(){
		setTitle("기말 프로젝트");
		setSize(600,300);
		//패널 생성 및 배치 설정
		panelA=new PanelA();
		add(panelA,BorderLayout.CENTER);
		add(new PanelC(this),BorderLayout.WEST);//해당 객체를 넘김
		setVisible(true);
	}
}
//PanelA는 도형 그림이 그려지는 캔버스 역할을 하는 클래스로 마우스 이벤트가 정의되어있는 클래스입니다.
class PanelA extends JPanel{
	ArrayList<Shape> shapes=new ArrayList<Shape>(); //그림 도형 객체를 저장하는 객체 배열
	Point start=null,end=null; //시작과 종료 좌표에 대한 객체 변수
	String selectBtn; //눌려진 버튼의 정보 저장할 변수	
	PanelA(){
		setBackground(Color.YELLOW);
		//마우스 이벤트 리스너 설정
		MPListener mplistener=new MPListener();
		addMouseListener(mplistener);
		addMouseMotionListener(mplistener);
	}
	//내부 클래스 이벤트 리스너-마우스 이벤트
	private class MPListener extends MouseAdapter{
		public void mouseClicked(MouseEvent e) { //클릭 할 때
			start=e.getPoint();
			switch(selectBtn) { //선택된 버튼 값에 맞게 객체 생성 및 저장
	        case "사각":
	        	Shape rect=new Rectangle(start.x,start.y,50,50);
				shapes.add(rect); //shape객체 배열에 저장>>업 캐스팅
	        	break;
	        case "직선": //다른 도형과 다르게 너비높이가 아니라 시작과 끝점
	        	Shape line=new Line(start.x, start.y, start.x+50, start.y+50);
	        	shapes.add(line);//shape객체 배열에 저장>>업 캐스팅
	        	break;
	        case "타원":
	        	Shape oval=new Oval(start.x,start.y,50,50);
	        	shapes.add(oval);//shape객체 배열에 저장>>업 캐스팅
	        	break;
	        }
			repaint();
		}
		public void mouseDragged(MouseEvent e) { //드래그 할 때 
			
		}
		public void mouseReleased(MouseEvent e) { //마우스를 뗄 때
			
			
		}
	}
	public void paintComponent(Graphics g) { //도형 그리기
        super.paintComponent(g);
        if(start==null) //(=마우스 이벤트 처리가 없었다면 그리지 않고 종료
        	return;
//      //값들을 적절히 수정하여 저장
//        int x=Math.min(start.x, end.x);
//        int y=Math.min(start.y, end.y);
//        int width=Math.abs(start.x-end.x);
//        int height=Math.abs(start.y-end.y);
//        switch(selectBtn) { //선택된 버튼 값에 맞게 그림그리기
//        case "사각":
//        	g.drawRect(x, y, width, height);
//        	break;
//        case "직선":
//        	g.drawLine(start.x, start.y, end.x, end.y);
//        	break;
//        case "타원":
//        	g.drawOval(x, y, width, height);
//        	break;
//        }
        
        for(int i=0;i<shapes.size();i++) { //배열 내에 저장된 도형 전체 출력
        	Shape shape=shapes.get(i);
        	shape.draw(g);
        }
    }
}
//PanelB는 버튼 이벤트를 처리하는 클래스입니다
class PanelB extends JPanel{
	FigureEditorFrame Tframe; //멤버변수로 메인 프레임 클래스를 선언
	PanelB(FigureEditorFrame Tframe){//프레임 객체의 멤버 정보 전달받음
		this.Tframe=Tframe; 
		setLayout(new GridLayout(7,1,5,5));
		setBackground(Color.BLUE);
		JButton btn1=new JButton("사각");
		JButton btn2=new JButton("직선");
		JButton btn3=new JButton("타원");
		JButton btn4=new JButton("복사");
		JButton btn5=new JButton("삭제");
		JButton btn6=new JButton("저장");
		JButton btn7=new JButton("불러오기");
		//각 버튼에 이벤트 리스너 더함
		BtnActionListener btnclick=new BtnActionListener();
		btn1.addActionListener(btnclick);
		btn2.addActionListener(btnclick);
		btn3.addActionListener(btnclick);
		btn4.addActionListener(btnclick);
		btn5.addActionListener(btnclick);
		btn6.addActionListener(btnclick);
		btn7.addActionListener(btnclick);
		add(btn1);
		add(btn2);
		add(btn3);
		add(btn4);
		add(btn5);
		add(btn6);
		add(btn7);
	}
	//내부 클래스 이벤트 리스너-액션 이벤트
	private class BtnActionListener implements ActionListener{
		public void actionPerformed(ActionEvent e) {
			JButton btn=(JButton)e.getSource();
			Tframe.panelA.selectBtn=btn.getText();//이벤트 발생 버튼의 텍스트로 변경 
		}
	}
}
class PanelC extends JPanel{
	PanelC(FigureEditorFrame Tframe){
		add(new PanelB(Tframe));
	}
}
