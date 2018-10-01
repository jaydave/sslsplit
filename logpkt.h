/*-
 * SSLsplit - transparent SSL/TLS interception
 * https://www.roe.ch/SSLsplit
 *
 * Copyright (c) 2009-2018, Daniel Roethlisberger <daniel@roe.ch>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LOGPKT_H
#define LOGPKT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pcap.h>
#include <errno.h>

#include <libnet.h>
#if defined(__OpenBSD__) && !defined(ETHERTYPE_IPV6)
#include <net/ethertypes.h>
#endif /* __OpenBSD__ && !ETHERTYPE_IPV6 */

#define MSS_VAL 1420

typedef struct pcap_file_hdr {
	uint32_t magic_number;  /* magic number */
	uint16_t version_major; /* major version number */
	uint16_t version_minor; /* minor version number */
	uint32_t thiszone;      /* GMT to local correction */
	uint32_t sigfigs;       /* accuracy of timestamps */
	uint32_t snaplen;       /* max length of captured packets, in octets */
	uint32_t network;       /* data link type */
} pcap_file_hdr_t;

typedef struct pcap_rec_hdr {
	uint32_t ts_sec;        /* timestamp seconds */
	uint32_t ts_usec;       /* timestamp microseconds */
	uint32_t incl_len;      /* number of octets of packet saved in file */
	uint32_t orig_len;      /* actual length of packet */
} pcap_rec_hdr_t;

typedef struct pcap_packet {
	time_t epoch;
	unsigned int src_ip;
	struct libnet_in6_addr src_ip6;
	unsigned int dst_ip;
	struct libnet_in6_addr dst_ip6;
	int af;
	unsigned short src_port;
	unsigned short dst_port;
	unsigned int ack;
	unsigned int seq;
	unsigned char dst_ether[ETHER_ADDR_LEN];
} pcap_packet_t;

extern libnet_t *libnet_pcap;
extern libnet_t *libnet_mirror;
struct libnet_ether_addr *mirrorsender_ether;

int logpkt_write_global_pcap_hdr(int);
int logpkt_set_packet_fields(libnet_t *, pcap_packet_t *,
                             char *, char *, char *, char *);
int logpkt_write_packet(libnet_t *, int, pcap_packet_t *, char,
                        const unsigned char *, size_t);
int logpkt_write_payload(libnet_t *, int, pcap_packet_t *, pcap_packet_t *,
                         char, const unsigned char *, size_t);
int logpkt_check_mirrortarget(char *, char *, char *);

#endif /* !LOGPKT_H */
