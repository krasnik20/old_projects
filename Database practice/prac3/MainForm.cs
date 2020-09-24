using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.Linq;

namespace prac3
{
	public partial class MainForm : Form
	{
		BindingList<Record> data;
		TextBox [] fields = new TextBox[6];
		public MainForm()
		{
			InitializeComponent();
			data = new BindingList<Record>();
			dataGridView1.DataSource=data;
			dataGridView1.AllowUserToAddRows=false;
			dataGridView1.AutoGenerateColumns=false;
			dataGridView1.Columns.Add(new DataGridViewButtonColumn());
			dataGridView1.Columns[7].Name="remove";
			dataGridView1.Columns["number"].HeaderText="Номер";
			dataGridView1.Columns["area"].HeaderText="Площадь";
			dataGridView1.Columns["living_area"].HeaderText="Жилая площадь";
			dataGridView1.Columns["price"].HeaderText="Цена";
			dataGridView1.Columns["habitants"].HeaderText="Жильцы";
			dataGridView1.Columns["kids"].HeaderText="Дети";
			dataGridView1.Columns["personal"].HeaderText="м^2/чел.";
			dataGridView1.Columns["remove"].HeaderText="Удалить";
			Button example = new Button();
			example.UseVisualStyleBackColor = true;
			example.Location = new Point(0, 385);
			example.Size = new Size(200, 30);
			example.TabIndex = 7;
			example.Text = "Вставить пример базы";
			example.Click += ExampleClick;
			Controls.Add(example);
			Label sortLabel = new Label();
			sortLabel.Location = new Point(0, 310);
			sortLabel.Size = new Size(100, 20);
			sortLabel.Text = "Сортировать: ";
			Controls.Add(sortLabel);
			Button byArea = new Button();
			byArea.UseVisualStyleBackColor = true;
			byArea.Location = new Point(100, 300);
			byArea.Size = new Size(100, 30);
			byArea.Text = "по площади";
			byArea.Click += ByAreaClick;
			Controls.Add(byArea);
			Button byHabitants = new Button();
			byHabitants.UseVisualStyleBackColor = true;
			byHabitants.Location = new Point(200, 300);
			byHabitants.Size = new Size(200, 30);
			byHabitants.Text = "по кол-ву жильцов";
			byHabitants.Click += ByHabitantsClick;
			Controls.Add(byHabitants);
			Button byPersonal = new Button();
			byPersonal.UseVisualStyleBackColor = true;
			byPersonal.Location = new Point(400, 300);
			byPersonal.Size = new Size(100, 30);
			byPersonal.Text = "по м^2/чел.";
			byPersonal.Click += ByPersonalClick;
			Controls.Add(byPersonal);
			Button byPrice = new Button();
			byPrice.UseVisualStyleBackColor = true;
			byPrice.Location = new Point(500, 300);
			byPrice.Size = new Size(100, 30);
			byPrice.Text = "по стоимости";
			byPrice.Click += ByPriceClick;
			Controls.Add(byPrice);
			Label addLabel = new Label();
			addLabel.Location = new Point(0, 360);
			addLabel.Size = new Size(100, 20);
			addLabel.Text = "Добавить запись: ";
			Controls.Add(addLabel);
			Label numberLabel = new Label();
			numberLabel.Location = new Point(100, 340);
			numberLabel.Size = new Size(100, 20);
			numberLabel.Text = "Номер: ";
			Controls.Add(numberLabel);
			Label areaLabel = new Label();
			areaLabel.Location = new Point(200, 340);
			areaLabel.Size = new Size(100, 20);
			areaLabel.Text = "Площадь: ";
			Controls.Add(areaLabel);
			Label livingAreaLabel = new Label();
			livingAreaLabel.Location = new Point(300, 340);
			livingAreaLabel.Size = new Size(100, 20);
			livingAreaLabel.Text = "Жилая площадь: ";
			Controls.Add(livingAreaLabel);
			Label priceLabel = new Label();
			priceLabel.Location = new Point(400, 340);
			priceLabel.Size = new Size(100, 20);
			priceLabel.Text = "Цена: ";
			Controls.Add(priceLabel);
			Label habitantsLabel = new Label();
			habitantsLabel.Location = new Point(500, 340);
			habitantsLabel.Size = new Size(100, 20);
			habitantsLabel.Text = "Жильцы: ";
			Controls.Add(habitantsLabel);
			Label kidsLabel = new Label();
			kidsLabel.Location = new Point(600, 340);
			kidsLabel.Size = new Size(100, 20);
			kidsLabel.Text = "Дети: ";
			Controls.Add(kidsLabel);
			TextBox number = new TextBox();
			number.TabIndex = 0;
			number.Location = new Point(100,360);
			number.Size = new Size(100,30);
			Controls.Add(number);
			TextBox area = new TextBox();
			area.TabIndex = 1;
			area.Location = new Point(200,360);
			area.Size = new Size(100,30);
			Controls.Add(area);
			TextBox livingArea = new TextBox();
			livingArea.TabIndex = 2;
			livingArea.Location = new Point(300,360);
			livingArea.Size = new Size(100,30);
			Controls.Add(livingArea);
			TextBox price = new TextBox();
			price.TabIndex = 3;
			price.Location = new Point(400,360);
			price.Size = new Size(100,30);
			Controls.Add(price);
			TextBox habitants = new TextBox();
			habitants.TabIndex = 4;
			habitants.Location = new Point(500,360);
			habitants.Size = new Size(100,30);
			Controls.Add(habitants);
			TextBox kids = new TextBox();
			kids.TabIndex = 5;
			kids.Location = new Point(600,360);
			kids.Size = new Size(100,30);
			Controls.Add(kids);
			fields[0]=number;
			fields[1]=area;
			fields[2]=livingArea;
			fields[3]=price;
			fields[4]=habitants;
			fields[5]=kids;
			Button send = new Button();
			send.Location = new Point(700,360);
			send.Size = new Size(100,20);
			send.TabIndex = 6;
			send.Text = "Добавить";
			send.UseVisualStyleBackColor = true;
			send.Click += SendClick;
			Controls.Add(send);
			
		}
		private void CreateExampleDB()
		{
			for(int i = 0; i<10;i++) data.Add(new Record(Convert.ToString(25+i),
			                                             Convert.ToString(55-i),
			                                             Convert.ToString(44-i),
			                                             Convert.ToString(2000+i*100),
			                                             Convert.ToString(3*(i%3)),
			                                             Convert.ToString(2)));
		}
		void DataGridView1CellContentClick(object sender, DataGridViewCellEventArgs e)
		{
			if (e.ColumnIndex!=7||e.RowIndex==-1) return;
			data.RemoveAt(e.RowIndex);
		}
		void ExampleClick(object sender, EventArgs e)
		{
			CreateExampleDB();
		}
		void ByAreaClick(object sender, EventArgs e)
		{
			data = new BindingList<Record>(data.OrderBy(d => d.area).ToList());
			dataGridView1.DataSource = data;
		}
		void ByHabitantsClick(object sender, EventArgs e)
		{
			data = new BindingList<Record>(data.OrderBy(d => d.habitants).ToList());
			dataGridView1.DataSource = data;
		}
		void ByPersonalClick(object sender, EventArgs e)
		{
			data = new BindingList<Record>(data.OrderBy(d => d.personal).ToList());
			dataGridView1.DataSource = data;
		}
		void ByPriceClick(object sender, EventArgs e)
		{
			data = new BindingList<Record>(data.OrderBy(d => d.price).ToList());
			dataGridView1.DataSource = data;
		}
		void SendClick(object sender, EventArgs e)
		{
			double tmp;
			if(!double.TryParse(fields[2].Text, out tmp)||!double.TryParse(fields[4].Text,out tmp))
			{
				MessageBox.Show("Некорректные данные!");
				return;
			}
			data.Add(new Record(fields[0].Text,
			                    fields[1].Text,
			                    fields[2].Text,
			                    fields[3].Text,
			                    fields[4].Text,
			                    fields[5].Text));
		}
	}
	public class Record
	{
		public Record (string Number, string Area, string Living_area, string Price, string Habitants, string Kids)
		{
			number=Number;
			area=Area;
			living_area=Living_area;
			price=Price;
			habitants=Habitants;
			kids=Kids;
		}
		public string number { get; set; }
		public string area { get; set; }
		public string living_area { get; set; }
		public string price { get; set; }
		public string habitants { get; set; }
		public string kids { get; set; }
		public string personal { get { return string.Format("{0:0.##}",(Convert.ToDouble(living_area)/Convert.ToDouble(habitants))); } }
	}
}
