/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	int offset = 0;
	*bytes = paquete->buffer->size + sizeof(int) * 2;
	void* a_enviar = malloc(*bytes);

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_buffer* buffer=malloc(sizeof(t_buffer));
			buffer->size=strlen(mensaje)+1;
			buffer->stream=malloc(buffer->size);
			memcpy(buffer->stream,mensaje, buffer->size);  // en el tamaño decia sizeof(buffer->size);

			t_paquete* paquete=malloc(sizeof(paquete));
			paquete->codigo_operacion= MENSAJE;
			paquete->buffer=malloc(buffer->size + sizeof(buffer->size));
			// cargo el paquete->buffer
			memcpy(paquete->buffer, buffer, buffer->size + sizeof(buffer->size));
			//tengo listo el paquete

			//lo serializo
			int cant_bytes;
			void* a_enviar= serializar_paquete(paquete,&cant_bytes);

			int status = send(socket_cliente,a_enviar,cant_bytes,0); //ponerlo en una variable el tamaño

			free(a_enviar);
			free(paquete->buffer->stream);
			free(paquete->buffer);
			free(paquete);

			return;
}

//TODO
char* recibir_mensaje(int socket_cliente) //esta funcion retorna el mensaje recibido.
{	//recorda que siempre tengo que reservar memoria cuando tenga puntero
	t_paquete* paquete_recibido = malloc(sizeof(paquete_recibido));
	int status = recv(socket_cliente,&(paquete_recibido->codigo_operacion),sizeof(int),0);
	/*recibimos el buffer*/
	paquete_recibido->buffer=malloc(sizeof(paquete_recibido->buffer));
	status = recv(socket_cliente,&(paquete_recibido->buffer->size),sizeof(int),0);
	paquete_recibido->buffer->stream = malloc(paquete_recibido->buffer->size);
	status = recv(socket_cliente,(paquete_recibido->buffer->stream),(paquete_recibido->buffer->size),0);

	switch(paquete_recibido->codigo_operacion){
		  case MENSAJE:
		    return paquete_recibido->buffer->stream; //retorna el mje recibido y sale de la funcion recibir_mensaje

	}
	return "me mandaron cualquier cosa"; //en caso que me hayan enviado algo que no es un mensaje, la funcion retorna esto.
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

