   M S B A M L   `   `   `  ����U  QProceduralTextureGenerator, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullG(clr-namespace:ProceduralTextureGeneratorProceduralTextureGenerator  5   	   3    -ProceduralTextureGenerator.SaveMaterialDialog   5   	   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   :local(clr-namespace:ProceduralTextureGenerator     5   	     � Title$  Save Material��5   	      DialogWindowStyle#��[  6   $	��255��6J   $	��350��6W   .��5	      �  � RowDefinitions 5
   
   �� 5      $��38 �6   �� 5      $��48 �6   �� 5      $��38 �6   �� 5      $��48 �6   �� 5      $��* �6   .+�5   
   �� $��Destination��6   $��Segoe UI)�6&   $��20(�6<   
�������$
��Left=�6^   $��20,0,0,0q�6y   $��0��6�   �� 5   
   -   5         ��Name$ functionGraphNameTextBox��5       �� TextAlignment$
 Left=�64   $��Segoe UI)�6I   $��18(�6_   $	��250��6m   $��32��6y   
����������	#FF202020
�������$
��Left=�6�   $	��Top=�6�   $��	40,10,0,0q�6�   $��1��6  �� 5   
   -   5         ��Name$# openFolderBrowserDialogButton��5      $��Segoe UI)�6X   $��20(�6n   $	��...��6|   $��40��6�   $��32��6�   $
��Left=�6�   $	��Top=�6�   
����������	#FF202020
�������$��
300,10,0,0q�6  $��1��6(  �� 5   
   $��Format��6   $��Segoe UI)�6!   $��20(�67   
�������$
��Left=�6Y   $��20,0,0,0q�6t   $��2��6�   �� 5   
   -   5         ��Name$ formatComboBox��5        ParameterComboBoxStyle#��[ 6+   $��18(�6[   $	��300��6i   $��32��6u   $
��Left=�6�   $	��Top=�6�   $��	40,10,0,0q�6�   $��3��6�   �� 5   
   -   5        $��OK��5       �� 	IsDefault . $��Segoe UI)�6F   $��16(�6\   $	��100��6j   $��28��6v   
����������	#FF202020
�������$��Right=�6�   $	��Top=�6�   $��
0,10,115,0q�6�   $��4��6
  �� 5   
   $��Cancel��6    �� IsCancel . $��Segoe UI)�62   $��16(�6H   $	��100��6V   $��28��6b   
����������	#FF202020
�������$��Right=�6�   $	��Top=�6�   $��	0,10,10,0q�6�   $��4��6�   5      5      