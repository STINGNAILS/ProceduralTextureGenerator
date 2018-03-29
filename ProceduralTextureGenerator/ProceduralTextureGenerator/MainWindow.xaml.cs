using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace ProceduralTextureGenerator
{
	/// <summary>
	/// Логика взаимодействия для MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		private Stopwatch frameTime;
		private TimeSpan lastRenderTime;


		public MainWindow()
		{
			InitializeComponent();

			CoreDll.Init();

			CompositionTarget.Rendering += Update;

			graphView.SetParameterPanel(parameterPannel);

			frameTime = new Stopwatch();
			frameTime.Start();
		}


		private void Update(object sender, EventArgs e)
		{
			RenderingEventArgs args = (RenderingEventArgs)e;

			if(lastRenderTime != args.RenderingTime)
			{
				UpdateControls();
				objectView.Render();
				graphView.Render();
				lastRenderTime = args.RenderingTime;
			}
		}


		private void UpdateControls()
		{
			frameTime.Stop();
			float dt = frameTime.ElapsedMilliseconds / 1000.0f;

			objectView.Update(dt);
			graphView.Update(dt);
			//textureView.OnMouseMove(dx, dy, dt);

			frameTime.Restart();
		}


		private void OnKeyDown(object sender, KeyEventArgs e)
		{

		}


		private void OnExit(object sender, EventArgs e)
		{
			CoreDll.Release();
		}
	}
}
