#include <pcap.h>
#include <stdbool.h>
#include <stdio.h>

void usage() {
	printf("syntax: pcap-test <interface>\n");
	printf("sample: pcap-test wlan0\n");
}

typedef struct {
	char* dev_;
} Param;

Param param  = {
	.dev_ = NULL
};

bool parse(Param* param, int argc, char* argv[]) {
	if (argc != 2) {
		usage();
		return false;
	}
	param->dev_ = argv[1];
	return true;
}

void hexdump(u_char* data, int len) {
	char ascii[17] = {0};
	for (int i = 0; i < len; i++) {
		printf("%02x ", data[i]);
		if (data[i] >= ' ' && data[i] <= '~') {
			ascii[i % 16] = data[i];
		} else {
			ascii[i % 16] = '.';
		}

		if ((i+1) % 16 == 0) {
			printf("  %s\n", ascii);
		}
		else if ((i+1) == len) {
			ascii[(i+1) % 16] = '\0';
			for (int j = (i+1)%16; j < 16; j++) {
				printf("   ");
			}
			printf("  %s\n", ascii);
		}
	}
}

int parseEthernet(u_char* packet) {
	printf("Dst MAC  : %02x:%02x:%02x:%02x:%02x:%02x\n", packet[0], packet[1], packet[2], packet[3], packet[4], packet[5]);
	printf("Src MAC  : %02x:%02x:%02x:%02x:%02x:%02x\n", packet[6], packet[7], packet[8], packet[9], packet[10], packet[11]);

	if (packet[12] == 0x08 && packet[13] == 0x00) {
		return 1;
	} else {
		return 0;
	}
}


int parseIP(u_char* packet) {
	printf("Dst IP   : %d.%d.%d.%d\n", packet[30], packet[31], packet[32], packet[33]);
	printf("Src IP   : %d.%d.%d.%d\n", packet[26], packet[27], packet[28], packet[29]);

	if (packet[23] == 17) {
		return 1;
	} else if (packet[23] == 6) {
		return 0;
	} else {
		return -1;
	}
}

void parseUDP(u_char* packet) {
	printf("Dst port : %d\n", packet[36] * 16 + packet[37]);
	printf("Src port : %d\n", packet[34] * 16 + packet[35]);
}

void parseTCP(u_char* packet) {
	printf("Dst port : %d\n", packet[36] * 16 + packet[37]);
	printf("Src port : %d\n", packet[34] * 16 + packet[35]);
}

int main(int argc, char* argv[]) {
	if (!parse(&param, argc, argv))
		return -1;

	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* pcap = pcap_open_live(param.dev_, BUFSIZ, 1, 1000, errbuf);
	if (pcap == NULL) {
		fprintf(stderr, "pcap_open_live(%s) return null - %s\n", param.dev_, errbuf);
		return -1;
	}

	while (true) {
		struct pcap_pkthdr* header;
		const u_char* packet;
		int res = pcap_next_ex(pcap, &header, &packet);
		if (res == 0) continue;
		if (res == PCAP_ERROR || res == PCAP_ERROR_BREAK) {
			printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(pcap));
			break;
		}
		printf("%u bytes captured\n", header->caplen);

		if (parseEthernet(packet)) {
			int protocol = parseIP(packet);
			if (protocol == 0) {
				parseTCP(packet);
			} else if (protocol == 1) {
				parseUDP(packet);
			}
		}
 
		hexdump(packet, header->caplen);
		putchar(10);
	}

	pcap_close(pcap);
}
