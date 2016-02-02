#include "lkl_decl.h"

#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <poll.h>
#include <lkl.h>
#include <errno.h>


long lkl_set_errno(long err);

long lkl_accept4(int fd, struct sockaddr * upeer_sockaddr, int * upeer_addrlen,
                 int flags) {
    return lkl_set_errno(lkl_sys_accept4(
            fd, (struct __lkl__kernel_sockaddr_storage *)upeer_sockaddr,
            upeer_addrlen, flags));
}

long lkl_accept(int fd, struct sockaddr * upeer_sockaddr,
                unsigned int * upeer_addrlen) {
    return lkl_set_errno(lkl_sys_accept(
            fd, (struct __lkl__kernel_sockaddr_storage *)upeer_sockaddr,
            upeer_addrlen));
}

long lkl_bind(int fd, const struct sockaddr * umyaddr, int addrlen) {
    return lkl_set_errno(lkl_sys_bind(fd, (struct __lkl__kernel_sockaddr_storage *)umyaddr,
                        addrlen));
}

long lkl_close(unsigned int fd) {
    return lkl_set_errno(lkl_sys_close(fd));
}

long lkl_connect(int fd, const struct sockaddr * uservaddr, int addrlen) {
    return lkl_set_errno(lkl_sys_connect(fd,
                           (struct __lkl__kernel_sockaddr_storage *)uservaddr,
                           addrlen));
}

long lkl_getpeername(int fd, struct sockaddr * usockaddr,
                     unsigned int * usockaddr_len) {
    return lkl_set_errno(lkl_sys_getpeername(
            fd, (struct __lkl__kernel_sockaddr_storage *)usockaddr,
            usockaddr_len));
}

long lkl_getsockname(int fd, struct sockaddr * usockaddr,
                     unsigned int * usockaddr_len) {
    return lkl_set_errno(lkl_sys_getsockname(
            fd, (struct __lkl__kernel_sockaddr_storage *)usockaddr,
            usockaddr_len));
}

long lkl_getsockopt(int fd, int level, int optname, const char * optval,
                    unsigned int * optlen) {
    return lkl_set_errno(lkl_sys_getsockopt(fd, level, optname, (char *)optval, optlen));
}

long lkl_listen(int fd, int backlog) {
    return lkl_set_errno(lkl_sys_listen(fd, backlog));
}

long lkl_poll(struct pollfd * ufds, unsigned int nfds, int timeout_msecs) {
    return lkl_set_errno(lkl_sys_poll((struct lkl_pollfd *)ufds, nfds, timeout_msecs));
}

long lkl_recvmsg(int fd, struct msghdr * msg, unsigned int flags) {
    return lkl_set_errno(lkl_sys_recvmsg(fd, (struct lkl_user_msghdr *)msg, flags));
}

long lkl_select(int n, fd_set * inp, fd_set * outp, fd_set * exp,
                struct timeval * tvp) {
    return lkl_set_errno(lkl_sys_select(n, (lkl_fd_set *)inp, (lkl_fd_set *)outp,
                          (lkl_fd_set *)exp, (struct lkl_timeval *)tvp));
}

long lkl_sendmsg(int fd, struct msghdr * msg, unsigned int flags) {
    return lkl_set_errno(lkl_sys_sendmsg(fd, (struct lkl_user_msghdr *)msg, flags));
}

long lkl_setsockopt(int fd, int level, int optname, const char * optval,
                    int optlen) {
    return lkl_set_errno(lkl_sys_setsockopt(fd, level, optname, (char *)optval, optlen));
}

long lkl_shutdown(int fd, int how) {
    return lkl_set_errno(lkl_sys_shutdown(fd, how));
}

long lkl_socket(int family, int type, int protocol) {
    return lkl_set_errno(lkl_sys_socket(family, type, protocol));
}

long lkl_socketpair(int family, int type, int protocol, int * usockvec) {
    return lkl_set_errno(lkl_sys_socketpair(family, type, protocol, usockvec));
}

long lkl_ioctl(int fd, unsigned long req, ...) {
    va_list vl;
    long arg;
    va_start(vl, req);
    arg = va_arg(vl, long);
    va_end(vl);

    return lkl_set_errno(lkl_sys_ioctl(fd, req, arg));
}

