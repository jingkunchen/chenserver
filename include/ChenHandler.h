#ifndef __CHEN_HANDLER_H__
#define __CHEN_HANDLER_H__
#include <ctype.h>
#include <stdio.h>



#define ISspace(x) isspace((int)(x))
	
#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"

void accept_request(int clinet,short iEvent, void *arg);
void bad_request(int client);
void cat(int client, FILE *resource);
void cannot_execute(int client);
void error_die(const char *sc);
void execute_cgi(int client, const char *path,
                 const char *method, const char *query_string);
int get_line(int sock, char *buf, int size);
void headers(int client, const char *filename);
void not_found(int client);
void serve_file(int client, const char *filename);
int startup(u_short *port);
void unimplemented(int client);


#endif
