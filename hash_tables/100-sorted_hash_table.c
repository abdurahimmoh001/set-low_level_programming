#include "hash_tables.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * shash_table_create - creates a sorted hash table
 * @size: size of the array
 *
 * Return: pointer to the new hash table, or NULL
 */
shash_table_t *shash_table_create(unsigned long int size)
{
	shash_table_t *ht;

	ht = malloc(sizeof(shash_table_t));
	if (ht == NULL)
		return (NULL);

	ht->size = size;
	ht->shead = NULL;
	ht->stail = NULL;
	ht->array = malloc(sizeof(shash_node_t *) * size);
	if (ht->array == NULL)
	{
		free(ht);
		return (NULL);
	}

	return (ht);
}

/**
 * shash_table_set - adds or updates an element
 * @ht: sorted hash table
 * @key: key
 * @value: value
 *
 * Return: 1 if successful, 0 otherwise
 */
int shash_table_set(shash_table_t *ht, const char *key, const char *value)
{
	unsigned long int index;
	shash_node_t *node;
	shash_node_t *current;

	if (ht == NULL || key == NULL || *key == '\0' || value == NULL)
		return (0);

	index = key_index((const unsigned char *)key, ht->size);
	current = ht->array[index];

	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			return (shash_update(current, value));
		}
		current = current->next;
	}

	node = shash_create_node(key, value);
	if (node == NULL)
		return (0);

	node->next = ht->array[index];
	ht->array[index] = node;
	shash_sorted_insert(ht, node);

	return (1);
}

/**
 * shash_create_node - creates a sorted hash node
 * @key: key
 * @value: value
 *
 * Return: new node, or NULL
 */
shash_node_t *shash_create_node(const char *key, const char *value)
{
	shash_node_t *node;

	node = malloc(sizeof(shash_node_t));
	if (node == NULL)
		return (NULL);

	node->key = strdup(key);
	if (node->key == NULL)
	{
		free(node);
		return (NULL);
	}

	node->value = strdup(value);
	if (node->value == NULL)
	{
		free(node->key);
		free(node);
		return (NULL);
	}

	node->next = NULL;
	node->sprev = NULL;
	node->snext = NULL;

	return (node);
}

/**
 * shash_update - updates a node value
 * @node: node to update
 * @value: new value
 *
 * Return: 1 if successful, 0 otherwise
 */
int shash_update(shash_node_t *node, const char *value)
{
	char *new_value;

	new_value = strdup(value);
	if (new_value == NULL)
		return (0);

	free(node->value);
	node->value = new_value;

	return (1);
}

/**
 * shash_sorted_insert - inserts a node into sorted list
 * @ht: sorted hash table
 * @node: node to insert
 *
 * Return: Nothing
 */
void shash_sorted_insert(shash_table_t *ht, shash_node_t *node)
{
	shash_node_t *current;

	if (ht->shead == NULL)
	{
		ht->shead = node;
		ht->stail = node;
		return;
	}

	current = ht->shead;
	while (current != NULL && strcmp(current->key, node->key) < 0)
		current = current->snext;

	if (current == NULL)
	{
		node->sprev = ht->stail;
		ht->stail->snext = node;
		ht->stail = node;
	}
	else
	{
		node->snext = current;
		node->sprev = current->sprev;

		if (current->sprev != NULL)
			current->sprev->snext = node;
		else
			ht->shead = node;

		current->sprev = node;
	}
}

/**
 * shash_table_get - retrieves a value
 * @ht: sorted hash table
 * @key: key to find
 *
 * Return: value, or NULL
 */
char *shash_table_get(const shash_table_t *ht, const char *key)
{
	unsigned long int index;
	shash_node_t *current;

	if (ht == NULL || key == NULL || *key == '\0')
		return (NULL);

	index = key_index((const unsigned char *)key, ht->size);
	current = ht->array[index];

	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}

	return (NULL);
}

/**
 * shash_table_print - prints the sorted hash table
 * @ht: sorted hash table
 *
 * Return: Nothing
 */
void shash_table_print(const shash_table_t *ht)
{
	shash_node_t *current;
	int first;

	if (ht == NULL)
		return;

	printf("{");
	first = 1;
	current = ht->shead;

	while (current != NULL)
	{
		if (!first)
			printf(", ");

		printf("'%s': '%s'", current->key, current->value);
		first = 0;
		current = current->snext;
	}

	printf("}\n");
}

/**
 * shash_table_print_rev - prints the sorted hash table in reverse
 * @ht: sorted hash table
 *
 * Return: Nothing
 */
void shash_table_print_rev(const shash_table_t *ht)
{
	shash_node_t *current;
	int first;

	if (ht == NULL)
		return;

	printf("{");
	first = 1;
	current = ht->stail;

	while (current != NULL)
	{
		if (!first)
			printf(", ");

		printf("'%s': '%s'", current->key, current->value);
		first = 0;
		current = current->sprev;
	}

	printf("}\n");
}

/**
 * shash_table_delete - deletes a sorted hash table
 * @ht: sorted hash table
 *
 * Return: Nothing
 */
void shash_table_delete(shash_table_t *ht)
{
	shash_node_t *current;
	shash_node_t *next;

	if (ht == NULL)
		return;

	current = ht->shead;
	while (current != NULL)
	{
		next = current->snext;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}

	free(ht->array);
	free(ht);
}
