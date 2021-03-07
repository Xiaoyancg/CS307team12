#pragma once
#include <SDL.h>
#include <Page.h>
#include <glad/glad.h>
#include <stdio.h>
#include "test_core.h"

void sdl_die ( const char *err_msg );

void init ();

int render ();

int core_main ( int argc, char *argv[] );



