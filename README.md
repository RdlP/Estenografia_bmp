El programa codificador.cpp permite esconder un texto en una imagen BMP y por el contrario el programa decodificador permite decoficar el texto de una imagen, si es que esta poseía un texto oculto.

Para compilar el programa basta con ejecutar los siguientes comandos

g++ -o codificador codificador.cpp
g++ -o decodificador decodificador.cpp

y para ejecutarlos basta con ejecutar:

./codificador
./decodificador

Y seguir las instrucciones que aparecen en pantalla

¿Como trabaja el programa?

Este programa únicamente va codificando cada bit de texto en el bit menos significativo de cada byte de la imagen, de tal manera que para codificar 1 byte de texto hacen falta 8 bytes de imagen. El texto codificado se guarda entre medio de 2 marcas, las cuales son usadas por el docodificador para saber si una imagen tiene un texto oculto o no.
