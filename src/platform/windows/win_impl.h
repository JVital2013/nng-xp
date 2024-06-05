//
// Copyright 2024 Staysail Systems, Inc. <info@staysail.tech>
// Copyright 2018 Capitar IT Group BV <info@capitar.com>
//
// This software is supplied under the terms of the MIT License, a
// copy of which should be located in the distribution where this
// file was obtained (LICENSE.txt).  A copy of the license may also be
// found online at https://opensource.org/licenses/MIT.
//

#ifndef PLATFORM_WIN_IMPL_H
#define PLATFORM_WIN_IMPL_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// These headers must be included first.
#include <windows.h>
#include <winsock2.h>

#include <mswsock.h>
#include <process.h>
#include <ws2tcpip.h>

#include "core/list.h"

#include <pthread.h>
// These types are provided for here, to permit them to be directly inlined
// elsewhere.

struct nni_plat_thr {
	pthread_t tid;
	void (*func)(void *);
	void *arg;
};

struct nni_plat_mtx {
	pthread_mutex_t mtx;
};

#define NNI_MTX_INITIALIZER               \
	{                                 \
		PTHREAD_MUTEX_INITIALIZER \
	}

struct nni_rwlock {
	pthread_rwlock_t rwl;
};

#define NNI_RWLOCK_INITIALIZER             \
	{                                  \
		PTHREAD_RWLOCK_INITIALIZER \
	}

// No static form of CV initialization because of the need to use
// attributes to set the clock type.
struct nni_plat_cv {
	pthread_cond_t cv;
	nni_plat_mtx  *mtx;
};

// NOTE: condition variables initialized with this should *NOT*
// be used with nni_cv_until -- the clock attributes are not passed
// and the wake-up times will not be correct.
#define NNI_CV_INITIALIZER(mxp)                            \
	{                                                  \
		.mtx = mxp, .cv = PTHREAD_COND_INITIALIZER \
	}


struct nni_atomic_flag {
	LONG f;
};

struct nni_atomic_bool {
	LONG v;
};

struct nni_atomic_int {
	LONG v;
};

struct nni_atomic_u64 {
	LONGLONG v;
};

struct nni_atomic_ptr {
	LONGLONG v;
};

// nni_win_io is used with io completion ports.  This allows us to get
// to a specific completion callback without requiring the poller (in the
// completion port) to know anything about the event itself.

typedef struct nni_win_io nni_win_io;
typedef void (*nni_win_io_cb)(nni_win_io *, int, size_t);

struct nni_win_io {
	OVERLAPPED    olpd;
	HANDLE        f;
	void         *ptr;
	nni_aio      *aio;
	nni_win_io_cb cb;
};

struct nni_plat_flock {
	HANDLE h;
};

extern int nni_win_error(int);

extern int nni_win_tcp_conn_init(nni_tcp_conn **, SOCKET);

extern int  nni_win_io_sysinit(void);
extern void nni_win_io_sysfini(void);

extern int  nni_win_ipc_sysinit(void);
extern void nni_win_ipc_sysfini(void);

extern int  nni_win_tcp_sysinit(void);
extern void nni_win_tcp_sysfini(void);

extern int  nni_win_udp_sysinit(void);
extern void nni_win_udp_sysfini(void);

extern int  nni_win_resolv_sysinit(void);
extern void nni_win_resolv_sysfini(void);

extern void nni_win_io_init(nni_win_io *, nni_win_io_cb, void *);

extern int nni_win_io_register(HANDLE);

extern int nni_win_sockaddr2nn(nni_sockaddr *, const SOCKADDR_STORAGE *);
extern int nni_win_nn2sockaddr(SOCKADDR_STORAGE *, const nni_sockaddr *);

#define NNG_PLATFORM_DIR_SEP "\\"

#endif // PLATFORM_WIN_IMPL_H
