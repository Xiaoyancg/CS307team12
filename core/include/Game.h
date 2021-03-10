#pragma once
#include <SDL.h>
#include <Page.h>
#include <glad/glad.h>
#include <stdio.h>
void sdl_die ( const char *err_msg );
void initShaders ();

void init ();

int render ();

int coreMain ( int argc, char *argv[] );



