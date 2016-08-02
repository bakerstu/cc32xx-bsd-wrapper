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
 * \file netdb.h
 * This file implements POSIX netdb.h prototypes.
 *
 * @author Stuart W. Baker
 * @date 2 July 2016
 */

#ifndef _NETDB_H_
#define _NETDB_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** contains host resolution errors */
extern int h_errno;

/** Structor to contain info about a host resolution.
 */
struct hostent
{
    char  *h_name;      /**< Official name of host */
    char **h_aliases;   /**< Alias list */
    int    h_addrtype;  /**< Host address type */
    int    h_length;    /**< Length of address */
    char **h_addr_list; /**< List of addresses */
};

/** Resolve a host by its name or IPv4 address (in standard dot notation) or
 * an IPv6 (in standard colon notation).
 * This is not multi-thread safe as implemented or by BSD convention.  Please
 * use @ref getaddrinfo() for a thread safe alternative.
 * @param name hostname to resolve
 * @return pointer to the host name credentials on success, else NULL on error
 */
struct hostent *gethostbyname(const char *name);

#define HOST_NOT_FOUND 1
#define TRY_AGAIN      2
#define NO_RECOVERY    3
#define NO_DATA        4
#define NO_ADDRESS     NO_DATA

/** Structure to contain information about address of a service provider.
 */
struct addrinfo
{
    int ai_flags;             /**< Input flags */
    int ai_family;            /**< Protocol family for socket */
    int ai_socktype;          /**< Socket type */
    int ai_protocol;          /**< Protocol for socket */
    socklen_t ai_addrlen;     /**< Length of socket address */
    struct sockaddr *ai_addr; /**< Socket address for socket */
    char *ai_canonname;       /**< Canonical name for service location */
    struct addrinfo *ai_next; /**< Pointer to next in list */
};

/** Translate any addrinfo error codes to a human readable string.
 * @param ecode error code to translate
 */
const char *gai_strerror (int ecode);

/** Free a struct addrinfo previously allocated by a call to getaddrinfo()
 * @param ai struct addrinfo to free
 */
void freeaddrinfo(struct addrinfo *ai);

/** Given nodename and servname, identify a host and service.
 * @param nodename typically a host name
 * @param servname typically a port name
 * @param hints any hints to best bind the results to
 * @param res resulting information
 * @return 0 on success, -1 on failure
 */
int getaddrinfo(const char *nodename, const char *servname,
                const struct addrinfo *hints,
                struct addrinfo **res);

# define EAI_AGAIN    -3    /**< Temporary failure in name resolution */
# define EAI_FAIL     -4    /**< Non-recoverable failure in name res */
# define EAI_MEMORY   -10   /**< Memory allocation failure */

#ifdef __cplusplus
}
#endif

#endif /* _NETDB_ */
