#include <stdlib.h>

typedef unsigned int VERTEX_ID;
typedef unsigned int MESSAGE_TYPE;
#include "my_pregel_preamble.h"

struct vertex_t
{
	VERTEX_STRUCTURE
	MESSAGE_TYPE value;
};

void compute(struct vertex_t* v)
{
	if(superstep == 0)
	{
		if(v->neighbours_count > 0)
		{
			v->value = v->neighbours[0];
			for(unsigned int i = 1; i < v->neighbours_count; i++)
			{
				if(v->neighbours[i] < v->value)
				{   
					v->value = v->neighbours[i];
				}
			}
		}
		else
		{
			v->value = v->id;
		}

		broadcast(v, v->value);
		vote_to_halt(v);
	}
	else
	{
		MESSAGE_TYPE valueTemp = v->value;
		MESSAGE_TYPE message_value;
		while(get_next_message(v, &message_value))
		{
			if(v->value > message_value)
			{
				v->value = message_value;
			}
		}

		if(valueTemp != v->value)
		{
			broadcast(v, v->value);
		}

		vote_to_halt(v);
	}
}

void combine(MESSAGE_TYPE* a, MESSAGE_TYPE* b)
{
	if(*a > *b)
	{
		*a = *b;
	}
}

void deserialise_vertex(FILE* f, struct vertex_t* v)
{
	size_t fread_size = fread(&v->id, sizeof(VERTEX_ID), 1, f);
	if(fread_size != 1)
	{
		printf("Error in fread from deserialise vertex.\n");
		exit(-1);
	}
	fread_size = fread(&v->neighbours_count, sizeof(unsigned int), 1, f);
	if(fread_size != 1)
	{
		printf("Error in fread from deserialise vertex.\n");
		exit(-1);
	}
	if(v->neighbours_count > 0)
	{
		v->neighbours = (unsigned int*)safe_malloc(sizeof(VERTEX_ID) * v->neighbours_count);
		fread_size = fread(&v->neighbours[0], sizeof(VERTEX_ID), v->neighbours_count, f);
		if(fread_size != v->neighbours_count)
		{
			printf("Error in fread from deserialise vertex.\n");
			exit(-1);
		}
	}
}

void serialise_vertex(FILE* f, struct vertex_t* v)
{
	(void)(f);
	(void)(v);
}

int verify()
{
	printf("Verification...");
	for(unsigned int i = 0; i < vertices_count; i++)
	{
		if(all_vertices[i].value != 0)
		{
			printf("failed.\n");
			return -1;
		}
	}
	printf("succeeded.\n");

	return 0;
}

int main(int argc, char* argv[])
{
	if(argc != 2) 
	{
		printf("Incorrect number of parameters.\n");
		return -1;
	}

	FILE* f = fopen(argv[1], "rb");
	if(!f)
	{
		perror("File opening failed.");
		return -1;
	}

	unsigned int number_of_vertices = 0;
	if(fread(&number_of_vertices, sizeof(unsigned int), 1, f) != 1)
	{
		perror("Could not read the number of vertices.");
		exit(-1);
	}
	init(f, number_of_vertices);
	run();
	verify();

	return EXIT_SUCCESS;
}

#include "my_pregel_postamble.h"
