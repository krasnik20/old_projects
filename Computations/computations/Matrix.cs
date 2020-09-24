using System;

namespace computations
{
	public class Matrix
	{
		public int n, m;
		private double[,] Arr;
		public Matrix(double[,] array)
		{
			n = array.GetLength(0);
			m = array.GetLength(1);
			Arr = new double[n, m];
			for (int i = 0; i < n; i++)
				for (int j = 0; j < m; j++)
					Arr[i, j] = array[i, j];
		}
		public Matrix(int nn, int mm)
		{
			n = nn;
			m = mm;
			Arr = new double[nn,mm];
		}
		public void PrintDevided(string label)
		{
			Console.WriteLine(label);
			PrintDevided();
		}
		public void PrintDevided()
		{
			Print(true);
		}
		public void Print()
		{
			Print(false);
		}
		public void Print(string label)
		{
			Console.WriteLine(label);
			Print();
		}
		private void Print(bool devided)
		{
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m - 1; j++)
					Console.Write("{0,10:0.###}", Arr[i, j]);
				if(devided) Console.Write(" |");
				Console.WriteLine("{0,10:0.###}", Arr[i, m - 1]);
			}
		}
		public Matrix Clone()
		{
			return new Matrix(GetAsArray());
		}
		public double this[int i, int j] {
			get { return Arr[i, j]; }
			set	{ Arr[i, j] = value; }
		}
		public double[,] GetAsArray()
		{
			double[,] array = new double[n, m];
			for (int i = 0; i < n; i++)
				for (int j = 0; j < m; j++)
					array[i, j] = Arr[i, j];
			return array;
		}
		public static Matrix operator *(Matrix a, Matrix b)
		{
			if (a.m != b.n)
				throw new Exception("Матрицы нельзя перемножить");
			int nn = a.n, mm = b.m, s = a.m;
			Matrix result = new Matrix(new double[nn, mm]);
			for (int i = 0; i < nn; i++)
				for (int j = 0; j < mm; j++)
					for (int k = 0; k < s; k++)
						result[i, j] += a[i, k] * b[k, j];
			return result;
		}
		public Matrix Transpose()
		{
			Matrix transposed = new Matrix(m,n);
			for (int i = 0; i < n; i++)
				for (int j = 0; j < m; j++)
					transposed[j,i]=Arr[i,j];
			return transposed;
		}
		public void Gaussian()
		{
			PrintDevided("***МЕТОД ГАУССА***\nИсходная матрица:");
			int i, j;
			Matrix temp = Clone();
			for (j = 0; j < n - 1; j++) {
				for (i = j; i < n; i++)
					if (temp[i, j] != 0)
						break;
				if (i == n)
					continue;
				if (i != j) {
					for (int k = 0; k <= m; k++) {
						double tmp = temp[i, k];
						temp[i, k] = temp[j, k];
						temp[j, k] = tmp;
					}
				}
				for (i = j + 1; i < n; i++) {
					double koef = -temp[i, j] / temp[j, j];
					for (int k = 0; k < m; k++)
						temp[i, k] += temp[j, k] * koef;
				}
			}
			temp.PrintDevided("Приведение к треугольному виду:");
			for (i = 0; i < n; i++)
				if (temp[i, i] == 0) {
					Console.WriteLine("Приведена к треугольному виду, выяснено, что определитель равен 0, решений нет.");
				}
			for (j = n - 1; j > 0; j--) {
				for (i = j - 1; i >= 0; i--) {
					double koef = -temp[i, j] / temp[j, j];
					for (int k = j; k < m; k++)
						temp[i, k] += temp[j, k] * koef;
				}
				temp[j, m - 1] /= temp[j, j];
				temp[j, j] = 1;
			}
			temp.PrintDevided("Приведение к диагональному виду:");
			if (n == m - 1) {
				Console.WriteLine("Ответ:");
				for (i = 0; i < n; i++)
					Console.WriteLine("x{0} = {1,10:0.00000}", i + 1, temp[i, m - 1]);
			} else
				Console.WriteLine("Имеется множество решений.");
		}
		private void SystemByIterations(bool seidel)
		{
			Console.WriteLine(seidel ? "***МЕТОД ЗЕЙДЕЛЯ***" : "***МЕТОД ПРОСТЫХ ИТЕРАЦИЙ***");
			PrintDevided("Исходная матрица:");
			int i, j, k = 0;
			double[] res = new double[n],
			oldres = new double[n];
			Matrix temp = Clone();
			double diff = 5;
			for (i = 0; i < n; i++) {
				double koef = temp[i, i];
				for (j = 0; j < m; j++)
					temp[i, j] /= koef;
				temp[i, i] = 0;
				oldres[i] = temp[i, m - 1] - 1;
				res[i] = temp[i, m - 1];
			}
			while (diff > 0.01) {
				k++;
				diff = 0;
				for (i = 0; i < n; i++)
					oldres[i] = res[i];
				for (i = 0; i < n; i++) {
					res[i] = temp[i, m - 1];
					for (j = 0; j < m - 1; j++)
						res[i] -= temp[i, j] * (seidel ? res[j] : oldres[j]);
					diff += Math.Abs(res[i] - oldres[i]);
				}
			}
			Console.WriteLine("Количество итераций: {0}. Ответ:", k);
			for (i = 0; i < n; i++)
				Console.WriteLine("x{0} = {1,10:0.00000}", i + 1, res[i]);
		}
		public void ByIterations()
		{
			SystemByIterations(false);
		}
		public void BySeidel()
		{
			SystemByIterations(true);
		}
		public void Rotations(double eps)
		{
			Console.WriteLine("***МЕТОД ВРАЩЕНИЙ***");
			if (n!=m) throw new Exception("Матрица не квадратная");
			Matrix temp = Clone(), result = new Matrix(n,m);
			temp.Print("Исходная матрица:");
			for(int i = 0; i < n; i++) result[i,i] = 1;
			while(true)
			{
				double max = 0;
				int imax = -1, jmax = -1;
				for(int i = 0; i < temp.n - 1; i++)
				{
					for(int j = i+1; j < temp.m; j++)
					{
						if (Math.Abs(temp[i,j]) > Math.Abs(max))
						{
							max=temp[i,j];
							imax=i;
							jmax=j;
						}
					}
				}
				if (Math.Abs(max)<eps) break;
				double fi = 0.5*Math.Atan2(2*max, temp[imax,imax]-temp[jmax,jmax]);
				Matrix U = new Matrix(n,m);
				for(int i = 0; i < n; i++) U[i,i] = 1;
				U[imax,imax] = U[jmax,jmax] = Math.Cos(fi);
				U[imax,jmax] = -Math.Sin(fi);
				U[jmax,imax] = Math.Sin(fi);
				temp = (U.Transpose() * temp) * U;
				result *= U;
			}
			Console.WriteLine("Собственные значения: ");
			for(int i = 0; i < n; i++) Console.Write("{0,10:0.000}", temp[i,i]);
			Console.WriteLine();
			result.Print("Собственные вектора (в столбцах):");
		}
	}
}
