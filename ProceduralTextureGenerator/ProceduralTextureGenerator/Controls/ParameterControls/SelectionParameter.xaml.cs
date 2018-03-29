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

namespace ProceduralTextureGenerator
{
	/// <summary>
	/// Логика взаимодействия для SelectionParameter.xaml
	/// </summary>
	public partial class SelectionParameter : UserControl
	{
		private int index;

		private KeyValuePair<string, int>[] selections;


		public SelectionParameter(string name, int index_, KeyValuePair<string, int>[] selections_)
		{
			InitializeComponent();

			selectionParameterName.Content = name;

			index = index_;
			selections = selections_;

			int value = CoreDll.GraphViewGetSelectedNodeIntParameter(index);

			for(int i = 0; i < selections.Length; i++)
			{
				selectionParameterComboBox.Items.Add(selections[i].Key);

				if(selections[i].Value == value)
				{
					selectionParameterComboBox.SelectedIndex = i;
				}
			}

			selectionParameterComboBox.SelectionChanged += ComboBoxSelectionChanged;
		}


		private void ComboBoxSelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			for(int i = 0; i < selections.Length; i++)
			{
				if(selections[i].Key == (string)((ComboBox)sender).SelectedItem)
				{
					CoreDll.GraphViewSetSelectedNodeIntParameter(index, selections[i].Value);
					CoreDll.GraphViewProcess();

					break;
				}
			}

			ParentHelper.GetParentParameterPanel(this)?.Update();
		}
	}
}
