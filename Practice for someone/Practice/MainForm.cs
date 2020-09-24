using System;
using System.Drawing;
using System.Windows.Forms;
//C# - строго объектно-ориентированный язык. поэтому тут сразу идет класс. 
//вообще, тут есть класс program и в нем есть метод main. аналог int main в c++. но это если бы у нас было консольное приложение.
//поскольку у нас оконное, то в том методе main (его кстати можно посмотреть в файле program.cs) только происходит, грубо говоря, 
//переключение на оконный режим, и запуск другого главного метода - конструктора объекта mainForm, который в данном контексте 
//является аналогом int main.
namespace Practice
{
	public partial class MainForm : Form //класс главной формы (форма - это любое окно, кнопка и прочее)
	{
		//partial - частичный, т.е. где-то в другом месте может быть продолжение объявления внутренностей этого класса
		//и оно есть - в MainForm.Designer.cs
		public MainForm()//а вот и конструктор, то есть место, где происходит главный экшон
		{
			InitializeComponent();//вызвыается метод, где создается окно, на нем создаются pictureBox-ы(подокна с картинкой). если интересно - смотри файл MainForm.Designer.cs
			double a = 1.9f, b = 2.93f; //границы поиска корней. f - указание, что float, потому что C# - более строго типизированный, чем c++, надо наглядно указывать
			int i, j;//индексы пригодятся
			//в c# работа с объектами ведется не напрямую и не по указателю, а по ссылке (что-то типа неочевидного указателя)
			//поэтому надо выделять память, а ссылка, возвращаемая new, записывается, как объект, поэтому нет звездочки
			Size stdsize = new Size(102,20);//константа стандартного размера всех блоков (дальше поймешь), чтобы текст влезал
			//массив с количествами приближений: 6 точностей и 4 метода поиска корней. насколько мне известно, тут все автоматически обнуляется, поэтому не делаем этого явно
			int [,] n = new int[6,4];
			double e = 0.01f;//точность
			//начинаем создавать всякие оконные элементы (они все относятся к абстрактному классу control)
			//label - просто надпись на экране
			Label header1 = new Label();//заголовок1
			header1.Text = "Корни:";//добавляем текст
			header1.Location = new Point(5,5);//местоположение на экране (ТОЧКОЙ (5,5))
			Controls.Add(header1);//и добавляем этот элемент к окну, чтобы отображалось
			Label [] labels1 = new Label[5];//массив подписей методов.
			for(i = 0; i < 5; i++)
			{//делаем теперь то же самое, что и для заголовка, но еще указываем размер, поскольку надпись "деление пополам" не влезает в размер, задаваемый по умолчанию 
				labels1[i] = new Label();
				labels1[i].Location = new Point(5 + i*(stdsize.Width + 10),30);
				labels1[i].Size = stdsize;
				Controls.Add(labels1[i]);
			}
			labels1[0].Text = "Точность:"; //ну и тексты записываем
			labels1[1].Text = "Деление пополам:";
			labels1[2].Text = "Хорды:";
			labels1[3].Text = "Касательные:";
			labels1[4].Text = "Итерации:";
			Label [] eps = new Label[6];//набор подписей для точностей
			for(i = 0; i < 6; i++)
			{
				eps[i] = new Label();
				eps[i].Location = new Point(5,60 + i*25);
				eps[i].Size = stdsize;
				eps[i].TextAlign = ContentAlignment.MiddleRight; //выравнивание по правому краю по центру высоты
				Controls.Add(eps[i]);
			}
			//другой вид control-ов - коробочки с текстом, то есть поле, из которого результат можно скопировать
			//можно и чтобы набирать туда текст можно было, но нам это не надо
			TextBox [,] roots = new TextBox[6,4]; //куча таких боксов для корней
			for (i = 0; i < 6; i++)
				for (j = 0; j < 4; j++)
				{
					roots[i,j] = new TextBox();
					roots[i,j].Size = stdsize;
					roots[i,j].Location = new Point(5 + (j+1)*(stdsize.Width + 10),60 + i*25);
					roots[i,j].ReadOnly = true;//чтобы нельзя было что-то туда набрать
					roots[i,j].TabStop = false;//почему-то автоматически текст выделяется, отключаем
					roots[i,j].TextAlign = HorizontalAlignment.Right;//выравнивание по правому краю
					Controls.Add(roots[i,j]);
				}
			for (i = 0; i < 6; e /= 10f, i++)  
			{//string.format - что-то вроде printf, где сначала задается формат строки, потом выводимые переменные
			 //вместо %d пишем {}, в которых номер аргумента, который требуется вывести (printf брал все по очереди, тут можно нумеровать)
				eps[i].Text = String.Format("{0:0.#######}", e);
				//по очереди вызываем все методы поиска корня, выводим их в их поля, считаем количество потребовавшихся приближений
				roots[i,0].Text = String.Format("{0:0.0000000}", half(e, a, b, ref n[i,0]));//ref-reference, то есть ссылка. Почти как указатель
				roots[i,1].Text = String.Format("{0:0.0000000}", chord(e, a, b, ref n[i,1]));
				roots[i,2].Text = String.Format("{0:0.0000000}", tang(e, a, b, ref n[i,2]));
				roots[i,3].Text = String.Format("{0:0.0000000}", iter(e, a, b, ref n[i,3]));
			}
			Label header2 = new Label();//второй заголовок
			header2.Text = "Количество приближений:";
			header2.Location = new Point(5,215);
			header2.Size = new Size(200,15);
			Controls.Add(header2);
			Label [] labels2 = new Label[5];//вторые подписи методов
			for(i = 0; i < 5; i++)
			{
				labels2[i] = new Label();
				labels2[i].Location = new Point(5 + i*(stdsize.Width + 10),240);
				labels2[i].Size = stdsize;
				labels2[i].Text = labels1[i].Text;//просто копируем текст старых подписей
				Controls.Add(labels2[i]);
			}
			Label [,] approximations = new Label[6,5];//количество приближений выводим просто текстом, без возможности скопировать
			for (i = 0; i < 6; i++)
				for (j = 0; j < 5; j++)
				{
					approximations[i,j] = new Label();
					approximations[i,j].Size = stdsize;
					approximations[i,j].Location = new Point(5+j*(stdsize.Width+10),265+i*15);
					approximations[i,j].TextAlign = ContentAlignment.MiddleLeft;
					Controls.Add(approximations[i,j]);
				}
			e = 0.01f;//надо сбросить это значение, потому что до этого мы его испортили
			for (i = 0; i < 6; e /= 10f, i++)  
			{
				approximations[i,0].Text=String.Format("{0:0.#######}",e);// точность
				for (j = 1; j < 5; j++)	approximations[i,j].Text=String.Format("{0}",n[i,j-1]);//количество приближений на каждом методе
			}
			Label header3 = new Label();//заголовок для графиков
			header3.Text = "Изменение количества приближений при переходе к повышенной точности:";
			header3.Location = new Point(5,370);
			header3.Size = new Size(400,15);
			Controls.Add(header3);
			//очередной контрол - в этот раз картиночка
			//их создание произошло в MainForm.designer.cs, поскольку я их добавлял не вручную, а через режим дизайнера
			//там инициализация не такая тривиальная, а он автоматически код генерит
			pictureBox1.Location = new Point(590, 10);//график 
			pictureBox1.Size = new Size(400, 300);
			pictureBox2.Location = new Point(5+stdsize.Width+10, 395);//и 4 диаграммы
			pictureBox2.Size = new Size(100, 100);
			pictureBox3.Location = new Point(5+2*stdsize.Width+10, 395);
			pictureBox3.Size = new Size(100, 100);
			pictureBox4.Location = new Point(5+3*(stdsize.Width+10), 395);
			pictureBox4.Size = new Size(100, 100);
			pictureBox5.Location = new Point(5+4*(stdsize.Width+10), 395);
			pictureBox5.Size = new Size(100, 100);
			//создаем описанные дальше график и диаграммы, и они сразу рисуются в своих конструкторах
			new Plot(pictureBox1, "X", "Y", function, 0, 3);
			new Diagram(pictureBox2, n, 0);
			new Diagram(pictureBox3, n, 1);
			new Diagram(pictureBox4, n, 2);
			new Diagram(pictureBox5, n, 3);
		}
		//тут описаны методы главного класса, а если на языке c++ - почти обычные функции
		double function (double x) { return x*x*x-2*x-5; } //исследуемая функция
		double derivative (double x) { return 3*x*x-2; } //ее производная
		//дальше описаны все методы поиска корня, подробности лучше загуглить, там понятнее алгоритмы объяснят
		double half (double e, double a, double b, ref int n) //деление отрезка пополам
		{
			double c;
			while (Math.Abs(function(a)) >= e) //Math.abs - это как в c++ просто вызвать abs из math.h
			{
				c = (a + b) / 2;
				n+=1;
				if (function(a) * function(c) <= 0) b = c;
				else a = c;
			}
			return a;
		}
		double tang (double e, double a, double b, ref int n) //касательные
		{
			double x = b;
			while (Math.Abs(function(x)) >= e)
			{
				n+=1;
				x = x - function(x)/derivative(x);
			}
			return x;
		}
		double chord (double e, double a, double b, ref int n) //хорды
		{
			double x = (a+b)/2;
			while (Math.Abs(function(x)) >= e)
			{
				n+=1;
				x = (a * function(b) - b * function(a)) / (function(b) - function(a));
				if (function(a)*function(x) < 0) b = x;
				else a = x; 
			}
			return x;
		}
		double iter (double e, double a, double b, ref int n) //итерации
		{
			double x = b;
			double c = 1/derivative(x);
			while (Math.Abs(function(x)) >= e)
			{
				x = x - c*function(x);
				n+=1;
			}
			return x;
		}
		void Button1Click(object sender, EventArgs e) //обработчик нажатий на кнопку "Сохранить графики"
		//сама кнопка создалась в mainForm.Designer
		{
			(pictureBox1.Image as Bitmap).Save("plot.png",System.Drawing.Imaging.ImageFormat.Png);//да, вот так в одну строку, классно, да?))
			(pictureBox2.Image as Bitmap).Save("diag1.png",System.Drawing.Imaging.ImageFormat.Png);//единственное, что он качество немного сжимает
			(pictureBox3.Image as Bitmap).Save("diag2.png",System.Drawing.Imaging.ImageFormat.Png);//поэтому если картинку не увеличить, может показаться
			(pictureBox4.Image as Bitmap).Save("diag3.png",System.Drawing.Imaging.ImageFormat.Png);//что там цифры недоотображаются
			(pictureBox5.Image as Bitmap).Save("diag4.png",System.Drawing.Imaging.ImageFormat.Png);
		}
	}
	public class Picture//класс-основа для графика и диаграммы, потом унаследуем
	{
		protected PictureBox p;//картинка на экране
		protected Graphics g;//если так можно выразиться, движок рисования
		protected Bitmap bmp;//битмап, на котором мы будем рисовать
		//потом этот битмап мы будем отправлять для отображения в pictureBox, а его в свою очередь отобразит mainForm. сложно, но куда деваться
		protected Pen pen;//перо для рисования, чтобы в дальнейшем много раз все не прописывать
		protected Font font;//и шрифт
		protected float w, h; //ширина, высота
		public Picture(PictureBox pp)//конструктор
		{
			p = pp;
			w = pp.Width;
			h = pp.Height;
			bmp = new Bitmap((int)w, (int)h);//инициализируем тот самый битмап, на котором мы будем рисовать
			g = Graphics.FromImage(bmp);//на основе битмапа создаем движок рисования.
			pen = new Pen(Color.Black,1);//указываем перо
			font = new Font("Calibri", 10);//и шрифт
		}
	}
	public class Plot : Picture //"график" - наследник "Картинки"
	{
		//если кратко, то эта конструкция нужна, чтобы можно было функцию передать как параметр в функцию
		//при этом как бы создается новый, так сказать, активный вид данных Function
		//который принимает аргумент и выдает значение типа double
		public delegate double Function (double x); 
		//:base - вызов конструктора класса-предка
		//axisX и Y - названия осей, Function func - функция, график которой мы строим, и границы рисования
		public Plot(PictureBox pp, string axisX, string axisY, Function func, double a, double b):base(pp)
		{
			float s = h/17;//s - size - нужно для размера стрелочки, отступа и еще кое-чего
			//точность графика (один поделить на из скольких отрезков состоит график)
			//шаг аргумента 
			//максимальное (подобранное) значение для оси y
			double precision = 0.01, step = (b-a)*precision, x, maxY = 10;
			//у движка g вызывается рисование линии пером pen, координаты как в line в graphics.h
			//отступ s от края рисунка, чтобы было, куда черточки вставлять
			g.DrawLine(pen, s, s, s, h-s);//ось y
			g.DrawLine(pen, s, 2*s, w-s, 2*s);//ось x
			g.DrawLine(pen, s, s, s-s/4, s+s/4);//стрелочка оси y
			g.DrawLine(pen, s, s, s+s/4, s+s/4);
			g.DrawLine(pen, w-s, 2*s, w-s-s/4, 2*s-s/4);//стрелочка оси х
			g.DrawLine(pen, w-s, 2*s, w-s-s/4, 2*s+s/4);
			g.DrawString(axisX,font,Brushes.Black,w-s,s);//подписи координат
			g.DrawString(axisY,font,Brushes.Black,0,0);
			for (x = a; x < b; x+=step)
			{//рисование графика соединением текущей точки с предыдущей
				//лень расписывать смысл формулы, надеюсь, не слишком сложно
				g.DrawLine(pen, s   + (float)((w-2*s) * x            / Math.Ceiling(b)),
				                2*s - (float)((h-2*s) * func(x)      / maxY),
				                s   + (float)((w-2*s) * (x-step)     / Math.Ceiling(b)),
				                2*s - (float)((h-2*s) * func(x-step) / maxY));
			}
			for (x = 1; x < b; x++)
			{
				g.DrawLine(pen, s+(float)((w-2*s)*x/Math.Ceiling(b)), 2*s - s/4,
                                s+(float)((w-2*s)*x/Math.Ceiling(b)), 2*s + s/4);//черточка и подпись оси x
				g.DrawString(String.Format("{0:0.}", x), font, Brushes.Black, s/1.5f + (float)((w-2*s) * x / Math.Ceiling(b)), 2*s + s/4);
			}
			for (x = 0; x > -maxY; x--)
			{
				g.DrawLine(pen, s-s/4, 2*s-(float)((h-2*s)*x/maxY),
				                s+s/4, 2*s-(float)((h-2*s)*x/maxY));//черточка и подпись оси у
				g.DrawString(String.Format("{0:0.}", x), font, Brushes.Black, 0, 1.5f*s-(float)((h-2*s)*x/maxY));
			}
			p.Image = bmp;//просим picturebox показать нам картинку, говоря, что текущий bitmap, на котором мы рисовали, теперь картинка у пикчербокса
		}
	}
	public class Diagram : Picture //диаграмма
	{
		public Diagram(PictureBox pp, int [,] n, int j):base(pp)
		{
			float s = h/10; //size
			int i; //счетчик
			g.DrawLine(pen, w/6, 0, w/6, h);//ось типа y
			g.DrawLine(pen, w/6+s/2, s/2, w/6, 0);//стрелочка
			g.DrawLine(pen, w/6-s/2, s/2, w/6, 0);
			for (i = 1; i < 6; i++) g.DrawLine(pen, w/6-s/3, h-i*h/6, w/6, h-i*h/6);//черточки
			for (i = 1; i < 6; i++) g.DrawString(String.Format("{0}",i), font, Brushes.Black, 0, h-i*h/6-h/12);//подписи
			for (i = 1; i < 6; i++)	g.FillRectangle(Brushes.Blue, i*w/6+2, h-(n[i,j]-n[i-1,j])*h/6, w/6-1, (n[i,j]-n[i-1,j])*h/6);//прямоугольники диаграммы
			p.Image = bmp;//просим picturebox показать нам картинку
		}
	}
}
