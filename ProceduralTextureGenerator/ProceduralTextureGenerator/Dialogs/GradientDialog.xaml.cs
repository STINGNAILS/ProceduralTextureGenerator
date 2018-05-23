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
using System.Windows.Shapes;

namespace ProceduralTextureGenerator
{
	/// <summary>
	/// Логика взаимодействия для GradientDialog.xaml
	/// </summary>
	public partial class GradientDialog : Window
	{
		private int gradientSize;
		private List<Grad> gradients;

		GradientStopCollection gradientStops;

		private int currentGradientStepIndex;
		private bool isDragging;

		public int GradientSize
		{
			get
			{
				return gradientSize;
			}
		}


		public GradientDialog(int gradientSize_, List<Grad> gradients_)
		{
			InitializeComponent();

			gradientSize = gradientSize_;
			gradients = gradients_;

			gradientStops = new GradientStopCollection(gradientSize);
			
			for(int i = 0; i < gradientSize; i++)
			{
				Grad gradient = gradients[i];
				Color color = Color.FromArgb((byte)255, (byte)((int)(gradient.r * 255)), (byte)((int)(gradient.g * 255)), (byte)((int)(gradient.b * 255)));

				gradientStops.Add(new GradientStop(color, gradient.k));

				Ellipse ellipse = new Ellipse()
				{
					Width = 10.0,
					Height = 10.0,
					Fill = new SolidColorBrush(color),
					Stroke = new SolidColorBrush(Colors.DarkRed),
					StrokeThickness = 1.0,
					Focusable = true
				};
				ellipse.MouseDown += GradientDragMouseDown;

				gradientCanvas.Children.Add(ellipse);
				Canvas.SetLeft(ellipse, gradient.k * 450.0 - 5.0);
			}
			
			LinearGradientBrush brush = new LinearGradientBrush(gradientStops)
			{
				StartPoint = new Point(0.0, 0.0),
				EndPoint = new Point(1.0, 0.0)
			};

			gradientRectangle.Fill = brush;

			gradientCanvas.MouseMove += GradientDragMouseMove;
			gradientCanvas.MouseUp += GradientDragMouseUp;

			gradientRectangle.MouseUp += CreateGradientStep;

			currentGradientStepIndex = 0;
			((Ellipse)gradientCanvas.Children[currentGradientStepIndex]).Stroke = new SolidColorBrush(Colors.White);

			isDragging = false;

			KeyDown += DeleteGradientStep;
			
			kTextBox.LostFocus += TextBoxFocusLost;
			kTextBox.GotKeyboardFocus += SelectText;
			kTextBox.MouseDoubleClick += SelectText;
			kTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			
			redColorTextBox.LostFocus += TextBoxFocusLost;
			redColorTextBox.GotKeyboardFocus += SelectText;
			redColorTextBox.MouseDoubleClick += SelectText;
			redColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			
			greenColorTextBox.LostFocus += TextBoxFocusLost;
			greenColorTextBox.GotKeyboardFocus += SelectText;
			greenColorTextBox.MouseDoubleClick += SelectText;
			greenColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;
			
			blueColorTextBox.LostFocus += TextBoxFocusLost;
			blueColorTextBox.GotKeyboardFocus += SelectText;
			blueColorTextBox.MouseDoubleClick += SelectText;
			blueColorTextBox.PreviewMouseLeftButtonDown += IgnoreMouseButton;

			UpdateCurrentGradientStep();
		}


		private void AcceptGradient(object sender, RoutedEventArgs e)
		{
			DialogResult = true;
		}


