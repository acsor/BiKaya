#ifndef BK_STRING_H
#define BK_STRING_H


/**
 * @param src String representation of integer number
 * @param base Base in which the output should be translated
 * @return The integer value of @c src in base @base, or @c BK_E_INVARG
 * if either @c src contained invalid digits or @c base was < 2.
 */
long int bk_atoi(char const *src, unsigned base);
/**
 * Like bk_atoi(), but in base 10.
 */
long int bk_atoi10(char const *src);
/**
 * Converts @n in string format, storing it into @c dest. If the needed
 * dimension to store the string exceeds @c length, then the remaining trailing
 * characters are discarded, storing a terminating <tt>\0</tt> character at
 * <tt>length - 1</tt>. Otherwise, the string is filled as needed.
 * @param n A base 10 number, possibly negative.
 * @return @c BK_E_GEN if an error occurred, @c BK_E_OK otherwise.
 */
int bk_itoa(int n, char *dest, unsigned length);

/**
 * @return Number of characters in @c str. No null-pointer checks are
 * performed.  */
int bk_strlen(char const *str);
/**
 * Copies at most @length characters from @c src to @c dest, stopping early if
 * the terminating @c '\0' character is encountered.
 * @return Number of characters effectively copied, or a @c BK_E_X error
 * otherwise.
 */
int bk_strncpy(char *dest, char const *src, unsigned length);
/**
 * Copies @c src into @c dest, until the terminating @c '\0' character is
 * encountered.
 * @return Number of characters effectively copied, including the @c '\0'
 * character, or a @c BK_E_X error otherwise.
 */
int bk_strcpy(char *dest, char const *src);
/**
 * Compares the two strings @c s and @c t until a terminating character is
 * met in one of them. No null-pointer errors are detected.
 * @param s First string to compare
 * @param t Second string to compare
 * @return @c 0 if the two strings are equal, a value <tt>v &lt; 0</tt> if @c s
 * comes before @c t, or a value <tt>v &gt; 0</tt> if @c s comes after @c t.
 */
int bk_strcmp(char *s, char *t);

/**
 * Fills the first @c n bytes of the memory area pointed to by @c dest with
 * @c d.
 * @return A pointer to @c dest
 */
void* bk_memset(void *dest, char d, unsigned n);
/**
 * Copies the first @c n bytes of the memory area pointed to by @c src to @c
 * dest.
 * @return A pointer to @c dest.
 */
void* bk_memcpy(void *dest, void const *src, unsigned n);
/**
 * @c bk_memcpy() alias.
 * @see bk_memcpy
 */
void* memcpy(void *dest, void const *src, unsigned n);
/* TODO Can we define memcpy as a function pointer? */
/* void* (*memcpy)(void*, const void *, unsigned); */


#endif
