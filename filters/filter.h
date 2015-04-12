#ifndef __FILTER_H
#define __FILTER_H

#include "core/audio.h"
#include "core/parameter.h"
#include "filters/audio.h"
#include "ui/graph.h"

struct filter;

typedef void * filter_state_pt;
typedef void * vamp_plugin_p;
typedef void (*filter_init_fn_pt)(struct filter * filter);
typedef void (*filter_update_fn_pt)(struct filter * filter, int t_msec, double value);
typedef void (*filter_del_fn_pt)(struct filter * filter);

typedef struct filter {
    // Input parameters
    int n_params;
    parameter_t* parameters;
    param_state_t* param_states;

    // Output value
    param_output_t output;

    // VAMP Configuration
    char * vamp_so;
    char * vamp_id;
    unsigned int vamp_output;
    vamp_plugin_p vamp_plugin;

    // UI Elements
    char * name;

    // State
    filter_state_pt state;
    graph_state_t graph_state;

    // Function Calls
    filter_init_fn_pt init;
    filter_update_fn_pt update;
    filter_del_fn_pt del;
} filter_t;

extern int n_filtered_chunks;
extern int n_filters;
extern filter_t filters[];
extern filter_t beat_filter;

void filters_update(chunk_pt chunk);
void filters_load();
void filters_unload();
#endif
