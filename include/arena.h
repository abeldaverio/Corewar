/*
** EPITECH PROJECT, 2024
** arena
** File description:
** arena
*/

#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "op.h"
#include "parameters.h"

    #define IND_SIZE_FILE 2
    #define DIR_SIZE_FILE 4
    #define REG_SIZE_FILE 1

typedef struct head_s {
    int number;
    int index;
    int wait_cycle;
    void (*instruction)(struct head_s *, char *, parameters_t *);
    bool carry;
    int registers[REG_NUMBER];
    struct head_s *next;
}head_t;

typedef struct arena_s {
    char *arena;
    head_t **heads;
}arena_t;
