/** \copyright
 * Copyright (c) 2016, Stuart W Baker
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are  permitted provided that the following conditions are met:
 * 
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file include/sys/select.h
 * This file imlements POSIX select().
 *
 * @author Stuart W. Baker
 * @date 31 July 2016
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#include "socket.h"

/*
 * ::socket()
 */
int socket(int domain, int type, int protocol)
{
    switch (domain)
    {
        case AF_INET:
            domain = SL_AF_INET;
            break;
        case AF_INET6:
            domain = SL_AF_INET6;
            break;
        case AF_PACKET:
            domain = SL_AF_PACKET;
            break;
        default:
            errno = EAFNOSUPPORT;
            return -1;
    }

    switch (type)
    {
        case SOCK_STREAM:
            type = SL_SOCK_STREAM;
            break;
        case SOCK_DGRAM:
            type = SL_SOCK_DGRAM;
            break;
        case SOCK_RAW:
            type = SL_SOCK_RAW;
            break;
        default:
            errno = EINVAL;
            return -1;
    }

    switch (protocol)
    {
        case 0:
            break;
        case IPPROTO_TCP:
            protocol = SL_IPPROTO_TCP;
            break;
        case IPPROTO_UDP:
            protocol = SL_IPPROTO_UDP;
            break;
        case IPPROTO_RAW:
            protocol = SL_IPPROTO_RAW;
            break;
        default:
            errno = EINVAL;
            return -1;
    }

    int result = sl_Socket(domain, type, protocol);

    if (result < 0)
    {
        switch (result)
        {
            default:
                errno = EINVAL;
                break;
            case SL_EAFNOSUPPORT:
                errno = EAFNOSUPPORT;
                break;
            case SL_EPROTOTYPE:
                errno = EPROTOTYPE;
                break;
            case SL_EACCES:
                errno = EACCES;
                break;
            case SL_ENSOCK:
                errno = EMFILE;
                break;
            case SL_POOL_IS_EMPTY:
            case SL_ENOMEM:
                errno = ENOMEM;
                break;
            case SL_EINVAL:
                errno = EINVAL;
                break;
            case SL_EPROTONOSUPPORT:
                errno = EPROTONOSUPPORT;
                break;
            case SL_EOPNOTSUPP:
                errno = EOPNOTSUPP;
                break;
        }
        return -1;
    }
    
    return result;
}

/*
 * ::bind()
 */
int bind(int s, const struct sockaddr *address, socklen_t address_len)
{
    SlSockAddr_t sl_address;
    sl_address.sa_family = address->sa_family;
    memcpy(sl_address.sa_data, address->sa_data, sizeof(sl_address.sa_data));

    int result = sl_Bind(s, &sl_address, address_len);

    if (result < 0)
    {
        switch (result)
        {
            default:
                errno = EINVAL;
                break;
            case SL_POOL_IS_EMPTY:
                errno = ENOMEM;
        }
        return -1;
    }
 
    return 0;  
}

/*
 * ::listen()
 */
int listen(int s, int backlog)
{
    int result = sl_Listen(s, backlog);

    if (result < 0)
    {
        switch (result)
        {
            default:
                errno = EOPNOTSUPP;
                break;
        }
        return -1;
    }

    return 0;  
}

/*
 * ::accept()
 */
int accept(int s, struct sockaddr *address, socklen_t *address_len)
{
    SlSockAddr_t sl_address;
    SlSocklen_t sl_address_len;

    int result = sl_Accept(s, &sl_address, &sl_address_len);

    if (address && address_len)
    {
        memcpy(address, &sl_address, *address_len);
    }

    if (result < 0)
    {
        switch (result)
        {
            default:
                errno = EINVAL;
                break;
            case SL_POOL_IS_EMPTY:
                usleep(10000);
                /* fall through */
            case SL_EAGAIN:
                errno = EAGAIN;
                break;
        }
        return -1;
    }

    return result;
}

/*
 * ::connect()
 */
int connect(int s, const struct sockaddr *address, socklen_t address_len)
{
    SlSockAddr_t sl_address;
    sl_address.sa_family = address->sa_family;
    memcpy(sl_address.sa_data, address->sa_data, sizeof(sl_address.sa_data));

    int result = sl_Connect(s, &sl_address, address_len);

    if (result < 0)
    {
        switch (result)
        {
            default:
            {
                errno = EINVAL;
                break;
            }
            case SL_EALREADY:
                errno = EALREADY;
                break;
            case SL_POOL_IS_EMPTY:
                usleep(10000);
            /* fall through */
            case SL_EAGAIN:
                errno = EAGAIN;
                break;
        }
        return -1;
    }

    return 0;  
}

/*
 * ::recv()
 */
