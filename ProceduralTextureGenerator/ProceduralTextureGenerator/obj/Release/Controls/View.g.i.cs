﻿#pragma checksum "..\..\..\Controls\View.xaml" "{ff1816ec-aa5e-4d10-87f7-6f4963833460}" "2690C8DB90491EB95D4D70EC53AB496F66868336"
//------------------------------------------------------------------------------
// <auto-generated>
//     Этот код создан программой.
//     Исполняемая версия:4.0.30319.42000
//
//     Изменения в этом файле могут привести к неправильной работе и будут потеряны в случае
//     повторной генерации кода.
// </auto-generated>
//------------------------------------------------------------------------------

using Microsoft.Wpf.Interop.DirectX;
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


namespace ProceduralTextureGenerator {
    
    
    /// <summary>
    /// View
    /// </summary>
    public partial class View : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        
        #line 11 "..\..\..\Controls\View.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Image image;
        
        #line default
        #line hidden
        
        
        #line 13 "..\..\..\Controls\View.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal Microsoft.Wpf.Interop.DirectX.D3D11Image directXView;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/ProceduralTextureGenerator;component/controls/view.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\Controls\View.xaml"
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
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            
            #line 9 "..\..\..\Controls\View.xaml"
            ((ProceduralTextureGenerator.View)(target)).Loaded += new System.Windows.RoutedEventHandler(this.OnLoaded);
            
            #line default
            #line hidden
            
            #line 9 "..\..\..\Controls\View.xaml"
            ((ProceduralTextureGenerator.View)(target)).SizeChanged += new System.Windows.SizeChangedEventHandler(this.OnResize);
            
            #line default
            #line hidden
            
            #line 9 "..\..\..\Controls\View.xaml"
            ((ProceduralTextureGenerator.View)(target)).MouseDown += new System.Windows.Input.MouseButtonEventHandler(this.OnMouseDown);
            
            #line default
            #line hidden
            
            #line 9 "..\..\..\Controls\View.xaml"
            ((ProceduralTextureGenerator.View)(target)).MouseUp += new System.Windows.Input.MouseButtonEventHandler(this.OnMouseUp);
            
            #line default
            #line hidden
            return;
            case 2:
            this.image = ((System.Windows.Controls.Image)(target));
            return;
            case 3:
            this.directXView = ((Microsoft.Wpf.Interop.DirectX.D3D11Image)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

