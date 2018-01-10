#include <stdlib.h>
#include <limits.h>

typedef unsigned int MP_VERTEX_ID_TYPE;
typedef MP_VERTEX_ID_TYPE MP_MESSAGE_TYPE;
typedef unsigned int MP_NEIGHBOURS_COUNT_TYPE;
#include "my_pregel_preamble.h"
struct mp_vertex_t
{
	MP_VERTEX_STRUCTURE
	MP_VERTEX_ID_TYPE min_f;
	MP_VERTEX_ID_TYPE min_b;
};
#include "my_pregel_postamble.h"

void mp_compute(struct mp_vertex_t* v)
{
	if(mp_is_first_meta_superstep())
	{
		if(mp_is_first_superstep())
		{
			v->min_f = v->id;
			if(v->in_neighbours_count > 0)
			{
				for(MP_NEIGHBOURS_COUNT_TYPE i = 0; i < v->in_neighbours_count; i++)
				{
					if(v->in_neighbours[i] < v->min_f)
					{   
						v->min_f = v->in_neighbours[i];
					}
				}
			}
			if(v->out_neighbours_count > 0)
			{
				for(MP_NEIGHBOURS_COUNT_TYPE i = 0; i < v->out_neighbours_count; i++)
				{
					if(v->out_neighbours[i] < v->min_f)
					{   
						v->min_f = v->out_neighbours[i];
					}
				}
			}
			mp_broadcast(v, v->min_f);
			mp_vote_to_halt(v);
		}
		else
		{
			MP_MESSAGE_TYPE initial_min_f = v->min_f;
			MP_MESSAGE_TYPE message_value;
			while(mp_get_next_message(v, &message_value))
			{
				if(v->min_f > message_value)
				{
					v->min_f = message_value;
				}
			}
	
			if(initial_min_f != v->min_f)
			{
				mp_broadcast(v, v->min_f);
			}
	
			mp_vote_to_halt(v);
		}
	}
	else
	{
		if(mp_is_first_superstep())
		{
			if(v->id == v->min_f)
			{
				v->min_b = v->id;
				mp_broadcast(v, v->min_b);
			}
			else
			{
				v->min_b = UINT_MAX;
			}
			mp_vote_to_halt(v);
		}
		else
		{
			MP_MESSAGE_TYPE initial_min_b = v->min_b;
			MP_MESSAGE_TYPE message_value;
			while(mp_get_next_message(v, &message_value))
			{
				if(v->min_b > message_value)
				{
					v->min_b = message_value;
				}
			}
	
			if(initial_min_b != v->min_b)
			{
				mp_broadcast(v, v->min_b);
			}
	
			mp_vote_to_halt(v);
		}
	}
}

void mp_combine(MP_MESSAGE_TYPE* a, MP_MESSAGE_TYPE* b)
{
	if(*a > *b)
	{
		*a = *b;
	}	
}

void mp_deserialise_vertex(FILE* f)
{
	MP_VERTEX_ID_TYPE vertex_id;
	void* buffer_out_neighbours = NULL;
	unsigned int buffer_out_neighbours_count = 0;
	void* buffer_in_neighbours = NULL;
	unsigned int buffer_in_neighbours_count = 0;

	mp_safe_fread(&vertex_id, sizeof(MP_VERTEX_ID_TYPE), 1, f); 
	mp_safe_fread(&buffer_out_neighbours_count, sizeof(unsigned int), 1, f); 
	if(buffer_out_neighbours_count > 0)
	{
		buffer_out_neighbours = (MP_VERTEX_ID_TYPE*)mp_safe_malloc(sizeof(MP_VERTEX_ID_TYPE) * buffer_out_neighbours_count);
		mp_safe_fread(buffer_out_neighbours, sizeof(MP_VERTEX_ID_TYPE), buffer_out_neighbours_count, f); 
	}
	mp_safe_fread(&buffer_in_neighbours_count, sizeof(unsigned int), 1, f);
	if(buffer_in_neighbours_count > 0)
	{
		buffer_in_neighbours = (MP_VERTEX_ID_TYPE*)mp_safe_malloc(sizeof(MP_VERTEX_ID_TYPE) * buffer_in_neighbours_count);
		mp_safe_fread(buffer_in_neighbours, sizeof(MP_VERTEX_ID_TYPE), buffer_in_neighbours_count, f); 
	}

	mp_add_vertex(vertex_id, buffer_out_neighbours, buffer_out_neighbours_count, buffer_in_neighbours, buffer_in_neighbours_count);
}

void mp_serialise_vertex(FILE* f, struct mp_vertex_t* v)
{
	mp_safe_fwrite(&v->id, sizeof(MP_VERTEX_ID_TYPE), 1, f);
	mp_safe_fwrite(&v->min_f, sizeof(MP_VERTEX_ID_TYPE), 1, f);
	mp_safe_fwrite(&v->min_b, sizeof(MP_VERTEX_ID_TYPE), 1, f);
}

int main(int argc, char* argv[])
{
	if(argc != 3) 
	{
		printf("Incorrect number of parameters.\n");
		return -1;
	}

	FILE* f_in = fopen(argv[1], "rb");
	if(!f_in)
	{
		perror("File opening failed.");
		return -1;
	}

	FILE* f_out = fopen(argv[2], "wb");
	if(!f_out)
	{
		perror("File opening failed.");
		return -1;
	}

	size_t number_of_vertices = 0;
	if(fread(&number_of_vertices, sizeof(unsigned int), 1, f_in) != 1)
	{
		perror("Could not read the number of vertices.");
		exit(-1);
	}
	mp_set_meta_superstep_count(2);
	mp_init(f_in, number_of_vertices);
	mp_run();
	mp_dump(f_out);

	return EXIT_SUCCESS;
}