		private void UpdateCurrentGradientStep()
		{
			Grad gradient = gradients[currentGradientStepIndex];
			float k;
			float r;
			float g;
			float b;

			if(float.TryParse(kTextBox.Text.Replace(".", ","), out k))
			{
				float kMin;
				float kMax;
				if(currentGradientStepIndex == 0)
				{
					kMin = 0.0f;
					kMax = gradients[1].k - 0.005f;
				}
				else if(currentGradientStepIndex == gradientSize - 1)
				{
					kMin = gradients[gradientSize - 2].k + 0.005f;
					kMax = 1.0f;
				}
				else
				{
					kMin = gradients[currentGradientStepIndex - 1].k + 0.005f;
					kMax = gradients[currentGradientStepIndex + 1].k - 0.005f;
				}

				k = Math.Min(Math.Max(k, kMin), kMax);
			}
			else
			{
				k = gradient.k;
			}

			if(float.TryParse(redColorTextBox.Text.Replace(".", ","), out r))
			{
				r = Math.Min(Math.Max(r, 0.0f), 1.0f);
			}
			else
			{
				r = gradient.r;
			}

			if(float.TryParse(greenColorTextBox.Text.Replace(".", ","), out g))
			{
				g = Math.Min(Math.Max(g, 0.0f), 1.0f);
			}
			else
			{
				g = gradient.g;
			}

			if(float.TryParse(blueColorTextBox.Text.Replace(".", ","), out b))
			{
				b = Math.Min(Math.Max(b, 0.0f), 1.0f);
			}
			else
			{
				b = gradient.b;
			}
			
			Color color = Color.FromArgb((byte)255, (byte)((int)(r * 255)), (byte)((int)(g * 255)), (byte)((int)(b * 255)));
			double x = k * 450.0;

			gradients[currentGradientStepIndex] = new Grad(k, r, g, b);
			gradientStops[currentGradientStepIndex].Offset = k;
			gradientStops[currentGradientStepIndex].Color = color;
			((Ellipse)gradientCanvas.Children[currentGradientStepIndex]).Fill = new SolidColorBrush(color);
			Canvas.SetLeft(gradientCanvas.Children[currentGradientStepIndex], x - 5.0);

			kTextBox.Text = k.ToString("F6").Replace(",", ".");
			redColorTextBox.Text = r.ToString("F6").Replace(",", ".");
			greenColorTextBox.Text = g.ToString("F6").Replace(",", ".");
			blueColorTextBox.Text = b.ToString("F6").Replace(",", ".");

			colorRect.Fill = new SolidColorBrush(color);
		}


		private void GradientDragMouseDown(object sender, MouseButtonEventArgs e)
		{
			UpdateCurrentGradientStep();

			((Ellipse)gradientCanvas.Children[currentGradientStepIndex]).Stroke = new SolidColorBrush(Colors.DarkRed);
			currentGradientStepIndex = gradientCanvas.Children.IndexOf((UIElement) sender);
			((Ellipse)gradientCanvas.Children[currentGradientStepIndex]).Stroke = new SolidColorBrush(Colors.White);

			kTextBox.Text = gradients[currentGradientStepIndex].k.ToString("F6").Replace(",", ".");
			redColorTextBox.Text = gradients[currentGradientStepIndex].r.ToString("F6").Replace(",", ".");
			greenColorTextBox.Text = gradients[currentGradientStepIndex].g.ToString("F6").Replace(",", ".");
			blueColorTextBox.Text = gradients[currentGradientStepIndex].b.ToString("F6").Replace(",", ".");

			Color color = Color.FromArgb((byte)255, (byte)((int)(gradients[currentGradientStepIndex].r * 255)), (byte)((int)(gradients[currentGradientStepIndex].g * 255)), (byte)((int)(gradients[currentGradientStepIndex].b * 255)));

			colorRect.Fill = new SolidColorBrush(color);

			Mouse.Capture(gradientCanvas);
			((UIElement)sender).Focus();
			isDragging = true;
		}


		private void GradientDragMouseMove(object sender, MouseEventArgs e)
		{
			if(isDragging)
			{
				Point coords = e.GetPosition(gradientCanvas);

				double xMin;
				double xMax;

				if(currentGradientStepIndex == 0)
				{
					xMin = 0.0;
					xMax = Canvas.GetLeft(gradientCanvas.Children[1]) + 2.75;
				}
				else if(currentGradientStepIndex == gradientSize - 1)
				{
					xMin = Canvas.GetLeft(gradientCanvas.Children[gradientSize - 2]) + 7.25;
					xMax = 450.0;
				}
				else
				{
					xMin = Canvas.GetLeft(gradientCanvas.Children[currentGradientStepIndex - 1]) + 7.25;
					xMax = Canvas.GetLeft(gradientCanvas.Children[currentGradientStepIndex + 1]) + 2.75;
				}

				double x = Math.Min(Math.Max(coords.X, xMin), xMax);
				Canvas.SetLeft(gradientCanvas.Children[currentGradientStepIndex], x - 5.0);

				float k = (float)(x / 450.0);
				gradients[currentGradientStepIndex].k = k;
				gradientStops[currentGradientStepIndex].Offset = k;

				kTextBox.Text = k.ToString("F6").Replace(",", ".");
			}
		}


		private void GradientDragMouseUp(object sender, MouseButtonEventArgs e)
		{
			isDragging = false;
			Mouse.Capture(null);
		}


