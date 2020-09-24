using System;
using System.Windows.Forms;
using System.Drawing;
using System.Threading;

namespace computations
{
	public delegate double Function(double x);
	public delegate double FunctionOf2(double x, double y);
	public class DifferentialEquation
	{
		private FunctionOf2 Func;
		private Function Solution;
		private Plot plot;
		private Thread t;
		private bool SolutionIsShown = false;
		public DifferentialEquation(FunctionOf2 f, Function s = null)
		{
			Func = f;
			Solution = s;
		}
		private void GetReadyToDraw(double a, double b)
		{
			if(plot == null)
			{
				plot = new Plot(a, b);
				t = new Thread(new ThreadStart (StartWindow));
				t.Start();
			}
		}
		private void DrawSolution(double a, double b)
		{
			if(Solution != null && !SolutionIsShown)
			{
				plot.pen.Color = Color.Black;
				plot.pen.Width = 4;
				plot.SetStart(a,Solution(a));
				double x = a, h = 0.1;
				for(x += h; x < b; x += h)
					plot.PutSegment(x, Solution(x));
				plot.ShowResult();
				SolutionIsShown = true;
				plot.pen.Width = 2;
			}
		}
		public void Euler(double knownx, double knowny, double a, double b, double h, bool showSolution = false)
		{
			double x = knownx;
			double y = knowny;
			GetReadyToDraw(a,b);
			if(showSolution) DrawSolution(a,b);
			plot.pen.Color = Color.Red;
			plot.SetStart(knownx,knowny);
			for(; x < b; x += h)
				plot.PutSegment(x + h, y += h * Func(x,y));
			plot.ShowResult();
		}
		public void EulerCauchy(double knownx, double knowny, double a, double b, double h, bool showSolution = false)
		{
			double x = knownx;
			double y = knowny;
			GetReadyToDraw(a,b);
			if(showSolution) DrawSolution(a,b);
			plot.pen.Color = Color.Green;
			plot.SetStart(x,y);
			for(; x < b; x += h)
				plot.PutSegment(x + h,y += h * (Func(x,y) + Func(x + h, y + h * Func(x,y)))/2);
			plot.ShowResult();
		}
		public void RungeKutta(double knownx, double knowny, double a, double b, double h, bool showSolution = false)
		{
			double x = knownx,
			       y = knowny,
			       k1, k2, k3, k4;
			GetReadyToDraw(a,b);
			if(showSolution) DrawSolution(a,b);
			plot.pen.Color = Color.Blue;
			plot.SetStart(x,y);
			for(; x < b; x += h)
			{
				k1 = Func(x,y);
				k2 = Func(x + h/2, y + h*k1/2);
				k3 = Func(x + h/2, y + h*k2/2);
				k4 = Func(x + h, y + h*k3);
				y += h*(k1 + 2*k2 + 2*k3 + k4)/6;
				plot.PutSegment(x + h, y);
			}
			plot.ShowResult();
		}
		[STAThread]
		private void StartWindow()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(plot);
		}
		private class Plot : Graph
		{
			private float lastx, lasty, a, b;
			public Plot(double a, double b): base(a,b) 
			{
				this.a = (int)Math.Floor(a);
				this.b = (int)Math.Ceiling(b);
			}
			public void SetStart(double firstx, double firsty)
			{
				lastx = (float)firstx;
				lasty = (float)firsty;
			}
			public void PutSegment(double x, double y)
			{
				float temp = ContentWidth / (b-a);
				g.DrawLine(pen, Centerx + lastx * temp, Centery - lasty * 150, Centerx + (float)x * temp, Centery - (float)y * 150);
				lastx = (float)x;
				lasty = (float)y;
			}
			public void ShowResult()
			{
				pictureBox1.Image = bmp;
			}
		}
	}
	public class NonlinearEquation
	{
		private Function Func;
		private Function Der;
		private Function DerN;
		public NonlinearEquation(Function f, Function d, Function dn)
		{ 
			Func = f;
			Der = d;
			DerN = dn;
		}
		public double Tang (double e, double a, double b) //касательные
		{
			double x = b;
			while (Math.Abs(Func(x)) >= e)
				x = x - Func(x)/Der(x);
			return x;
		}
		public double Chords (double e, double a, double b) //хорды
		{
			double x = (a+b)/2;
			while (Math.Abs(Func(x)) >= e)
			{
				x = (a * Func(b) - b * Func(a)) / (Func(b) - Func(a));
				if (Func(a)*Func(x) < 0) b = x;
				else a = x; 
			}
			return x;
		}
		public double Iterations (double e, double a, double b) //итерации
		{
			double x = b;
			double c = 1/Der(x);
			while (Math.Abs(Func(x)) >= e)
				x = x - c*Func(x);
			return x;
		}
		public void DrawGraph()
		{
			Thread t = new Thread(new ThreadStart (StartWindow));
			t.Start();
		}
		[STAThread]
		private void StartWindow()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Plot(Func, Der, DerN,-5,5,0.05));
		}
		private class Plot : Graph
		{
			public Plot(Function func, Function deriv1, Function deriv2, int a, int b, double h) : base (a,b)
			{
				x = (float)a;
				float temp = ContentWidth / (float)(b - a);
				pen.Color = Color.Black;
				Pen penred = new Pen(Color.Red,2);
				Pen pengreen = new Pen(Color.Green,2);
				for (x += (float)h; x < b; x += (float)h)
				{
					g.DrawLine(pen,Centerx + (float)(x - h) * temp, Centery - (float)func(x-h) * 5,
					           Centerx + x * temp, Centery - (float)func(x) * 5);
					g.DrawLine(penred,Centerx + (float)(x - h) * temp, Centery - (float)deriv1(x-h) * 5,
					           Centerx + x * temp, Centery - (float)deriv1(x) * 5);
					g.DrawLine(pengreen,Centerx + (float)(x - h) * temp, Centery - (float)deriv2(x-h) * 5,
					           Centerx + x * temp, Centery - (float)deriv2(x) * 5);
				}
				pictureBox1.Image = bmp;
			}
		}
	}	
}
