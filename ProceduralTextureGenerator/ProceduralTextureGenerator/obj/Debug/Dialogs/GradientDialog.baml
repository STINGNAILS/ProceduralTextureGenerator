   M S B A M L   `   `   `  ����U  QProceduralTextureGenerator, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullG(clr-namespace:ProceduralTextureGeneratorProceduralTextureGenerator  5   	   /    )ProceduralTextureGenerator.GradientDialog   5   	   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   :local(clr-namespace:ProceduralTextureGenerator     5   	     � Title$  Gradient Editor��5   	      DialogWindowStyle#��[  6!   $	��352��6L   $	��500��6Y   .��5	      �  � RowDefinitions 5
   
   �� 5      $��38 �6   �� 5      $��48 �6   �� 5      $��38 �6   �� 5      $	��142 �6   �� 5      $��* �6   .+�5   
   �� $��Gradient��6   $��Segoe UI)�6#   $��20(�69   
�������$
��Left=�6[   $��20,0,0,0q�6v   $��0��6�   �� 5   
   $	��450��6   $��32��6   $
��Left=�6)   $	��Top=�6D   $��1q�6\   
�������$��	40,10,0,0q�6�   $��1��6�   ._�5      �� -   5         ��Name$ gradientRectangle��5      5      �� 5   
   -   5         ��Name$ gradientCanvas��5      $	��450��6)   $��10��65   $
��Left=�6A   $	��Top=�6\   $��1��6t   $��	40,35,0,0q�6�    K� ZIndex$ 1��6�   �� 5   
   $��Gradient Key��6   $��Segoe UI)�6'   $��20(�6=   
�������$
��Left=�6_   $��20,0,0,0q�6z   $��2��6�   � 5   
   $��3��6    5      �� 5      $��32 �6    �� 5      $��32 �6    �� 5      $��32 �6    �� 5      $��32 �6    �� 5      $��8 �6     � ColumnDefinitions 5!      �� 5"      $��60 �6#   �� 5#      $��1* �6#   �� 5$      $	��102 �6#   .+�5'      �� -   5         ��Name$ kLabel��5'        IndicatorLabelStyle#��[ 6$   $��K��6Q   $��0��6]   $��0��6j   �� 5(      -   5         ��Name$ kTextBox��5(      $��24��6(   $��Segoe UI)�64   $��14(�6J   
����������	#FF202020
�������$��2,0,2,0q�6�    �� TextAlignment$ Center=�6�   $��0��6�   $��1��6�   �� 5*      -   5        $ redColorLabel��5*      #��[ 6+   $��R��6X   $��1��6d   $��0��6q   �� 5+      -   5        $ redColorTextBox��5+      $��24��6/   $��Segoe UI)�6;   $��14(�6Q   
����������	#FF202020
�������$��2,0,2,0q�6�   $ Center=�6�   $��1��6�   $��1��6�   �� 5-      -   5        $ greenColorLabel��5-      #��[ 6-   $��G��6Z   $��2��6f   $��0��6s   �� 5.      -   5        $ greenColorTextBox��5.      $��24��61   $��Segoe UI)�6=   $��14(�6S   
����������	#FF202020
�������$��2,0,2,0q�6�   $ Center=�6�   $��2��6�   $��1��6�   �� 50      -	   5        $ blueColorLabel��50      #��[ 6,   $��B��6Y   $��3��6e   $��0��6r   �� 51      -
   5        $ blueColorTextBox��51      $��24��60   $��Segoe UI)�6<   $��14(�6R   
����������	#FF202020
�������$��2,0,2,0q�6�   $ Center=�6�   $��3��6�   $��1��6�   �� 53      -   5        $ 	colorRect��53      
�������$��2,2,10,2q�6;   $��1��6M   $��2��6Z   $��3��6j   54      �� 56   
   -   5        $��OK��56      	 �� 	IsDefault	 . $��Segoe UI)�6F   $��16(�6\   $	��100��6j   $��28��6v   
����������	#FF202020
�������$��Right=�6�   $	��Top=�6�   $��	0,10,10,0q�6�   $��4��6	  57      58      