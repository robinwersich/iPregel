/**
 * @file my_pregel_postamble.h
 * @author Ludovic Capelli
 **/

#ifndef MY_PREGEL_POSTAMBLE_H_INCLUDED
#define MY_PREGEL_POSTAMBLE_H_INCLUDED

#ifdef MP_USE_COMBINER
	#ifdef MP_USE_SPREAD
		#ifdef MP_USE_SINGLE_BROADCAST
			#include "combiner_spread_single_broadcast_postamble.h"
		#else // ifndef MP_USE_SINGLE_BROADCAST
			#include "combiner_spread_postamble.h"
		#endif // if(n)def MP_USE_SINGLE_BROADCAST
	#else // ifndef MP_USE_SPREAD
		#ifdef MP_USE_SINGLE_BROADCAST
			#include "combiner_single_broadcast_postamble.h"
		#else // ifndef MP_USE_SINGLE_BROADCAST
			#include "combiner_postamble.h"
		#endif // if(n)def MP_USE_SINGLE_BROADCAST
	#endif // if(n)def MP_USE_SPREAD
#else // ifndef MP_USE_COMBINER
	#include "no_combiner_postamble.h"	
#endif // if(n)def MP_USE_COMBINER

size_t mp_get_superstep()
{
	return mp_superstep;
}

void mp_increment_superstep()
{
	mp_superstep++;
}

bool mp_is_first_superstep()
{
	return mp_get_superstep() == 0;
}

void mp_reset_superstep()
{
	mp_superstep = 0;
}

size_t mp_get_meta_superstep()
{
	return mp_meta_superstep;
}

void mp_increment_meta_superstep()
{
	mp_meta_superstep++;
}

bool mp_is_first_meta_superstep()
{
	return mp_get_meta_superstep() == 0;
}

size_t mp_get_meta_superstep_count()
{
	return mp_meta_superstep_count;
}

void mp_set_meta_superstep_count(size_t meta_superstep_count)
{
	mp_meta_superstep_count = meta_superstep_count;
}

void mp_set_id_offset(size_t offset)
{
	mp_id_offset = offset;
}

size_t mp_get_id_offset()
{
	return mp_id_offset;
}

void mp_set_vertices_count(size_t vertices_count)
{
	mp_vertices_count = vertices_count;
}

size_t mp_get_vertices_count()
{
	return mp_vertices_count;
}

struct mp_vertex_t* mp_get_vertex_by_location(size_t location)
{
	return &mp_all_vertices[location];
}

struct mp_vertex_t* mp_get_vertex_by_id(MP_VERTEX_ID_TYPE id)
{
	#if MP_MINIMUM_ID == 0
		return mp_get_vertex_by_location(id);
	#else
		size_t location = id;
		location -= mp_get_id_offset();
		return mp_get_vertex_by_location(location);
	#endif
}

void mp_dump(FILE* f)
{
	double timer_dump_start = omp_get_wtime();
	double timer_dump_stop = 0;
	unsigned char progress = 0;
	size_t i = 0;
	size_t chunk = mp_get_vertices_count() / 100;

	if(chunk == 0)
	{
		chunk = 1;
	}
	printf("%3u %% vertices stored.\r", progress);
	fflush(stdout);
	while(i < mp_get_vertices_count())
	{
		mp_serialise_vertex(f, mp_get_vertex_by_location(i));
		if(i % chunk == 0)
		{
			progress++;
			printf("%3u %%\r", progress);
			fflush(stdout);
		}
		i++;
	}
	printf("100 %%\n");

	timer_dump_stop = omp_get_wtime();
	printf("Dumping finished in %fs.\n", timer_dump_stop - timer_dump_start);
}

void* mp_safe_malloc(size_t size_to_malloc)
{
	void* ptr = malloc(size_to_malloc);
	if(ptr == NULL)
	{
		exit(-1);
	}
	return ptr;
}

void* mp_safe_realloc(void* ptr, size_t size_to_realloc)
{
	ptr = realloc(ptr, size_to_realloc);
	if(ptr == NULL)
	{
		exit(-1);
	}
	return ptr;
}

void mp_safe_free(void* ptr)
{
	if(ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}

void mp_safe_fread(void * ptr, size_t size, size_t count, FILE * stream)
{
	if(fread(ptr, size, count, stream) != count)
	{
		printf("Failed to read.\n");
		exit(-1);
	}
}

void mp_safe_fwrite(void * ptr, size_t size, size_t count, FILE * stream)
{
	if(fwrite(ptr, size, count, stream) != count)
	{
		printf("Failed to write.'\n");
		exit(-1);
	}
}

#endif // MY_PREGEL_POSTAMBLE_H_INCLUDED