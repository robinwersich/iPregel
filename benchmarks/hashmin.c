#include <stdlib.h>

typedef unsigned int MP_VERTEX_ID_TYPE;
typedef MP_VERTEX_ID_TYPE MP_MESSAGE_TYPE;
typedef unsigned int MP_NEIGHBOURS_COUNT_TYPE;
#include "my_pregel_preamble.h"
struct mp_vertex_t
{
	MP_VERTEX_STRUCTURE
	MP_MESSAGE_TYPE value;
};
#include "my_pregel_postamble.h"

void mp_compute(struct mp_vertex_t* v)
{
	if(mp_is_first_superstep())
	{
		v->value = v->id;
		if(v->out_neighbours_count > 0)
		{
			for(MP_NEIGHBOURS_COUNT_TYPE i = 0; i < v->out_neighbours_count; i++)
			{
				if(v->out_neighbours[i] < v->value)
				{   
					v->value = v->out_neighbours[i];
				}
			}
		}
		mp_broadcast(v, v->value);
	}
	else
	{
		MP_MESSAGE_TYPE valueTemp = v->value;
		MP_MESSAGE_TYPE message_value;
		while(mp_get_next_message(v, &message_value))
		{
			if(v->value > message_value)
			{
				v->value = message_value;
			}
		}
		if(valueTemp != v->value)
		{
			mp_broadcast(v, v->value);
		}
	}
	mp_vote_to_halt(v);
}

void mp_combine(MP_MESSAGE_TYPE* a, MP_MESSAGE_TYPE b)
{
	if(*a > b)
	{
		*a = b;
	}
}

void mp_deserialise(FILE* f)
{
	MP_VERTEX_ID_TYPE src;
	MP_VERTEX_ID_TYPE dest;
	size_t lineCount = 0;
	while(fscanf(f, "%u %u", &src, &dest) == 2)
	{
		mp_add_edge(src, dest);
		lineCount++;
	}
	fclose(f);
}

void mp_serialise_vertex(FILE* f, struct mp_vertex_t* v)
{
	mp_safe_fwrite(&v->id, sizeof(MP_VERTEX_ID_TYPE), 1, f);
	mp_safe_fwrite(&v->value, sizeof(MP_MESSAGE_TYPE), 1, f);
}

int main(int argc, char* argv[])
{
	if(argc != 3) 
	{
		printf("Incorrect number of parameters, expecting: %s <inputFile> <outputFile>.\n", argv[0]);
		return -1;
	}

	////////////////////
	// INITILISATION //
	//////////////////
	FILE* f_in = fopen(argv[1], "r");
	if(!f_in)
	{
		perror("File opening failed.");
		return -1;
	}
	MP_VERTEX_ID_TYPE number_of_vertices;
	MP_VERTEX_ID_TYPE number_of_edges;
	if(fscanf(f_in, "%u %u", &number_of_vertices, &number_of_edges) != 2)
	{
		perror("Could not read the number of vertices and number of edges.");
		return -1;
	}
	printf("|V| = %u, |E| = %u.\n", number_of_vertices, number_of_edges);
	mp_init(f_in, number_of_vertices, number_of_edges);

	//////////
	// RUN //
	////////
	//mp_set_id_offset(1);
	mp_run();

	//////////////
	// DUMPING //
	////////////
	FILE* f_out = fopen(argv[4], "w");
	if(!f_out)
	{
		perror("File opening failed.");
		return -1;
	}
	mp_dump(f_out);

	return EXIT_SUCCESS;
}
