#ifndef LKL_DECL_H_
#define LKL_DECL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/select.h>


struct sockaddr;
struct pollfd;
struct msghdr;
struct timeval;

long lkl_accept4(int fd, struct sockaddr * upeer_sockaddr, int * upeer_addrlen, int flags);
long lkl_accept(int fd, struct sockaddr * upeer_sockaddr, unsigned int * upeer_addrlen);
long lkl_bind(int fd, const struct sockaddr * umyaddr, int addrlen);
long lkl_close(unsigned int fd);
long lkl_connect(int fd, const struct sockaddr * uservaddr, int addrlen);
long lkl_getpeername(int fd, struct sockaddr * usockaddr, unsigned int * usockaddr_len);
long lkl_getsockname(int fd, struct sockaddr * usockaddr, unsigned int * usockaddr_len);
long lkl_getsockopt(int fd, int level, int optname, const char * optval, unsigned int * optlen);
long lkl_ioctl(int fd, unsigned long req, ...);
long lkl_listen(int fd, int backlog);
long lkl_poll(struct pollfd * ufds, unsigned int nfds, int timeout_msecs);
long lkl_recvmsg(int fd, struct msghdr * msg, unsigned int flags);
long lkl_select(int n, fd_set * inp, fd_set * outp, fd_set * exp, struct timeval * tvp);
long lkl_sendmsg(int fd, struct msghdr * msg, unsigned int flags);
long lkl_setsockopt(int fd, int level, int optname, const char * optval, int optlen);
long lkl_shutdown(int fd, int how);
long lkl_socket(int family, int type, int protocol);
long lkl_socketpair(int family, int type, int protocol, int * usockvec);

#ifdef __cplusplus
}
#endif

#endif /* LKL_DECL_H_ */
