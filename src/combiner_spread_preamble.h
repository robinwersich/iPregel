/**
 * @file combiner_spread_preamble.h
 * @author Ludovic Capelli
 * @brief This version is optimised for graph traversal algorithms.
 * @details This version relies on a list of vertices to run at every superstep.
 * It can provide better performance when only a small number of vertices are to
 * be executed; instead of checking all vertices if they are active, only the
 * active ones are executed.
 **/

#ifndef COMBINER_SPREAD_PREAMBLE_H_INCLUDED
#define COMBINER_SPREAD_PREAMBLE_H_INCLUDED

#include <pthread.h> 

// Global variables
/// This structure holds a list of vertex identifiers.
struct ip_vertex_list_t
{
	/// The size of the memory buffer. It is used for reallocation purpose.
	size_t max_size;
	/// The number of identifiers currently stored.
	size_t size;
	/// The actual identifiers.
	IP_VERTEX_ID_TYPE* data;
};
/// This variable contains the number of messages that have not been read yet.
size_t ip_messages_left = 0;
/// This variable is used for multithreading reduction into message_left.
size_t ip_messages_left_omp[OMP_NUM_THREADS] = {0};
/// The number of vertices part of the current wave of vertices to execute.
size_t ip_spread_vertices_count = 0;
/// This contains all the vertices to execute next superstep.
struct ip_vertex_list_t ip_all_spread_vertices;
/// This contains the vertices that threads found to be executed next superstep.
struct ip_vertex_list_t ip_all_spread_vertices_omp[OMP_NUM_THREADS];

/**
 * @brief This function adds the given vertex to the list of vertices to execute
 * at next superstep.
 * @param[in] id The identifier of the vertex to executed next superstep.
 * @post The vertex identifier by \p id will be executed at next superstep.
 **/
void ip_add_spread_vertex(IP_VERTEX_ID_TYPE id);

#ifdef IP_USE_SPINLOCK
	/// This macro defines the type of lock used.
	#define IP_LOCKTYPE pthread_spinlock_t
	/// This macro defines how the lock can be initialised.
	#define IP_LOCK_INIT(X) pthread_spin_init(X, PTHREAD_PROCESS_PRIVATE)
	/// This macro defines how the lock can be locked.
	#define IP_LOCK(X) pthread_spin_lock(X)
	/// This macro defines how the lock can be unlocked.
	#define IP_UNLOCK(X) pthread_spin_unlock(X)
	#ifdef IP_WEIGHTED_EDGES
		#ifdef IP_UNUSED_IN_NEIGHBOURS
			/// This macro defines the minimal attributes of a vertex.
			#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
										IP_EDGE_WEIGHTED_TYPE* out_edge_weights; \
										bool has_message; \
										bool has_message_next; \
										IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
										pthread_spinlock_t lock; \
										IP_VERTEX_ID_TYPE id; \
										IP_MESSAGE_TYPE message; \
										IP_MESSAGE_TYPE message_next;
		#else // ifndef IP_UNUSED_IN_NEIGHBOURS
			#ifdef IP_UNUSED_IN_NEIGHBOUR_IDS
				/// This macro defines the minimal attributes of a vertex.
				#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
											IP_EDGE_WEIGHTED_TYPE* out_edge_weights; \
											bool has_message; \
											bool has_message_next; \
											IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
											IP_NEIGHBOURS_COUNT_TYPE in_neighbours_count; \
											pthread_spinlock_t lock; \
											IP_VERTEX_ID_TYPE id; \
											IP_MESSAGE_TYPE message; \
											IP_MESSAGE_TYPE message_next;
			#else // IP_UNUSED_IN_NEIGHBOUR_IDS
				/// This macro defines the minimal attributes of a vertex.
				#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
											IP_EDGE_WEIGHTED_TYPE* out_edge_weights; \
											IP_VERTEX_ID_TYPE* in_neighbours; \
											IP_EDGE_WEIGHTED_TYPE* in_edge_weights; \
											bool has_message; \
											bool has_message_next; \
											IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
											IP_NEIGHBOURS_COUNT_TYPE in_neighbours_count; \
											pthread_spinlock_t lock; \
											IP_VERTEX_ID_TYPE id; \
											IP_MESSAGE_TYPE message; \
											IP_MESSAGE_TYPE message_next;
			#endif // if(n)def IP_UNUSED_IN_NEIGHBOUR_IDS
		#endif // if(n)def IP_UNUSED_IN_NEIGHBOURS
	#else // ifndef IP_WEIGHTED_EDGES
		#ifdef IP_UNUSED_IN_NEIGHBOURS
			/// This macro defines the minimal attributes of a vertex.
			#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
										bool has_message; \
										bool has_message_next; \
										IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
										pthread_spinlock_t lock; \
										IP_VERTEX_ID_TYPE id; \
										IP_MESSAGE_TYPE message; \
										IP_MESSAGE_TYPE message_next;
		#else // ifndef IP_UNUSED_IN_NEIGHBOURS
			#ifdef IP_UNUSED_IN_NEIGHBOUR_IDS
				/// This macro defines the minimal attributes of a vertex.
				#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
											bool has_message; \
											bool has_message_next; \
											IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
											IP_NEIGHBOURS_COUNT_TYPE in_neighbours_count; \
											pthread_spinlock_t lock; \
											IP_VERTEX_ID_TYPE id; \
											IP_MESSAGE_TYPE message; \
											IP_MESSAGE_TYPE message_next;
			#else // IP_UNUSED_IN_NEIGHBOUR_IDS
				/// This macro defines the minimal attributes of a vertex.
				#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
											IP_VERTEX_ID_TYPE* in_neighbours; \
											bool has_message; \
											bool has_message_next; \
											IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
											IP_NEIGHBOURS_COUNT_TYPE in_neighbours_count; \
											pthread_spinlock_t lock; \
											IP_VERTEX_ID_TYPE id; \
											IP_MESSAGE_TYPE message; \
											IP_MESSAGE_TYPE message_next;
			#endif // if(n)def IP_UNUSED_IN_NEIGHBOUR_IDS
		#endif // if(n)def IP_UNUSED_IN_NEIGHBOURS
	#endif // if(n)def IP_WEIGHTED_EDGES
