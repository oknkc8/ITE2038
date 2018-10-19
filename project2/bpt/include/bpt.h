#ifndef __BPT_H__
#define __BPT_H__

// Uncomment the line below if you are compiling on Windows.
// #define WINDOWS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file_manager.h"
#ifdef WINDOWS
#define bool char
#define false 0
#define true 1
#endif

// Default order is 4.
#define DEFAULT_ORDER 4

// Minimum order is necessarily 3.  We set the maximum
// order arbitrarily.  You may change the maximum order.
#define MIN_ORDER 3
#define MAX_ORDER 20

// Constants for printing part or all of the GPL license.
#define LICENSE_FILE "LICENSE.txt"
#define LICENSE_WARRANTEE 0
#define LICENSE_WARRANTEE_START 592
#define LICENSE_WARRANTEE_END 624
#define LICENSE_CONDITIONS 1
#define LICENSE_CONDITIONS_START 70
#define LICENSE_CONDITIONS_END 625

// TYPES.

/* Type representing the record
 * to which a given key refers.
 * In a real B+ tree system, the
 * record would hold data (in a database)
 * or a file (in an operating system)
 * or some other information.
 * Users can rewrite this part of the code
 * to change the type and content
 * of the value field.
 */
typedef struct record {
    int value;
} record;

/* Type representing a node in the B+ tree.
 * This type is general enough to serve for both
 * the leaf and the internal node.
 * The heart of the node is the array
 * of keys and the array of corresponding
 * pointers.  The relation between keys
 * and pointers differs between leaves and
 * internal nodes.  In a leaf, the index
 * of each key equals the index of its corresponding
 * pointer, with a maximum of order - 1 key-pointer
 * pairs.  The last pointer points to the
 * leaf to the right (or NULL in the case
 * of the rightmost leaf).
 * In an internal node, the first pointer
 * refers to lower nodes with keys less than
 * the smallest key in the keys array.  Then,
 * with indices i starting at 0, the pointer
 * at i + 1 points to the subtree with keys
 * greater than or equal to the key in this
 * node at index i.
 * The num_keys field is used to keep
 * track of the number of valid keys.
 * In an internal node, the number of valid
 * pointers is always num_keys + 1.
 * In a leaf, the number of valid pointers
 * to data is always num_keys.  The
 * last leaf pointer points to the next leaf.
 */
/*
typedef struct node{
    void ** pointers;
    int * keys;
    struct node * parent;
    bool is_leaf;
    int num_keys;
    struct node * next; // Used for queue.
} node;
*/
typedef struct queue_node{
    pagenum_t num;
    struct queue_node * next;
} queue_node;

// GLOBALS.

/* The order determines the maximum and minimum
 * number of entries (keys and pointers) in any
 * node.  Every node has at most order - 1 keys and
 * at least (roughly speaking) half that number.
 * Every leaf has as many pointers to data as keys,
 * and every internal node has one more pointer
 * to a subtree than the number of keys.
 * This global variable is initialized to the
 * default value.
 */
extern int order;
extern bool verbose;

/* The queue is used to print the tree in
 * level order, starting from the root
 * printing each entire rank on a separate
 * line, finishing with the leaves.
 */
//extern node * queue;
extern queue_node * queue;

/* The user can toggle on and off the "verbose"
 * property, which causes the pointer addresses
 * to be printed out in hexadecimal notation
 * next to their corresponding keys.
 */
//extern bool verbose_output;


// FUNCTION PROTOTYPES.

// Output and utility.

void license_notice( void );
void print_license( int licence_part );
void usage_1( void );
void usage_2( void );
void usage_3( void );
//void enqueue( node * new_node );
void enqueue( pagenum_t new_pagenum );
//node * dequeue( void );
pagenum_t dequeue( void );
//int height( node * root );
//int path_to_root( node * root, node * child );
int path_to_root( pagenum_t child_pagenum );
//void print_leaves( node * root );
void print_leaves( void );
//void print_tree( node * root );
void print_tree( void );
//void find_and_print(node * root, int key, bool verbose); 
void find_and_print( int64_t key, bool is_p );
/*void find_and_print_range(node * root, int range1, int range2, bool verbose); 
int find_range( node * root, int key_start, int key_end, bool verbose,
        int returned_keys[], void * returned_pointers[]); */
//node * find_leaf( node * root, int key, bool verbose );
page_t * find_leaf( int64_t key, pagenum_t * index );
//record * find( node * root, int key, bool verbose );
char * find( int64_t key );
int cut( int length );

// Insertion.

//record * make_record(int value);
record_t * make_record( int64_t key, char * value );
//node * make_node( void );
page_t * make_node( void );
//node * make_leaf( void );
page_t * make_leaf( void );

//int get_left_index(node * parent, node * left);
int get_left_index( page_t * parent, pagenum_t left_pagenum);
//node * insert_into_leaf( node * leaf, int key, record * pointer );
void insert_info_leaf( page_t * leaf, record_t * record );
//node * insert_into_leaf_after_splitting(node * root, node * leaf, int key,
//                                        record * pointer);
void insert_into_leaf_after_splitting( page_t * leaf, record_t * record, pagenum_t index );
//node * insert_into_node(node * root, node * parent, 
//        int left_index, int key, node * right);
void insert_into_node( page_t * n, pagenum_t n_pagenum, 
    int left_index, int64_t key, pagenum_t right_pagenum );
//node * insert_into_node_after_splitting(node * root, node * parent,
//                                        int left_index, int key, node * right);
void insert_into_node_after_splitting( page_t * old_page, pagenum_t * old_page_pagenum, int left_index, 
    int64_t key, page_t * right, pagenum_t * right_pagenum);
//node * insert_into_parent(node * root, node * left, int key, node * right);
void insert_into_parent(page_t * left, pagenum_t * left_pagenum, int64_t key, 
    page_t * right, pagenum_t * right_pagenum);
//node * insert_into_new_root(node * left, int key, node * right);
void insert_into_new_root(page_t * left, pagenum_t * left_pagenum, int64_t key, 
    page_t * right, pagenum_t * right_pagenum);
//node * start_new_tree(int key, record * pointer);
void start_new_tree( record_t * record );
//node * insert( node * root, int key, int value );
int insert( int64_t key, char * value );

// Deletion.

//int get_neighbor_index( node * n );
pagenum_t get_neighbor_index( page_t * n, pagenum_t n_pagenum, bool * is_most_left ,
    int * parent_entries_index);
page_t * remove_entry_from_node( page_t * n, pagenum_t n_pagenum, int64_t key );
//node * adjust_root(node * root);
void adjust_root(void);
//node * coalesce_nodes(node * root, node * n, node * neighbor,
//                      int neighbor_index, int k_prime);
//node * redistribute_nodes(node * root, node * n, node * neighbor,
//                          int neighbor_index, int k_prime_index, int k_prime);
void redistribute_nodes( page_t * n, pagenum_t n_pagenum, page_t * neighbor,
    pagenum_t neighbor_pagenum, page_t * parent, pagenum_t parent_pagenum,
    bool is_most_left, int entries_index);
//node * delete_entry( node * root, node * n, int key, void * pointer );
void delete_entry( page_t * n, pagenum_t n_pagenum, int64_t key, 
    char * key_record_value);
//node * delete( node * root, int key );
int delete(int64_t key);

//void destroy_tree_nodes(node * root);
//node * destroy_tree(node * root);

#endif /* __BPT_H__*/
