/*
 * IncFile1.h
 *
 * Created: 28/09/2022 21:09:48
 *  Author: Kirill
 */ 


#ifndef UTIL_H_
#define UTIL_H_

static inline int is_odd(int x) { return x & 1; }

void set_bit(uint8_t reg, int bit) {
  reg |= (1 << bit);
  return;
}

void clear_bit(uint8_t reg, int bit) {
  reg &= ~(1 << bit);
  return;
}

#endif /* UTIL_H_ */
