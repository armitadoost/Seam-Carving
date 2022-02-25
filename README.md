# Seam-Carving
This project creates a data type that resizes a H-by-W image using seam-carving techniques. The technique includes computing the dual-gradient energy function, and then finding vertical “seams” – paths from the top to the bottom of the image – such that the sum of the dual-gradient energy values in the pixels along the path is as small as possible.

### Seam Carving Description
Seam-carving is a content-aware image resizing technique where the image is reduced in size by one pixel of width (or height) at a time.
A vertical seam in an image is a path of pixels connected from the top to the bottom with one pixel in each row; a horizontal seam is a path of pixels connected from the left to the right with one pixel in each column. 
Unlike standard content-agnostic resizing techniques (such as cropping and scaling), seam carving preserves the most interest features (aspect ratio, set of objects present, etc.) of the image. Although the underlying algorithm is simple and elegant, it was not discovered until 2007. Now, it is now a core feature in Adobe Photoshop and other computer graphics applications.

### Finding Seams
![image](https://user-images.githubusercontent.com/86267678/155808334-4af5b82a-53ed-49ce-be50-cde36521c5f3.png)

### Example
This image here was reduced in size by 30%
![image](https://user-images.githubusercontent.com/86267678/155808300-bbf535f1-00c8-4934-a033-3113ea4dd38c.png)
![image](https://user-images.githubusercontent.com/86267678/155808307-0c8eb84c-2c9e-42d0-a12b-00b8ba3a4af0.png)

