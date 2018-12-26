using System;
using System.Collections.Generic;
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
using Microsoft.Win32;

namespace ProceduralTextureGenerator
{
    /// <summary>
    /// Interaction logic for TextureView.xaml
    /// </summary>
    public partial class TextureView : UserControl
    {
        public TextureView()
        {
            InitializeComponent();

			view.Bind(3);
		}


		public void Update(float dt)
		{
			view.Update(dt);
		}


		public void Render()
		{
			view.Render();
		}


		private void ScopeTexture(object sender, RoutedEventArgs e)
		{
			CoreDll.TextureViewScope();
		}


		private void SaveTexture(object sender, RoutedEventArgs e)
		{
			SaveFileDialog saveFileDialog = new SaveFileDialog()
			{
				Filter = "JPEG (*.jpg)|*.jpg|Portable Network Graphics (*png)|*.png|Bitmap (*.bmp)|*.bmp|DirectDraw Surface (*.dds)|*.dds"
			};

			if(saveFileDialog.ShowDialog() == true)
			{
				CoreDll.TextureViewSaveTrackedTextureToFile(saveFileDialog.FileName, saveFileDialog.FilterIndex);
			}
		}


		private void OnMouseDown(object sender, MouseButtonEventArgs e)
		{
			Focus();
		}
	}
}