#else // ifndef IP_USE_SPINLOCK
	/// This macro defines the type of lock used.
	#define IP_LOCKTYPE pthread_mutex_t
	/// This macro defines how the lock can be initialised.
	#define IP_LOCK_INIT(X) pthread_mutex_init(X, NULL)
	/// This macro defines how the lock can be locked.
	#define IP_LOCK(X) pthread_mutex_lock(X)
	/// This macro defines how the lock can be unlocked.
	#define IP_UNLOCK(X) pthread_mutex_unlock(X)
	#ifdef IP_WEIGHTED_EDGES
		#ifdef IP_UNUSED_IN_NEIGHBOURS
			/// This macro defines the minimal attributes of a vertex.
			#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
										IP_EDGE_WEIGHTED_TYPE* out_edge_weights; \
										bool has_message; \
										bool has_message_next; \
										IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
										pthread_mutex_t lock; \
										IP_VERTEX_ID_TYPE id; \
										IP_MESSAGE_TYPE message; \
										IP_MESSAGE_TYPE message_next;
		#else // ifndef IP_UNUSED_IN_NEIGHBOURS
			#ifdef IP_UNUSED_IN_NEIGHBOUR_IDS
				/// This macro defines the minimal attributes of a vertex.
				#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
											IP_EDGE_WEIGHTED_TYPE* out_edge_weights; \
											bool has_message; \
											bool has_message_next; \
											IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
											IP_NEIGHBOURS_COUNT_TYPE in_neighbours_count; \
											pthread_mutex_t lock; \
											IP_VERTEX_ID_TYPE id; \
											IP_MESSAGE_TYPE message; \
											IP_MESSAGE_TYPE message_next;
			#else // IP_UNUSED_IN_NEIGHBOUR_IDS
				/// This macro defines the minimal attributes of a vertex.
				#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
											IP_EDGE_WEIGHTED_TYPE* out_edge_weights; \
											IP_VERTEX_ID_TYPE* in_neighbours; \
											IP_EDGE_WEIGHTED_TYPE* in_edge_weights; \
											bool has_message; \
											bool has_message_next; \
											IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
											IP_NEIGHBOURS_COUNT_TYPE in_neighbours_count; \
											pthread_mutex_t lock; \
											IP_VERTEX_ID_TYPE id; \
											IP_MESSAGE_TYPE message; \
											IP_MESSAGE_TYPE message_next;
			#endif // if(n)def IP_UNUSED_IN_NEIGHBOUR_IDS
		#endif // if(n)def IP_UNUSED_IN_NEIGHBOURS
	#else // ifndef IP_WEIGHTED_EDGES
		#ifdef IP_UNUSED_IN_NEIGHBOURS
			/// This macro defines the minimal attributes of a vertex.
			#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
										bool has_message; \
										bool has_message_next; \
										IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
										pthread_mutex_t lock; \
										IP_VERTEX_ID_TYPE id; \
										IP_MESSAGE_TYPE message; \
										IP_MESSAGE_TYPE message_next;
		#else // ifndef IP_UNUSED_IN_NEIGHBOURS
			#ifdef IP_UNUSED_IN_NEIGHBOUR_IDS
				/// This macro defines the minimal attributes of a vertex.
				#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
											bool has_message; \
											bool has_message_next; \
											IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
											IP_NEIGHBOURS_COUNT_TYPE in_neighbours_count; \
											pthread_mutex_t lock; \
											IP_VERTEX_ID_TYPE id; \
											IP_MESSAGE_TYPE message; \
											IP_MESSAGE_TYPE message_next;
			#else // IP_UNUSED_IN_NEIGHBOUR_IDS
				/// This macro defines the minimal attributes of a vertex.
				#define IP_VERTEX_STRUCTURE IP_VERTEX_ID_TYPE* out_neighbours; \
											IP_VERTEX_ID_TYPE* in_neighbours; \
											bool has_message; \
											bool has_message_next; \
											IP_NEIGHBOURS_COUNT_TYPE out_neighbours_count; \
											IP_NEIGHBOURS_COUNT_TYPE in_neighbours_count; \
											pthread_mutex_t lock; \
											IP_VERTEX_ID_TYPE id; \
											IP_MESSAGE_TYPE message; \
											IP_MESSAGE_TYPE message_next;
			#endif // if(n)def IP_UNUSED_IN_NEIGHBOUR_IDS
		#endif // if(n)def IP_UNUSED_IN_NEIGHBOURS
	#endif // if(n)def IP_WEIGHTED_EDGES
#endif // if(n)def IP_USE_SPINLOCK

#endif // COMBINER_SPREAD_PREAMBLE_H_INCLUDED
