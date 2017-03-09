/* a simple hash function similiar to what perl does for strings.
 * for good results, the string should not be excessivly large.
 */
static unsigned long lh_perllike_str_hash(const void *k)
{
    const char *rkey = (const char *)k;
    unsigned hashval = 1;

    while (*rkey)
        hashval = hashval * 33 + *rkey++;

    return hashval;
}
