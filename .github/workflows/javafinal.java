public boolean include(Point p) {
		int x2=width+super.x;
		int y2=height+super.y;
		return super.include(p)&&x2>=p.x&&y2>=p.y;
	}