int recv(int s, void *buffer, size_t length, int flags)
{
    int result = sl_Recv(s, buffer, length, flags);

    if (result < 0)
    {
        switch (result)
        {
            default:
                errno = EINVAL;
                break;
            case SL_POOL_IS_EMPTY:
                usleep(10000);
                /* fall through */
            case SL_EAGAIN:
                errno = EAGAIN;
                break;
        }
        return -1;
    }

    return result;  
}

/*
 * ::send()
 */
int send(int s, const void *buffer, size_t length, int flags)
{
    int result = sl_Send(s, buffer, length, flags);

    if (result < 0)
    {
        switch (result)
        {
            default:
                errno = EINVAL;
                break;
        }
        return -1;
    }

    return result;  
}
/*
 * ::setsocketopt()
 */
int setsockopt(int s, int level, int option_name,
               const void *option_value, socklen_t option_len)
{
    int result;

    switch (level)
    {
        default:
            errno = EINVAL;
            return -1;
        case SOL_SOCKET:
            level = SL_SOL_SOCKET;
            switch (option_name)
            {
                default:
                    errno = EINVAL;
                    return -1;
                case SO_REUSEADDR:
                    /* CC32xx does not care about SO_REUSEADDR, ignore it */
                    result = 0;
                    break;
            }
            break;
        case IPPROTO_TCP:
            switch (option_name)
            {
                default:
                    errno = EINVAL;
                    return -1;
                case TCP_NODELAY:
                    /* CC32xx does not care about Nagel algorithm, ignore it */
                    result = 0;
                    break;
            }
            break;
    }
                    
    if (result < 0)
    {
        switch (result)
        {
            default:
                errno = EINVAL;
                break;
        }
        return -1;
    }

    return result;
}

/*
 * ::close() or ::_close_r() for newlib
 */
#if defined(__TI_COMPILER_VERSION__)
int close(int fd)
#else
int _close_r(struct _reent *reent, int s)
#endif
{
    int result = sl_Close(s);

    if (result < 0)
    {
        switch (result)
        {
            default:
                errno = EBADF;
                break;
        }
        return -1;
    }

    return 0;
}

/*
 * ::gai_strerror
 */
const char *gai_strerror (int ecode)
{
    return "gai_strerror unkown";
}

/*
 * ::freeaddrinfo
 */
void freeaddrinfo(struct addrinfo *ai)
{
    free(ai->ai_addr);
    free(ai);
}

/*
 * ::getaddrinfo
 */
int getaddrinfo(const char *nodename, const char *servname,
                const struct addrinfo *hints,
                struct addrinfo **res)
{
    uint32_t ip_addr;
    uint8_t domain;

    *res = malloc(sizeof(struct addrinfo));
    if (*res == NULL)
    {
        return EAI_MEMORY;
    }
    memset(*res, 0, sizeof(struct addrinfo));

    (*res)->ai_addr = malloc(sizeof(struct sockaddr));
    if ((*res)->ai_addr == NULL)
    {
        free(*res);
        return EAI_MEMORY;
    }
    memset((*res)->ai_addr, 0, sizeof(struct addrinfo));

    switch (hints->ai_family)
    {
        case AF_INET:
            domain = SL_AF_INET;
            break;
        case AF_INET6:
            domain = SL_AF_INET6;
            break;
        case AF_PACKET:
            domain = SL_AF_PACKET;
            break;
        default:
            errno = EAFNOSUPPORT;
            free((*res)->ai_addr);
            free(*res);
            return -1;
    }

    int result = sl_NetAppDnsGetHostByName((int8_t*)nodename, strlen(nodename),
                                           &ip_addr, domain);

    if (result != 0)
    {
        free((*res)->ai_addr);
        free(*res);
        switch (result)
        {
            default:
            case SL_POOL_IS_EMPTY:
                return EAI_AGAIN;
            case SL_NET_APP_DNS_QUERY_NO_RESPONSE:
            case SL_NET_APP_DNS_NO_SERVER:
            case SL_NET_APP_DNS_QUERY_FAILED:
            case SL_NET_APP_DNS_MALFORMED_PACKET:
            case SL_NET_APP_DNS_MISMATCHED_RESPONSE:
                return EAI_FAIL;
        }
    }

    switch (hints->ai_family)
    {
        case AF_INET:
        {
            struct sockaddr_in *addr_in = (struct sockaddr_in*)(*res)->ai_addr;
            (*res)->ai_flags = 0;
            (*res)->ai_family = hints->ai_family;
            (*res)->ai_socktype = hints->ai_socktype;
            (*res)->ai_protocol = hints->ai_protocol;
            (*res)->ai_addrlen = sizeof(struct sockaddr_in);
            addr_in->sin_family = hints->ai_family;
            addr_in->sin_port = htons((uint16_t)strtol(servname, NULL, 0));
            addr_in->sin_addr.s_addr = htonl(ip_addr);
            break;
        }
        case AF_INET6:
        case AF_PACKET:
        default:
            errno = EAFNOSUPPORT;
            free((*res)->ai_addr);
            free(*res);
            return -1;
    }

    return 0;
}

