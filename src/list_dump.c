#define LOGGING
#include <list_dump.h>

void FPrintList( list_t* list, FILE* file )
{
    file = fopen(LOG_FILENAME, "a");
    assert(file);

    fprintf(file, "__list__\n");
    fprintf(file, "========================================================\n");
    fprintf(file, "size: %zd\ncapacity: %zd\nhead: %zd\ntail: %zd\nfirst_empty: %zd\n",
            list->size,
            list->capacity,
            list->list_arr[0].next,
            list->list_arr[0].prev,
            list->first_empty_iterator);
    fprintf(file, "--------------------------------------------------------\n");

    fprintf(file, "index\t\t| data\t\t| next\t\t| prev\t\t\n");
    fprintf(file, "--------------------------------------------------------\n");


    for (ssize_t id = 0; id < list->capacity; id++)
    {
        fprintf(file, "[%zd]\t\t| %d\t\t| %zd\t\t| %zd\t\t\n",
                id,
                list->list_arr[id].data,
                list->list_arr[id].next,
                list->list_arr[id].prev);
    }

    fprintf(file, "========================================================\n\n\n");
    fclose(file);
}

void GraphDump( list_t* list )
{
    char graph_file_name[70] = {};
    char time_buffer[20] = {};
    strcpy(time_buffer, FillTimeForNames());
    snprintf(graph_file_name, 70, "logs/txt/graph_dot_code%s.txt", time_buffer);

    FILE* graph = fopen(graph_file_name, "w");
    assert(graph);

    fprintf(graph, "digraph\n");
    fprintf(graph, "{\n");
    fprintf(graph, "rankdir=UD\n");

    fprintf(graph, "{ rank = sink; head      [shape = Mrecord, style = filled, color = "color_pink"; fillcolor = "color_lightpink"; label = \"Head\"];}\n");
    fprintf(graph, "{ rank = sink;   tail    [shape = Mrecord; style = filled, color = "color_pink"; fillcolor = "color_lightpink"; label = \"Tail\"];}\n");
    fprintf(graph, "{ rank = source; frst_mt [shape = Mrecord; style = filled, color = "color_pink"; fillcolor = "color_lightpink"; label = \"First Empty\"];}\n");
    fprintf(graph, "{ rank = source; error   [shape = hexagon; style = filled, color = "color_red"; fillcolor = "color_lightred"; label = \"Error\"];}\n");

    fprintf(graph, "head -> node%zd    [color = "color_pink"];\n", list->list_arr[0].next);
    fprintf(graph, "tail -> node%zd    [color = "color_pink"];\n", list->list_arr[0].prev);
    fprintf(graph, "frst_mt -> node%zd [color = "color_pink"];\n", list->first_empty_iterator);

    for (ssize_t id = 0; id < list->capacity - 1; id++)
    {
        fprintf(graph,
        "node%zd -> node%zd [style = invis, weight = 100];\n", id, id + 1);
    }

    char fillcolor_str[15] = {};
    char color_str[15] = {};

    //инициализирует все элементы
    for (ssize_t id = 0; id < list->capacity; id++)
    {
        if (id == 0)
        {
            strcpy(fillcolor_str, color_lightgray);
            strcpy(color_str, color_gray);
        }
        else if (list->list_arr[id].prev == -1)
        {
            strcpy(fillcolor_str, color_lightblue);
            strcpy(color_str, color_blue);
        }
        else if (list->list_arr[id].next < 0 || list->list_arr[id].prev < 0)
        {
            strcpy(fillcolor_str, color_lightred);
            strcpy(color_str, color_red);
        }
        else
        {
            strcpy(fillcolor_str, color_lightgreen);
            strcpy(color_str, color_green);
        }

        fprintf(graph,
        "node%zd [shape = Mrecord; style = filled; fillcolor = %s; color = %s;\n"
        "         label = \" { data = %d | id = %zd | { next = %zd | prev = %zd } }\"; ]\n",
                  id, fillcolor_str, color_str,
                  list->list_arr[id].data, id, list->list_arr[id].next, list->list_arr[id].prev);
    }

    fprintf(graph, "{rank = same; ");
    for (ssize_t i = 0; i < list->capacity; i++)
    {
        fprintf(graph, "node%zd; ", i);
    }
    fprintf(graph, "}\n");

    fprintf(graph, "splines = ortho;\n");

    //стрелочки next
    char edge_style_str[15] = {};
    for (ssize_t id = 0; id < list->capacity; id++)
    {
        if (list->list_arr[id].prev == -1)
        {
            strcpy(color_str, color_lightblue);
            strcpy(edge_style_str, "solid");
        }
        else if (list->list_arr[id].next < 0)
        {
            strcpy(color_str, color_red);
            strcpy(edge_style_str, "bold");
            fprintf(graph, "node%zd -> error [color = %s, style = %s, weight = 0]\n",
                    id, color_str, edge_style_str);
            continue;
        }
        else
        {
            strcpy(color_str, color_green);
            strcpy(edge_style_str, "solid");
        }

        fprintf(graph, "node%zd -> node%zd [color = %s, style = %s, weight = 0]\n",
                id, list->list_arr[id].next, color_str, edge_style_str);
    }

    //стрелочки prev
    for (ssize_t id = 0; id < list->capacity; id++)
    {
        if (0 <= list->list_arr[id].prev && list->list_arr[id].prev < list->capacity)
        {
            strcpy(color_str, color_darkblue);
            strcpy(edge_style_str, "solid");
            fprintf(graph, "node%zd -> node%zd [color = %s, style = %s, weight = 0]\n",
                id, list->list_arr[id].prev, color_str, edge_style_str);
        }
        else if (list->list_arr[id].prev != -1)
        {
            strcpy(color_str, color_red);
            strcpy(edge_style_str, "bold");
            fprintf(graph, "node%zd -> node%zd [color = %s, style = %s, weight = 0]\n",
                id, id, color_str, edge_style_str);
        }
    }

    fprintf(graph, "}");
    fclose(graph);

    char command[150] = {};
    snprintf(command, 150, "dot -Tsvg %s -o logs/img/graph_img%s.svg", graph_file_name, time_buffer);
    system(command);

    #ifdef LOGGING
    snprintf(command, 150, "<img src = ../img/graph_img%s.svg>", time_buffer);
    Log(command);
    #endif
}
