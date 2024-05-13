/*
** EPITECH PROJECT, 2024
** init arena
** File description:
** init the arena
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "arena.h"
#include "op.h"
#include "parameters.h"
#include "libmy.h"
#include "op.h"
#include "functions.h"

static uint32_t reverse_hex_32(uint32_t nb)
{
    uint32_t result = 0;

    result |= ((nb & 0xFF000000) >> 24) | ((nb & 0x00FF0000) >> 8) |
    ((nb & 0x0000FF00) << 8) | ((nb & 0x000000FF) << 24);
    return result;
}

static bool extract_header(head_t *tmp, FILE **file)
{
    int magic = 0;

    fread(&magic, sizeof(int), 1, *file);
    magic = reverse_hex_32(magic);
    if (magic != COREWAR_EXEC_MAGIC) {
        my_puterr("wrong binary error\n");
        return false;
    }
    fread(tmp->name, PROG_NAME_LENGTH + 1, 1, *file);
    fseek(*file, 0, SEEK_SET);
    if (fseek(*file, sizeof(header_t), SEEK_CUR) == -1) {
        fclose(*file);
        return false;
    }
    return true;
}

static bool print_program_in_arena(char *arena, champion_t *champion,
    head_t **heads)
{
    int address = champion->load_address;
    FILE *file = fopen(champion->path, "r");
    head_t *tmp = malloc(sizeof(head_t));

    if (file == NULL) {
        my_puterr(champion->path);
        my_puterr(": no such file or directory.\n");
        return false;
    }
    if (!extract_header(tmp, &file))
        return false;
    for (int i = 0;
        fread(arena + ((address + i) % MEM_SIZE), 1, 1, file) > 0; ++i);
    fclose(file);
    tmp->index = address;
    tmp->number = champion->number;
    tmp->wait_cycle = 0;
    push_front_head(heads, tmp);
    return true;
}

static head_t **init_heads(champion_t **champions, char *arena)
{
    head_t **heads = malloc(sizeof(head_t *));

    if (heads == NULL)
        return NULL;
    *heads = NULL;
    for (int i = 0; champions[i]; ++i) {
        if (!print_program_in_arena(arena, champions[i], heads)) {
            return NULL;
        }
    }
    return heads;
}

arena_t *init_arena(parameters_t *parameters)
{
    arena_t *arena = malloc(sizeof(arena_t));

    if (arena == NULL)
        return NULL;
    arena->arena = malloc(MEM_SIZE);
    if (arena->arena == NULL) {
        free(arena);
        return NULL;
    }
    for (int i = 0; i < MEM_SIZE; ++i) {
        arena->arena[i] = 0;
    }
    arena->heads = init_heads(parameters->champions, arena->arena);
    if (arena->heads == NULL) {
        free(arena->arena);
        free(arena);
        return NULL;
    }
    return arena;
}
