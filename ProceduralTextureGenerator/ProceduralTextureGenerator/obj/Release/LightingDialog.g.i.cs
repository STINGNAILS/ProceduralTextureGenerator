#pragma checksum "..\..\LightingDialog.xaml" "{ff1816ec-aa5e-4d10-87f7-6f4963833460}" "F8E120B0413FE35F08900202BD35440E9057C46B"
//------------------------------------------------------------------------------
// <auto-generated>
//     Этот код создан программой.
//     Исполняемая версия:4.0.30319.42000
//
//     Изменения в этом файле могут привести к неправильной работе и будут потеряны в случае
//     повторной генерации кода.
// </auto-generated>
//------------------------------------------------------------------------------

using ProceduralTextureGenerator;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms.Integration;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Shell;


namespace ProceduralTextureGenerator
{


	/// <summary>
	/// LightingDialog
	/// </summary>
	public partial class LightingDialog : System.Windows.Window, System.Windows.Markup.IComponentConnector
	{

#line default
#line hidden

		private bool _contentLoaded;

		/// <summary>
		/// InitializeComponent
		/// </summary>
		[System.Diagnostics.DebuggerNonUserCodeAttribute()]
		[System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
		public void InitializeComponent()
		{
			if(_contentLoaded)
			{
				return;
			}
			_contentLoaded = true;
			System.Uri resourceLocater = new System.Uri("/ProceduralTextureGenerator;component/lightingdialog.xaml", System.UriKind.Relative);

#line 1 "..\..\LightingDialog.xaml"
			System.Windows.Application.LoadComponent(this, resourceLocater);

#line default
#line hidden
		}

		[System.Diagnostics.DebuggerNonUserCodeAttribute()]
		[System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
		[System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
		[System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
		[System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
		[System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
		void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target)
		{
			switch(connectionId)
			{
				case 1:
				this.sphereLight1IsEnabledCheckBox = ((System.Windows.Controls.CheckBox)(target));
				return;
				case 2:
				this.sphereLight1StackPanel = ((System.Windows.Controls.StackPanel)(target));
				return;
				case 3:
				this.redColorSph1Label = ((System.Windows.Controls.Label)(target));
				return;
				case 4:
				this.redColorSph1TextBox = ((System.Windows.Controls.TextBox)(target));
				return;
				case 5:
				this.greenColorSph1Label = ((System.Windows.Controls.Label)(target));
				return;
				case 6:
				this.greenColorSph1TextBox = ((System.Windows.Controls.TextBox)(target));
				return;
				case 7:
				this.blueColorSph1Label = ((System.Windows.Controls.Label)(target));
				return;
				case 8:
				this.blueColorSph1TextBox = ((System.Windows.Controls.TextBox)(target));
				return;
				case 9:
				this.colorSph1Rect = ((System.Windows.Shapes.Rectangle)(target));
				return;
				case 10:
				this.intensitySph1TextBox = ((System.Windows.Controls.TextBox)(target));
				return;
				case 11:
				this.lightRadiusSph1TextBox = ((System.Windows.Controls.TextBox)(target));
				return;
				case 12:
				this.sourceRadiusSph1TextBox = ((System.Windows.Controls.TextBox)(target));
				return;
				case 13:
				this.xSph1Label = ((System.Windows.Controls.Label)(target));
				return;
				case 14:
				this.xSph1TextBox = ((System.Windows.Controls.TextBox)(target));
				return;
				case 15:
				this.ySph1Label = ((System.Windows.Controls.Label)(target));
				return;
				case 16:
				this.ySph1TextBox = ((System.Windows.Controls.TextBox)(target));
				return;
				case 17:
				this.zSph1Label = ((System.Windows.Controls.Label)(target));
				return;
				case 18:
				this.zSph1TextBox = ((System.Windows.Controls.TextBox)(target));
				return;
			}
			this._contentLoaded = true;
		}

		internal ProceduralTextureGenerator.DirectionalLightParameters directionalLightParameters;
		internal ProceduralTextureGenerator.SphereLightParameters sphereLight1Parameters;
		internal ProceduralTextureGenerator.SphereLightParameters sphereLight2Parameters;
		internal ProceduralTextureGenerator.SphereLightParameters sphereLight3Parameters;
		internal ProceduralTextureGenerator.SphereLightParameters sphereLight4Parameters;
	}
}
