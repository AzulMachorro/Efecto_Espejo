#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 1

int main()
{
    FILE *image, *outputImage, *lecturas;
    image = fopen("10.bmp","rb");          //Imagen original a transformar
    outputImage = fopen("img3.bmp","wb");    //Imagen transformada
    long ancho;
    long alto;
    unsigned char r, g, b;               //Color del Pixel
    unsigned char* ptr;

    //defini el 100 ya que asi se lograba una mejor nitidez en la imagen
    unsigned char xx[100];
    int cuenta = 0;
    for(int i=0; i<100; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    printf("largo img %li\n",alto);
    printf("ancho img %li\n",ancho);

    ptr = (unsigned char*)malloc(alto*ancho*3* sizeof(unsigned char));

    while(!feof(image)){
      b = fgetc(image);
      g = fgetc(image);
      r = fgetc(image);


      unsigned char pixel = 0.21*r+0.72*g+0.07*b;

      ptr[cuenta] = b; //b
      ptr[cuenta+1] = g; //g
      ptr[cuenta+2] = r; //r


      cuenta++;

    }                                        //Grises
    //en ancho se multiplica por dos para obtener mejor resultado en el acomodo de los pixeles
    cuenta = ancho*2;
    #pragma omp parallel
    {
      #pragma omp for schedule (dynamic)
    for (int i = 0; i < alto*ancho*3; ++i) {
        // en esta parte tambien modifique algunas operaciones para que los pixeles siguieran un orden al acomodarce y no se generaran desplazamientos
      fputc(ptr[i+(cuenta+cuenta+2)], outputImage);
      fputc(ptr[i+(cuenta+cuenta+1)+1], outputImage);
      fputc(ptr[i+(cuenta+cuenta)+2], outputImage);
      cuenta--;
      if (cuenta == 0){
        cuenta = ancho*2;
      }
    }
    }

    free(ptr);
    fclose(image);
    fclose(outputImage);
    return 0;
}
