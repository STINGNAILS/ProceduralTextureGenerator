   M S B A M L   `   `   `  ���� U  QProceduralTextureGenerator, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullG(clr-namespace:ProceduralTextureGeneratorProceduralTextureGenerator  ;    5ProceduralTextureGenerator.DirectionalLightParameters   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     @d2http://schemas.microsoft.com/expression/blend/2008     :local(clr-namespace:ProceduralTextureGenerator     .���� .+�� .+��� $��Directional Light��$��Segoe UI Bold)�$��20(�$
��Left=�$	��200��
�������$
��Left=�$��10,0,0,0q��� $��
Is Enabled��$��Segoe UI Italic)�$��14(�
�������$��Right=�$��Center=�$��0,2,25,0q��� -     ��Name$  isEnabledCheckBox��$��Center=�
�������
�������$��Right=�$��0,4,10,0q��� -    ��Name$  directionalLightStackPanel��$y�Visible=�.+��� $��Color��$��Segoe UI)�$��20(�
�������$
��Left=�$��20,0,0,0q��  � RowDefinitions �� $��32 ��� $��32 ��� $��32 � � ColumnDefinitions �� $��50 ��� $��1* ��� $	��104 �.+��� -    ��Name$ redColorLabel��   IndicatorLabelStyle#��[  $��R��$��0��$��0���� -    ��Name$ redColorTextBox��$��24��$��Segoe UI)�$��14(�
����������	#FF202020
�������$��2,0,2,0q� �� TextAlignment$ Center=�$��0��$��1���� -   $ greenColorLabel��#��[  $��G��$��1��$��0���� -   $ greenColorTextBox��$��24��$��Segoe UI)�$��14(�
����������	#FF202020
�������$��2,0,2,0q�$ Center=�$��1��$��1���� -   $ blueColorLabel��#��[  $��B��$��2��$��0���� -   $ blueColorTextBox��$��24��$��Segoe UI)�$��14(�
����������	#FF202020
�������$��2,0,2,0q�$ Center=�$��2��$��1���� -	    ��Name$ 	colorRect��
�������$��2,2,10,2q�$��2��$��0��$��3���� $��	Intensity��$��Segoe UI)�$��20(�
�������$
��Left=�$��20,0,0,0q��� -
   $ intensityTextBox��$
 Left=�$��Segoe UI)�$��14(�$��24��
����������	#FF202020
�������$	��Top=�$��	40,5,10,5q��� $��	Direction��$��Segoe UI)�$��20(�
�������$
��Left=�$��20,0,0,0q��  �� $��32 ��� $��32 � �� $��50 ��� $��1* �.+��� -   $ phiLabel��#��[  $��φ��$��0��$��0���� -   $ 
phiTextBox��$��24��$��Segoe UI)�$��14(�
����������	#FF202020
�������$��2,0,10,0q�$ Center=�$��0��$��1���� -   $ psiLabel��#��[  $��ψ��$��1��$��0���� -   $ 
psiTextBox��$��24��$��Segoe UI)�$��14(�
����������	#FF202020
�������$��2,0,10,0q�$ Center=�$��1��$��1��