long lkl_set_errno(long err)
{
    if (err >= 0)
        return err;

    switch (err) {
    case -LKL_EPERM:
        errno = EPERM;
        break;
    case -LKL_ENOENT:
        errno = ENOENT;
        break;
    case -LKL_ESRCH:
        errno = ESRCH;
        break;
    case -LKL_EINTR:
        errno = EINTR;
        break;
    case -LKL_EIO:
        errno = EIO;
        break;
    case -LKL_ENXIO:
        errno = ENXIO;
        break;
    case -LKL_E2BIG:
        errno = E2BIG;
        break;
    case -LKL_ENOEXEC:
        errno = ENOEXEC;
        break;
    case -LKL_EBADF:
        errno = EBADF;
        break;
    case -LKL_ECHILD:
        errno = ECHILD;
        break;
    case -LKL_EAGAIN:
        errno = EAGAIN;
        break;
    case -LKL_ENOMEM:
        errno = ENOMEM;
        break;
    case -LKL_EACCES:
        errno = EACCES;
        break;
    case -LKL_EFAULT:
        errno = EFAULT;
        break;
    case -LKL_ENOTBLK:
        errno = ENOTBLK;
        break;
    case -LKL_EBUSY:
        errno = EBUSY;
        break;
    case -LKL_EEXIST:
        errno = EEXIST;
        break;
    case -LKL_EXDEV:
        errno = EXDEV;
        break;
    case -LKL_ENODEV:
        errno = ENODEV;
        break;
    case -LKL_ENOTDIR:
        errno = ENOTDIR;
        break;
    case -LKL_EISDIR:
        errno = EISDIR;
        break;
    case -LKL_EINVAL:
        errno = EINVAL;
        break;
    case -LKL_ENFILE:
        errno = ENFILE;
        break;
    case -LKL_EMFILE:
        errno = EMFILE;
        break;
    case -LKL_ENOTTY:
        errno = ENOTTY;
        break;
    case -LKL_ETXTBSY:
        errno = ETXTBSY;
        break;
    case -LKL_EFBIG:
        errno = EFBIG;
        break;
    case -LKL_ENOSPC:
        errno = ENOSPC;
        break;
    case -LKL_ESPIPE:
        errno = ESPIPE;
        break;
    case -LKL_EROFS:
        errno = EROFS;
        break;
    case -LKL_EMLINK:
        errno = EMLINK;
        break;
    case -LKL_EPIPE:
        errno = EPIPE;
        break;
    case -LKL_EDOM:
        errno = EDOM;
        break;
    case -LKL_ERANGE:
        errno = ERANGE;
        break;
    case -LKL_EDEADLK:
        errno = EDEADLK;
        break;
    case -LKL_ENAMETOOLONG:
        errno = ENAMETOOLONG;
        break;
    case -LKL_ENOLCK:
        errno = ENOLCK;
        break;
    case -LKL_ENOSYS:
        errno = ENOSYS;
        break;
    case -LKL_ENOTEMPTY:
        errno = ENOTEMPTY;
        break;
    case -LKL_ELOOP:
        errno = ELOOP;
        break;
    case -LKL_ENOMSG:
        errno = ENOMSG;
        break;
    case -LKL_EIDRM:
        errno = EIDRM;
        break;
    case -LKL_ECHRNG:
        errno = ECHRNG;
        break;
    case -LKL_EL2NSYNC:
        errno = EL2NSYNC;
        break;
    case -LKL_EL3HLT:
        errno = EL3HLT;
        break;
    case -LKL_EL3RST:
        errno = EL3RST;
        break;
    case -LKL_ELNRNG:
        errno = ELNRNG;
        break;
    case -LKL_EUNATCH:
        errno = EUNATCH;
        break;
    case -LKL_ENOCSI:
        errno = ENOCSI;
        break;
    case -LKL_EL2HLT:
        errno = EL2HLT;
        break;
    case -LKL_EBADE:
        errno = EBADE;
        break;
    case -LKL_EBADR:
        errno = EBADR;
        break;
    case -LKL_EXFULL:
        errno = EXFULL;
        break;
    case -LKL_ENOANO:
        errno = ENOANO;
        break;
    case -LKL_EBADRQC:
        errno = EBADRQC;
        break;
    case -LKL_EBADSLT:
        errno = EBADSLT;
        break;
    case -LKL_EBFONT:
        errno = EBFONT;
        break;
    case -LKL_ENOSTR:
        errno = ENOSTR;
        break;
    case -LKL_ENODATA:
        errno = ENODATA;
        break;
    case -LKL_ETIME:
        errno = ETIME;
        break;
    case -LKL_ENOSR:
        errno = ENOSR;
        break;
    case -LKL_ENONET:
        errno = ENONET;
        break;
    case -LKL_ENOPKG:
        errno = ENOPKG;
        break;
    case -LKL_EREMOTE:
        errno = EREMOTE;
        break;
    case -LKL_ENOLINK:
        errno = ENOLINK;
        break;
    case -LKL_EADV:
        errno = EADV;
        break;
    case -LKL_ESRMNT:
        errno = ESRMNT;
        break;
    case -LKL_ECOMM:
        errno = ECOMM;
        break;
    case -LKL_EPROTO:
        errno = EPROTO;
        break;
    case -LKL_EMULTIHOP:
        errno = EMULTIHOP;
        break;
    case -LKL_EDOTDOT:
        errno = EDOTDOT;
        break;
    case -LKL_EBADMSG:
        errno = EBADMSG;
        break;
    case -LKL_EOVERFLOW:
        errno = EOVERFLOW;
        break;
    case -LKL_ENOTUNIQ:
        errno = ENOTUNIQ;
        break;
    case -LKL_EBADFD:
        errno = EBADFD;
        break;
    case -LKL_EREMCHG:
        errno = EREMCHG;
        break;
    case -LKL_ELIBACC:
        errno = ELIBACC;
        break;
    case -LKL_ELIBBAD:
        errno = ELIBBAD;
        break;
    case -LKL_ELIBSCN:
        errno = ELIBSCN;
        break;
    case -LKL_ELIBMAX:
        errno = ELIBMAX;
        break;
    case -LKL_ELIBEXEC:
        errno = ELIBEXEC;
        break;
    case -LKL_EILSEQ:
        errno = EILSEQ;
        break;
    case -LKL_ERESTART:
        errno = ERESTART;
        break;
    case -LKL_ESTRPIPE:
        errno = ESTRPIPE;
        break;
    case -LKL_EUSERS:
        errno = EUSERS;
        break;
    case -LKL_ENOTSOCK:
        errno = ENOTSOCK;
        break;
    case -LKL_EDESTADDRREQ:
        errno = EDESTADDRREQ;
        break;
    case -LKL_EMSGSIZE:
        errno = EMSGSIZE;
        break;
    case -LKL_EPROTOTYPE:
        errno = EPROTOTYPE;
        break;
    case -LKL_ENOPROTOOPT:
        errno = ENOPROTOOPT;
        break;
    case -LKL_EPROTONOSUPPORT:
        errno = EPROTONOSUPPORT;
        break;
    case -LKL_ESOCKTNOSUPPORT:
        errno = ESOCKTNOSUPPORT;
        break;
    case -LKL_EOPNOTSUPP:
        errno = EOPNOTSUPP;
        break;
    case -LKL_EPFNOSUPPORT:
        errno = EPFNOSUPPORT;
        break;
    case -LKL_EAFNOSUPPORT:
        errno = EAFNOSUPPORT;
        break;
    case -LKL_EADDRINUSE:
        errno = EADDRINUSE;
        break;
    case -LKL_EADDRNOTAVAIL:
        errno = EADDRNOTAVAIL;
        break;
    case -LKL_ENETDOWN:
        errno = ENETDOWN;
        break;
    case -LKL_ENETUNREACH:
        errno = ENETUNREACH;
        break;
    case -LKL_ENETRESET:
        errno = ENETRESET;
        break;
    case -LKL_ECONNABORTED:
        errno = ECONNABORTED;
        break;
    case -LKL_ECONNRESET:
        errno = ECONNRESET;
        break;
    case -LKL_ENOBUFS:
        errno = ENOBUFS;
        break;
    case -LKL_EISCONN:
        errno = EISCONN;
        break;
    case -LKL_ENOTCONN:
        errno = ENOTCONN;
        break;
    case -LKL_ESHUTDOWN:
        errno = ESHUTDOWN;
        break;
    case -LKL_ETOOMANYREFS:
        errno = ETOOMANYREFS;
        break;
    case -LKL_ETIMEDOUT:
        errno = ETIMEDOUT;
        break;
    case -LKL_ECONNREFUSED:
        errno = ECONNREFUSED;
        break;
    case -LKL_EHOSTDOWN:
        errno = EHOSTDOWN;
        break;
    case -LKL_EHOSTUNREACH:
        errno = EHOSTUNREACH;
        break;
    case -LKL_EALREADY:
        errno = EALREADY;
        break;
    case -LKL_EINPROGRESS:
        errno = EINPROGRESS;
        break;
    case -LKL_ESTALE:
        errno = ESTALE;
        break;
    case -LKL_EUCLEAN:
        errno = EUCLEAN;
        break;
    case -LKL_ENOTNAM:
        errno = ENOTNAM;
        break;
    case -LKL_ENAVAIL:
        errno = ENAVAIL;
        break;
    case -LKL_EISNAM:
        errno = EISNAM;
        break;
    case -LKL_EREMOTEIO:
        errno = EREMOTEIO;
        break;
    case -LKL_EDQUOT:
        errno = EDQUOT;
        break;
    case -LKL_ENOMEDIUM:
        errno = ENOMEDIUM;
        break;
    case -LKL_EMEDIUMTYPE:
        errno = EMEDIUMTYPE;
        break;
    case -LKL_ECANCELED:
        errno = ECANCELED;
        break;
    case -LKL_ENOKEY:
        errno = ENOKEY;
        break;
    case -LKL_EKEYEXPIRED:
        errno = EKEYEXPIRED;
        break;
    case -LKL_EKEYREVOKED:
        errno = EKEYREVOKED;
        break;
    case -LKL_EKEYREJECTED:
        errno = EKEYREJECTED;
        break;
    case -LKL_EOWNERDEAD:
        errno = EOWNERDEAD;
        break;
    case -LKL_ENOTRECOVERABLE:
        errno = ENOTRECOVERABLE;
        break;
    case -LKL_ERFKILL:
        errno = ERFKILL;
        break;
    case -LKL_EHWPOISON:
        errno = EHWPOISON;
        break;
    }

    return -1;
}
