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

char* chararray_init ();

void chararray_append (char** chararray, char c);

void chararray_free (char* chararray);

#endif
