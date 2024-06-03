#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

// ------ data definitions

struct message_id
{
  uint8_t year;   // year since 2000; ex: 24 for 2024
  uint8_t month;  // 1 through 12
  uint8_t day;    // 1 through 31
  uint8_t hour;   // 0 through 23
  uint8_t minute; // 0 through 59
  uint32_t random_tag;
};
typedef struct message_id message_id;

struct packet
{
  char magic[3]; // to contain 'S' 'O' 'S'
  message_id id;
  uint8_t n_packets;
  uint8_t packet_index;
  uint8_t packed_chars[5];
};
typedef struct packet packet;

typedef struct packet_list packet_list;
struct packet_list
{
  packet pkt;
  packet_list *next;
};

// remove leading and trailing spaces
char *trim(char *s);

// construct an ASCII version of SOS message
char *sos_translate(char *s);

// eliminate double spaces
char *squeeze_spaces(char *s);

// append two trailing spaces
char *sos_terminate(char *s);

// convert string to an array of SOS indexes
uint8_t *ascii_to_sos(char *s);

// pack SOS chars next to one another, 5 bits each
uint8_t *sos_pack(uint8_t *sos_indices, int len, int *outlen);

// put it all together...
uint8_t *sos_encode(char *message, int *outlen);

// create a list of SOS packets given a packed message
packet_list *sos_packetize(uint8_t *packed_message, int len);

// build a list with only those packets with the given message ID
// note: construct a new list; do not share nodes with the argument list
packet_list *filter_packets(packet_list *packets, message_id id);

// sort packets in ascending packet_index order
// GIGO if packets belong to different messages
packet_list *sort_packets(packet_list *packets);

// unpack packet's five bytes into eight bytes
uint8_t *sos_unpack(packet pkt);

// turn n bytes from SOS indices to ASCII chars; array is not NUL-terminated
char *sos_to_ascii(uint8_t *sos, int len);

// given list of packets all from same message, decode into string
// ?s for any portions where packet was lost
// does not require sorted order, resilient to duplicate packets
// GIGO if packets from more than one message
char *sos_decode(packet_list *packets);

// Free packet list
void pl_free(packet_list *pl);

// Create a package from input
packet_list *build_packet_list_from_input(char *input);