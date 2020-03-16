

/* 
 * File:   proxecto
 * Author: Abraham Carrera Groba
 *
 * Created on 1 de marzo de 2016, 16:14
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <opencv/cv.h>    
#include <opencv/cxcore.h>      
#include <opencv/highgui.h>  

#define FILAMODELO 10
#define COLUMNAMODELO 10


void copiarPixel(int i, int j, IplImage* Img, IplImage * Img2) { // nos permite copiar un pixel en la misma posición de otra imagen
    //punteros señalando a la misma posición especificada por los parámetros i, j de las distintas imágenes
    unsigned char *pImg = (unsigned char *) Img->imageData + i  * (Img->widthStep)  + j  * Img->nChannels;
    unsigned char *pImg2 = (unsigned char *) Img2->imageData + i  * (Img2->widthStep)  + j  * Img2->nChannels;

            *pImg2++ = *pImg++;
            *pImg2++ = *pImg++;

            *pImg2 = *pImg;

        


}

int compararPixel(int i, int j, IplImage* Img,int bluemodel, int greenmodel, int redmodel) {
    //nos devuelve la diferencia en valor absoluto entre los componentes de color de un pixel (i,j) y unas dadas como modelo 
    int  toret;
    toret = 0;


        unsigned char *pImg = (unsigned char *) Img->imageData + i  * (Img->widthStep)  + j  * Img->nChannels;
       

      

            //calculamos la diferencia de cada componente de color
            toret += abs(bluemodel - *pImg++);
            toret += abs(greenmodel - *pImg++);
            toret += abs(redmodel - *pImg);



      
    return toret;

}

int main(int argc, char** argv) {
struct timespec start, finish;
float elapsed;
//empezamos a contar el tiempo
clock_gettime(CLOCK_MONOTONIC, &start);

    //comprueba el número de argumentos
    if (argc != 3) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }
    IplImage* Img = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);
    // Comprueba que tenemos imagen 
    if (!Img) {
        fprintf(stderr, "Cannot open FICHERO FONDO!\n");
        return 1;
    }
    int key = 0;

    // Inicializa cámara e imagen OpenCV  
    
    CvCapture* capture = cvCaptureFromAVI("chroma.avi");
    IplImage* frame = cvQueryFrame(capture);

    // Comprueba que se puede abrir el video   
    if (!capture) {
        fprintf(stderr, "Cannot open AVI!\n");
        return 1;
    }

    // Obtiene la información de fps, necesaria para establecer el delay
    int fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);

    // Crea una ventana para mostrar el video 
    cvNamedWindow("VIDEO (pulse x para salir)", CV_WINDOW_AUTOSIZE);
    int a;
    a=0;
    //Descartamos los tres primeros frames en negro
    for ( a=0;a<3;a++){
        frame = cvQueryFrame(capture);
    }
    //usamos el píxel fila 10, columna 10 del cuarto frame como modelo
    frame = cvQueryFrame(capture);
    int azulModelo, rojoModelo, verdeModelo;
    azulModelo=0;
    verdeModelo=0;
    rojoModelo=0;
   
    
    unsigned char *pframe = (unsigned char *) frame->imageData + FILAMODELO  * (frame->widthStep)  + COLUMNAMODELO  * frame->nChannels;
    azulModelo= *pframe++;
    verdeModelo=*pframe++;
    rojoModelo=*pframe;
    //PROCESADO DE LOS FRAMES:
    int f,c;
    f=0;
    c=0;
     
    while (key != 'x') { //seguimos procesando mientras el usuario no pulse 'x'
         
        
        
        
        for (f = 0; f < (frame->height ); f++) { //recorremos todos los pixeles
            for (c = 0; c < (frame->width ); c++) {
                //comparando con el modelo, si la diferencia <200  copiamos el fondo en el frame
                if (compararPixel (f,c,frame,azulModelo, verdeModelo, rojoModelo)<200){
                    copiarPixel (f,c,Img,frame);
                }


            }
        }
        

        // mostramos el frame
        cvShowImage("VIDEO (pulse x para salir)", frame);

        // se sale si el usuario pulsa 'x'         
        key = cvWaitKey(1000 / fps);
        //cogemos el siguiente frame para procesarlo
        frame = cvQueryFrame(capture);
        if (!frame) break; //también se sale cuando ya no hay más frames
    }

    // Borrado  
    cvDestroyWindow("VIDEO (pulse x para salir)");
    cvReleaseCapture(&capture);
    //devolvemos el tiempo empleado
clock_gettime(CLOCK_MONOTONIC, &finish);
elapsed = (finish.tv_sec - start.tv_sec);
elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
printf("Tiempo transcurrido: %f", elapsed);

    return 0;
} 
