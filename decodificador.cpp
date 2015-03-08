/* Codificador de texto dentro de imagenes BMP y JPG.
Copyright © 2011 Ángel Luis Perales Gómez
Este programa es software libre: usted puede redistribuirlo y/o modificarlo
conforme a los términos de la Licencia Pública General de GNU publicada por
la Fundación para el Software Libre, ya sea la versión 3 de esta Licencia
o (a su elección) cualquier versión posterior.
Este programa se distribuye con el deseo de que le resulte útil,
pero SIN GARANTÍAS DE NINGÚN TIPO; ni siquiera con las garantías implícitas de
COMERCIABILIDAD o APTITUD PARA UN PROPÓSITO DETERMINADO. Para más información,
consulte la Licencia Pública General de GNU.
Junto con este programa, se debería incluir una copia de la
Licencia Pública General de GNU.
De no ser así, ingrese en <http://www.gnu.org/licenses/>.*/
#include<iostream>
#include<string>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
struct Byte1 {
    unsigned int b0:1;
    unsigned int b1:1;
    unsigned int b2:1;
    unsigned int b3:1;
    unsigned int b4:1;
    unsigned int b5:1;
    unsigned int b6:1;
    unsigned int b7:1;
}bits_imagen, bits_texto;
bool isBMP(FILE * fd)
{
    if (fgetc(fd) != 'B')
    return false;
    if (fgetc(fd) != 'M')
    return false;
    return true;
}
bool is24(FILE *fd)
{
    fseek(fd,27, SEEK_SET);
    char caracter;
    caracter = fgetc(fd);
    if (caracter == 24)
    return true;
    caracter = fgetc(fd);
    if (caracter == 24)
    return true;
    return false;
}
bool isJPG(FILE * fd)
{
    fseek(fd,0, SEEK_SET);
    if (fgetc(fd) != 0xFF)
    return false;
    if (fgetc(fd) != 0xD8)
    return false;
    if (fgetc(fd) != 0xFF)
    return false;
    return true;
}
bool isCodificada(FILE *fd, int pos)
{
    int byte_imagen;
    int byte_texto=0;
    int caracter=0;
    //fseek(fd,10, SEEK_SET);
    //fseek(fd,54, SEEK_SET);
    fseek(fd,pos, SEEK_SET);
    for (int i= 0; i<8; i++)
    {
        byte_imagen = fgetc(fd);
        memcpy(&bits_imagen, &byte_imagen,1);
        bits_texto.b7 = bits_imagen.b0;
        //cout << bits_texto.b7 << bits_texto.b6 << bits_texto.b5 << bits_texto.b4 << " " << bits_texto.b3 << bits_texto.b2 << bits_texto.b1 << bits_texto.b0 << endl;
        if (i != 7)
        {
            memcpy(&byte_texto, &bits_texto,1);
            byte_texto >>=1;
            memcpy(&bits_texto, &byte_texto,1);
        }
    }
    memcpy(&byte_texto, &bits_texto,1);
    //printf("VALOR: %Xn", byte_texto);
    memcpy(&caracter, &byte_texto,1);
    if (caracter==178)
    return true;
    return false;
}
int decodificar(FILE *fd_imagen, int pos)
{
    int byte_imagen;
    int byte_texto=0;
    int caracter=0;
    bool marca_fin = false;
    //fseek(fd_imagen,62, SEEK_SET);
    //fseek(fd_imagen,18, SEEK_SET);
    fseek(fd_imagen,pos, SEEK_SET);
    while (!marca_fin)
    {
        for (int i= 0; i<8; i++)
        {
            byte_imagen = fgetc(fd_imagen);
            memcpy(&bits_imagen, &byte_imagen,1);
            bits_texto.b7 = bits_imagen.b0;
            if (i != 7)
            {
                memcpy(&byte_texto, &bits_texto,1);
                byte_texto >>=1;
                memcpy(&bits_texto, &byte_texto,1);
            }
        }
        memcpy(&byte_texto, &bits_texto,1);
        memcpy(&caracter, &byte_texto,1);
        if (caracter==178)
        marca_fin = true;
        else
        printf("%c", caracter);
    }
    return 0;
}
int main()
{
    string ruta_imagen;
    FILE *fd_imagen;
    bool isbBMP = false, isbJPG = false;
    char opcion;
    cout << "Bienvenido al programa, elija un archivo BMPn";
    cin >> ruta_imagen;
    const char * ruta_imagen_char = ruta_imagen.c_str();
    fd_imagen = fopen(ruta_imagen_char, "r");
    if (fd_imagen == NULL)
    {
        cout << "No se puede abrir el archivon";
        exit(1);
    }
    if (isBMP(fd_imagen)==true)
    {
        cout << "El archivo de imagen es un archivo BMPn";
        isbBMP = true;
        if (isCodificada(fd_imagen, 54)==false)
        {
            cout << "El archivo de imagen no esta codificadon";
            exit(0);
        }
        else
        {
            cout << "Este archivo de imagen tiene oculto en su interior un texto codificado ¿Quieres leer el texto?(s/n)";
            cin >> opcion;
            if(opcion == 's')
            {
                //if (is24(fd_imagen)==false)
                //cout << "El archivo de imagen no es un archivo con 24 bits de profundidadn";
                cout << "===================INICIO DEL TEXTO OCULTO======================n";
                decodificar(fd_imagen, 62);
                cout << "===================FIN DEL TEXTO OCULTO======================n";
            }
            else
            exit(0);
        }
    }
    else if (isJPG(fd_imagen)==true)
    {
        cout << "El archivo de imagen es un archivo JPGn";
        isbJPG = true;
        if (isCodificada(fd_imagen, 10)==false)
        {
            cout << "El archivo de imagen no esta codificadon";
            exit(0);
        }
        else
        {
            cout << "Este archivo de imagen tiene oculto en su interior un texto codificado ¿Quieres leer el texto?(s/n) ";
            cin >> opcion;
            if(opcion == 's')
            {
                //if (is24(fd_imagen)==false)
                //cout << "El archivo de imagen no es un archivo con 24 bits de profundidadn";
                cout << "===================INICIO DEL TEXTO OCULTO======================n";
                decodificar(fd_imagen, 18);
                cout << "===================FIN DEL TEXTO OCULTO======================n";
            }
            else
            exit(0);
        }
    }
    else
    {
        cout << "El archivo no está soportado actualmente por el programan";
        exit(1);
    }
    fclose(fd_imagen);
    cout << "================CREDITOS=================n";
    cout << "=Programado por Ángel Luis Perales Gómez=n";
    cout << "=tFacultad de Informática t=n";
    cout << "=tUniversidad de Murcia tt=n";
    cout << "=tBajo licencia GPL tt=n";
    cout << "=========================================tn";
    return 0;
}
