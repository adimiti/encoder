#ifndef ENCODE_H
#define ENCODE_H (1)
/** 
 * @brief Coverts 24 bit integer in 4 butes base 64
 *
 * @param n		24bit integer
 * @param buf	pointer to four bytes output container
 */
void ito64(int n, char buf[4]);

#endif /* ENCODE_H */

