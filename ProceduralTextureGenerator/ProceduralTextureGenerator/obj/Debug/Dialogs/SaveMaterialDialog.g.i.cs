﻿#pragma checksum "..\..\..\Dialogs\SaveMaterialDialog.xaml" "{ff1816ec-aa5e-4d10-87f7-6f4963833460}" "90534D164BFA6A4D05D88DACFA23B4EFBA107C60"
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


namespace ProceduralTextureGenerator {
    
    
    /// <summary>
    /// SaveMaterialDialog
    /// </summary>
    public partial class SaveMaterialDialog : System.Windows.Window, System.Windows.Markup.IComponentConnector {
        
        
        #line 19 "..\..\..\Dialogs\SaveMaterialDialog.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox functionGraphNameTextBox;
        
        #line default
        #line hidden
        
        
        #line 20 "..\..\..\Dialogs\SaveMaterialDialog.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button openFolderBrowserDialogButton;
        
        #line default
        #line hidden
        
        
        #line 23 "..\..\..\Dialogs\SaveMaterialDialog.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.ComboBox formatComboBox;
        
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
            System.Uri resourceLocater = new System.Uri("/ProceduralTextureGenerator;component/dialogs/savematerialdialog.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\Dialogs\SaveMaterialDialog.xaml"
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
            this.functionGraphNameTextBox = ((System.Windows.Controls.TextBox)(target));
            return;
            case 2:
            this.openFolderBrowserDialogButton = ((System.Windows.Controls.Button)(target));
            
            #line 20 "..\..\..\Dialogs\SaveMaterialDialog.xaml"
            this.openFolderBrowserDialogButton.Click += new System.Windows.RoutedEventHandler(this.OpenFolderBrowserDialog);
            
            #line default
            #line hidden
            return;
            case 3:
            this.formatComboBox = ((System.Windows.Controls.ComboBox)(target));
            return;
            case 4:
            
            #line 25 "..\..\..\Dialogs\SaveMaterialDialog.xaml"
            ((System.Windows.Controls.Button)(target)).Click += new System.Windows.RoutedEventHandler(this.AcceptMaterial);
            
            #line default
            #line hidden
            return;
            }
            this._contentLoaded = true;
        }
    }
}

