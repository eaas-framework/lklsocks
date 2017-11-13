#ifndef LKL_HELPER_H_
#define LKL_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

void init_lkl(char const *cmd, char const * const vde_path, unsigned int addr, unsigned int netmask);
void halt_lkl(void);

#ifdef __cplusplus
}
#endif

#endif /* LKL_HELPER_H_ */