		private void CreateGradientStep(object sender, MouseButtonEventArgs e)
		{
			UpdateCurrentGradientStep();

			if(gradientSize <= 100)
			{
				Point coords = e.GetPosition(gradientCanvas);

				float k = (float)(Math.Min(Math.Max(coords.X / 450.0, 0.0), 1.0));

				int insertionIndex;
				for(insertionIndex = 0; insertionIndex < gradientSize; insertionIndex++)
				{
					if(k < gradients[insertionIndex].k)
					{
						break;
					}
				}

				float r;
				float g;
				float b;

				if(insertionIndex == 0)
				{
					r = gradients[0].r;
					g = gradients[0].g;
					b = gradients[0].b;
				}
				else if(insertionIndex == gradientSize)
				{
					r = gradients[gradientSize - 1].r;
					g = gradients[gradientSize - 1].g;
					b = gradients[gradientSize - 1].b;
				}
				else
				{
					Grad grad1 = gradients[insertionIndex - 1];
					Grad grad2 = gradients[insertionIndex];
					float t = (k - grad1.k) / (grad2.k - grad1.k);

					r = (1.0f - t) * grad1.r + t * grad2.r;
					g = (1.0f - t) * grad1.g + t * grad2.g;
					b = (1.0f - t) * grad1.b + t * grad2.b;
				}

				gradients.Insert(insertionIndex, new Grad(k, r, g, b));

				Color color = Color.FromArgb((byte)255, (byte)((int)(r * 255)), (byte)((int)(g * 255)), (byte)((int)(b * 255)));

				gradientStops.Insert(insertionIndex, new GradientStop(color, k));

				((Ellipse)gradientCanvas.Children[currentGradientStepIndex]).Stroke = new SolidColorBrush(Colors.DarkRed);
				currentGradientStepIndex = insertionIndex;

				Ellipse ellipse = new Ellipse()
				{
					Width = 10.0,
					Height = 10.0,
					Fill = new SolidColorBrush(color),
					Stroke = new SolidColorBrush(Colors.White),
					StrokeThickness = 1.0,
					Focusable = true
				};
				ellipse.MouseDown += GradientDragMouseDown;

				gradientCanvas.Children.Insert(insertionIndex, ellipse);
				Canvas.SetLeft(ellipse, k * 450.0 - 5.0);

				gradientSize++;

				kTextBox.Text = k.ToString("F6").Replace(",", ".");
				redColorTextBox.Text = r.ToString("F6").Replace(",", ".");
				greenColorTextBox.Text = g.ToString("F6").Replace(",", ".");
				blueColorTextBox.Text = b.ToString("F6").Replace(",", ".");

				colorRect.Fill = new SolidColorBrush(color);
			}

			((UIElement) sender).Focus();
		}


		private void DeleteGradientStep(object sender, KeyEventArgs e)
		{
			if(e.Key == Key.Delete)
			{
				if(gradientSize > 2)
				{
					UpdateCurrentGradientStep();

					gradients.RemoveAt(currentGradientStepIndex);
					gradientStops.RemoveAt(currentGradientStepIndex);
					gradientCanvas.Children.RemoveAt(currentGradientStepIndex);

					currentGradientStepIndex = Math.Max(currentGradientStepIndex - 1, 1);
					((Ellipse)gradientCanvas.Children[currentGradientStepIndex]).Stroke = new SolidColorBrush(Colors.White);

					kTextBox.Text = gradients[currentGradientStepIndex].k.ToString("F6").Replace(",", ".");
					redColorTextBox.Text = gradients[currentGradientStepIndex].r.ToString("F6").Replace(",", ".");
					greenColorTextBox.Text = gradients[currentGradientStepIndex].g.ToString("F6").Replace(",", ".");
					blueColorTextBox.Text = gradients[currentGradientStepIndex].b.ToString("F6").Replace(",", ".");

					Color color = Color.FromArgb((byte)255, (byte)((int)(gradients[currentGradientStepIndex].r * 255)), (byte)((int)(gradients[currentGradientStepIndex].g * 255)), (byte)((int)(gradients[currentGradientStepIndex].b * 255)));

					colorRect.Fill = new SolidColorBrush(color);

					gradientSize--;
				}

				((UIElement)sender).Focus();
			}
		}


		private void TextBoxFocusLost(object sender, RoutedEventArgs e)
		{
			UpdateCurrentGradientStep();
		}


		private void SelectText(object sender, KeyboardFocusChangedEventArgs e)
		{
			TextBox textBox = (TextBox)sender;
			textBox.SelectAll();
		}


		private void SelectText(object sender, MouseButtonEventArgs e)
		{
			TextBox textBox = (TextBox)sender;
			textBox.SelectAll();
		}


		private void IgnoreMouseButton(object sender, MouseButtonEventArgs e)
		{
			TextBox textBox = (TextBox)sender;
			if(textBox == null || textBox.IsKeyboardFocusWithin)
			{
				return;
			}

			e.Handled = true;
			textBox.Focus();
		}
	}
}
