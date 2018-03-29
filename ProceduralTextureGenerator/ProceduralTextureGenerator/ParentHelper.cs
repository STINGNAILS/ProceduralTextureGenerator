using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace ProceduralTextureGenerator
{
    static class ParentHelper
    {
		public static ParameterPanel GetParentParameterPanel(DependencyObject child)
		{
			DependencyObject parent = VisualTreeHelper.GetParent(child);

			if(parent == null)
			{
				return null;
			}

			ParameterPanel parentParameterpanel = parent as ParameterPanel;
			if(parentParameterpanel != null)
			{
				return parentParameterpanel;
			}
			else
			{
				return GetParentParameterPanel(parent);
			}
		}
    }
}
