/**
 * Project: IFJ22 Compiler
 * 
 * @file chararray.h
 * @brief Iterface for dynamic char array
 * 
 * @authors xmacur09
*/

#ifndef CHARARRAY_H
#define CHARARRAY_H

/**
 * @brief Creates new char array
 * 
 * @return char_array* 
 */
char* chararray_init ();

/**
 * @brief Adds a char to the char array
 * 
 * @param array 
 * @param c 
 */
void chararray_append (char** chararray, char c);

/**
 * @brief Adds a string to the char array
 * 
 * @param array 
 * @param c 
 */
void chararray_append_string(char **chararray, char* str);

/**
 * @brief Frees the char array
 * 
 * @param array 
 */
void chararray_free (char* chararray);

#endif
