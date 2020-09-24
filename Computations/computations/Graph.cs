using System;
using System.Windows.Forms;
using System.Drawing;

namespace computations
{
	public class Graph : Form
	{
		protected PictureBox pictureBox1;
		protected float x, y, Centerx, Centery,
			    	  PaddingTop, PaddingBottom, PaddingLeft, PaddingRight,
			    	  ContentWidth, ContentHeight;
		public Pen pen;
		protected Bitmap bmp;
		protected Graphics g;
		protected Font font = new Font("Calibri", 10, FontStyle.Bold);
		public Graph(double a = -5, double b = 5) 
		{
			Centery = 300;
			PaddingTop = 10;
			PaddingBottom = 10;
			PaddingLeft = 10;
			PaddingRight = 10;
			ContentWidth = 500 - PaddingLeft - PaddingRight;
			ContentHeight = 500 - PaddingTop - PaddingBottom;
			BackColor = SystemColors.Window;
			ClientSize = new Size(500, 500);
			Text = "График";
			pictureBox1 = new PictureBox();
			pictureBox1.Dock = DockStyle.Fill;
			pictureBox1.Size = new Size(500, 500);
			Controls.Add(pictureBox1);
			bmp = new Bitmap(500, 500);
			pen = new Pen(Color.Black,2);
			g = Graphics.FromImage(bmp);
			Centerx = PaddingLeft + (float)(a < 0?  -a * ContentWidth / (b - a) : 0);
			g.DrawLine(pen,Centerx,PaddingTop,Centerx,PaddingTop + ContentHeight);//y
			g.DrawLine(pen,PaddingLeft,Centery,PaddingLeft+ContentWidth,Centery);//x
			g.DrawLine(pen, Centerx, PaddingTop, Centerx-5,PaddingTop+5);
			g.DrawLine(pen, Centerx, PaddingTop, Centerx+5,PaddingTop+5);
			g.DrawLine(pen, PaddingLeft+ContentWidth, Centery, PaddingLeft+ContentWidth-5, Centery-5);
			g.DrawLine(pen, PaddingLeft+ContentWidth, Centery, PaddingLeft+ContentWidth-5, Centery+5);
			g.DrawString("X",font,Brushes.Black,PaddingLeft+ContentWidth,Centery);
			g.DrawString("Y",font,Brushes.Black,Centerx+5,PaddingTop-5);
			float temp = ContentWidth / (float)(a < 0?  (b - a) : b);
			for(int i = (int)Math.Floor(a); i < b; i++)
			{
				g.DrawLine(pen, Centerx + i * temp,Centery-5, Centerx + i * temp, Centery+5);
				g.DrawString(string.Format("{0:0}",i),font,Brushes.Black,Centerx + i * temp,Centery+5);
			}
		}
	}
}
