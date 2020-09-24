using System;
using System.Windows.Forms;
using System.Drawing;

namespace computations
{
	class Program
	{
		public static void Main()
		{
			ShowMenu();
			int option;
			int.TryParse(Console.ReadLine(), out option);
			while (option != 15) 
			{
				if(option > 0 && option < 15)
				{
					Console.Clear();
					if(option == 1)
					{
						Matrix test = new Matrix(new double[,] {
							{ 1, 2, -1, -7, -23 },
							{ 8, 0, -9, -3, 39 },
							{ 2, -3, 7, 1, -7 },
							{ 1, -5,-6,-8, 30 }
						});
						test.Gaussian();
					}
					if(option == 2)
					{
						// ничего пока
					}
					if(option == 3)
					{
						Matrix test = new Matrix(new double[,] {
							{ 23, -6, -5, 9, 232 },
							{ 8, 22, -2, 5, -82 },
							{ 7, -6, 18, -1, 202 },
							{ 3, 5, 5, -19, -57 }
						});
						test.ByIterations();
						test.BySeidel();
					}
					if(option == 4)
					{
						Matrix test = new Matrix(new double[,] {
							{ 9, 2, -7 },
							{ 2, -4, -1 },
							{ -7, -1, 1 }
						});
						test.Rotations(0.05);
					}
					if(option == 5)
					{
						Function Func = (double x) => Math.Exp(x) - 2 * x - 2;
						Function Der = (double x) => Math.Exp(x) - 2;
						Function DerN = Math.Exp;
						NonlinearEquation test = new NonlinearEquation(Func, Der, DerN);
						Console.WriteLine("Метод простой итерации: {0:0.000}", test.Iterations(0.001,0,5));
						Console.WriteLine("            Метод хорд: {0:0.000}", test.Chords(0.001,0,5));
						Console.WriteLine("         Метод Ньютона: {0:0.000}", test.Tang(0.001,0,5));
						test.DrawGraph();
					}
					if(option == 12)
					{
						FunctionOf2 Func = (double x, double y) => -y * Math.Tan(x) + Math.Sin(2*x);
						Function Solution = (double x) => -2*Math.Pow(Math.Cos(x),2) + Math.Cos(x);
						DifferentialEquation test = new DifferentialEquation(Func,Solution);
						test.Euler(0, -1, 0, 2, 0.1, true);
						test.EulerCauchy(0, -1, 0, 2, 0.1);
						test.RungeKutta(0,-1,0,2,0.1);
					}
					if(option == 14) break;
				}
				else Console.WriteLine("Некорректный номер");
				ShowMenu();
				Console.WriteLine("\nВведите номер следующего задания :");
				int.TryParse(Console.ReadLine(), out option);
			}
		}
		private static void ShowMenu()
		{
			Console.WriteLine("Выберите номер задания:");
			Console.WriteLine(" 1. Метод Гаусса");
			Console.WriteLine(" 3. Метод простых итераций и метод Зейделя");
			Console.WriteLine(" 4. Метод вращений");
			Console.WriteLine(" 5. Решение нелинейного уравнения");
			Console.WriteLine("12. Решение дифференциального уравнения");
			Console.WriteLine("14. Выход");
		}
	}
}