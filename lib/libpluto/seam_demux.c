#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

time_t         packet_time=0;
pcap_dumper_t *packet_save;

void send_packet_setup_pcap(char *file)
{
	pcap_t *pt;

	pt = pcap_open_dead(DLT_NULL, 1500);
	packet_save = pcap_dump_open(pt, file);
}


bool
send_packet(struct state *st, const char *where, bool verbose)
{
    u_int8_t *ptr;
    unsigned long len;

    ptr = st->st_tpacket.ptr;
    len = (unsigned long) st->st_tpacket.len;

    printf("sending %lu bytes for %s through %s:%d to %s:%u (using #%lu)"
	   , (unsigned long) st->st_tpacket.len
	   , where
	   , st->st_interface->ip_dev->id_rname
	   , st->st_interface->port
	   , ip_str(&st->st_remoteaddr)
	   , st->st_remoteport
	   , st->st_serialno);

    DBG_dump(NULL, ptr, len);

    if(packet_save) {
	    char buf[1600];
	    struct pcap_pkthdr pp;
	    struct iphdr  *ip;
	    struct udphdr *udp;
	    u_int32_t *dlt;
	    int caplen = sizeof(struct iphdr)+sizeof(struct udphdr)+len;

	    dlt = (u_int32_t*)buf;
	    *dlt = PF_INET;

	    ip  = (struct iphdr *)&buf[4];
	    ip->version = 4;
	    ip->ihl     = 5;
	    ip->tos     = 0;
	    ip->tot_len = htons(caplen);
	    ip->id      = 0;
	    ip->frag_off= 0;
	    ip->ttl     = 64;
	    ip->protocol= IPPROTO_UDP;
	    ip->check   = 0;
	    ip->saddr   = htonl(0xc00a022d); /* 192.0.1.45 - west */
	    ip->daddr   = htonl(0xc0010217); /* 192.0.1.23 - east */
	    udp = (struct udphdr *)&buf[sizeof(struct iphdr)+4];
	    udp->source = htons(500);
	    udp->dest   = htons(500);
	    udp->len    = htons(sizeof(struct udphdr)+len);
	    udp->check  = 0;
	    
	    memcpy(&buf[sizeof(struct iphdr)+sizeof(struct udphdr)+4],ptr,len);

	    packet_time  = packet_time + 86400;
	    pp.ts.tv_sec = packet_time;
	    pp.ts.tv_usec= 0;
	    pp.caplen = caplen+4;
	    pp.len    = caplen+4;
	    pcap_dump((u_char *)packet_save, &pp, buf);
    }
}


void
complete_state_transition(struct msg_digest **mdp, stf_status result)
{
	fprintf(stderr, "transitioning on result: %s\n"
		, enum_name(&stfstatus_name, result));
}
