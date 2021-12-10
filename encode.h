#ifndef ENCODE_H
#define ENCODE_H (1)
/** 
 * @brief Coverts 24 bit integer in 4 butes base 64
 *
 * @param n		24bit integer
 * @param buf	pointer to four bytes output container
 */
void ito64(int n, char buf[4]);

/** 
 * @brief Coverts 24 bit integer in 4 butes base 64
 *
 * @param x		24bit integer
 * @param n		number of input bytes
 * @param buf	pointer to four bytes output container
 */
void into64(int32_t x, char buf[4], int n);

/**
 * coverts binary array to base 64 string
 * 
 * @src input data
 * @src_len size of data in bytes
 * @dest	pointer to buffer where the reult will be stored with proper size, dest_size = ((src_len+2)/3)<<2;
 */
void encode(char *dest, const char *src, int src_len);
#endif /* ENCODE_H */

