   M S B A M L   `   `   `  ���� U  QProceduralTextureGenerator, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullG(clr-namespace:ProceduralTextureGeneratorProceduralTextureGenerator  3    -ProceduralTextureGenerator.SaveMaterialDialog   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     @d2http://schemas.microsoft.com/expression/blend/2008     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     :local(clr-namespace:ProceduralTextureGenerator       � Title$  Save Material��   DialogWindowStyle#��[  $	��255��$	��350��.���  � RowDefinitions �� $��38 ��� $��48 ��� $��38 ��� $��48 ��� $��* �.+��� $��Destination��$��Segoe UI)�$��20(�
�������$
��Left=�$��20,0,0,0q�$��0���� -    ��Name$ functionGraphNameTextBox�� �� TextAlignment$
 Left=�$��Segoe UI)�$��18(�$	��250��$��32��
����������	#FF202020
�������$
��Left=�$	��Top=�$��	40,10,0,0q�$��1���� -    ��Name$# openFolderBrowserDialogButton��$��Segoe UI)�$��20(�$	��...��$��40��$��32��$
��Left=�$	��Top=�
����������	#FF202020
�������$��
300,10,0,0q�$��1���� $��Format��$��Segoe UI)�$��20(�
�������$
��Left=�$��20,0,0,0q�$��2���� -    ��Name$ formatComboBox��  ParameterComboBoxStyle#��[ $��18(�$	��300��$��32��$
��Left=�$	��Top=�$��	40,10,0,0q�$��3���� -   $��OK�� �� 	IsDefault . $��Segoe UI)�$��16(�$	��100��$��28��
����������	#FF202020
�������$��Right=�$	��Top=�$��
0,10,115,0q�$��4���� $��Cancel�� �� IsCancel . $��Segoe UI)�$��16(�$	��100��$��28��
����������	#FF202020
�������$��Right=�$	��Top=�$��	0,10,10,0q�$��4��