   M S B A M L   `   `   `  ���� U  QProceduralTextureGenerator, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullG(clr-namespace:ProceduralTextureGeneratorProceduralTextureGenerator  /    )ProceduralTextureGenerator.GradientDialog   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     @d2http://schemas.microsoft.com/expression/blend/2008     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     :local(clr-namespace:ProceduralTextureGenerator       � Title$  Gradient Editor��   DialogWindowStyle#��[  $	��352��$	��500��.���  � RowDefinitions �� $��38 ��� $��48 ��� $��38 ��� $	��142 ��� $��* �.+��� $��Gradient��$��Segoe UI)�$��20(�
�������$
��Left=�$��20,0,0,0q�$��0���� $	��450��$��32��$
��Left=�$	��Top=�$��1q�
�������$��	40,10,0,0q�$��1��._��� -    ��Name$ gradientRectangle���� -    ��Name$ gradientCanvas��$	��450��$��10��$
��Left=�$	��Top=�$��1��$��	40,35,0,0q� K� ZIndex$ 1���� $��Gradient Key��$��Segoe UI)�$��20(�
�������$
��Left=�$��20,0,0,0q�$��2��� $��3�� �� $��32 ��� $��32 ��� $��32 ��� $��32 ��� $��8 � � ColumnDefinitions �� $��60 ��� $��1* ��� $	��102 �.+��� -    ��Name$ kLabel��  IndicatorLabelStyle#��[ $��K��$��0��$��0���� -    ��Name$ kTextBox��$��24��$��Segoe UI)�$��14(�
����������	#FF202020
�������$��2,0,2,0q� �� TextAlignment$ Center=�$��0��$��1���� -   $ redColorLabel��#��[ $��R��$��1��$��0���� -   $ redColorTextBox��$��24��$��Segoe UI)�$��14(�
����������	#FF202020
�������$��2,0,2,0q�$ Center=�$��1��$��1���� -   $ greenColorLabel��#��[ $��G��$��2��$��0���� -   $ greenColorTextBox��$��24��$��Segoe UI)�$��14(�
����������	#FF202020
�������$��2,0,2,0q�$ Center=�$��2��$��1���� -	   $ blueColorLabel��#��[ $��B��$��3��$��0���� -
   $ blueColorTextBox��$��24��$��Segoe UI)�$��14(�
����������	#FF202020
�������$��2,0,2,0q�$ Center=�$��3��$��1���� -   $ 	colorRect��
�������$��2,2,10,2q�$��1��$��2��$��3���� -   $��OK��	 �� 	IsDefault	 . $��Segoe UI)�$��16(�$	��100��$��28��
����������	#FF202020
�������$��Right=�$	��Top=�$��	0,10,10,0q�$��4��