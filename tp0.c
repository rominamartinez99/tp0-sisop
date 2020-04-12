/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"


int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;


	typedef enum
	{
		MENSAJE=1
	}op_code;

	typedef struct t_paquete
	{ 	op_code codigo_operacion;
		t_buffer* buffer;
	}t_paquete;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	//Loggear "soy un log"
	log_info(logger,"soy un log");

	config = leer_config();
	ip= config_get_string_value(config,"IP");
	puerto= config_get_string_value(config,"PUERTO");

	//loggear la config
	log_info(logger,ip);
	log_info(logger,puerto);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//crear conexion
	conexion= crear_conexion(ip,puerto);

	//enviar mensaje
	enviar_mensaje("holaa",conexion);

	//recibir mensaje recorda que retorna el mje recibido


	//loguear mensaje recibido
	log_info(logger,recibir_mensaje(conexion));

	terminar_programa(conexion, logger, config);
}

//TODO
t_log* iniciar_logger(void)
{
	return log_create("tp0.log","tp0_game_and_watch",true,LOG_LEVEL_INFO);
}


//TODO
t_config* leer_config(void)
{
	return config_create("tp0.config");

}


//TODO
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	log_destroy(logger);
	config_destroy(config);
	return;

}






