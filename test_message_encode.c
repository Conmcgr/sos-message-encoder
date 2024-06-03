#include <stdio.h>
#include <stdint.h>
#include <criterion/criterion.h>
#include "message_encode.h"

// ======== tests

// -------- sos_packetize

Test(hw7_sos_packetize, sos_packetize00)
{
  uint8_t packed[] = {1, 2, 3, 4, 5, 6, 7, 8};
  packet_list *pl = sos_packetize(packed, 8);
  cr_assert(pl);
  cr_assert(pl->next);
  cr_assert(!pl->next->next);
  cr_assert(pl->pkt.id.year == 24);
  cr_assert(pl->next->pkt.id.year == 24);
  cr_assert(pl->pkt.id.month == 6);
  cr_assert(pl->next->pkt.id.month == 6);
  cr_assert(pl->pkt.id.day == pl->next->pkt.id.day);
  cr_assert(pl->pkt.id.hour == pl->next->pkt.id.hour);
  cr_assert(pl->pkt.id.minute == pl->next->pkt.id.minute);
  cr_assert(pl->pkt.id.random_tag == pl->next->pkt.id.random_tag);
  cr_assert(pl->pkt.magic[0] == 'S');
  cr_assert(pl->pkt.magic[1] == 'O');
  cr_assert(pl->pkt.magic[2] == 'S');
  cr_assert(pl->next->pkt.magic[0] == 'S');
  cr_assert(pl->next->pkt.magic[1] == 'O');
  cr_assert(pl->next->pkt.magic[2] == 'S');
  cr_assert(pl->pkt.n_packets == 2);
  cr_assert(pl->next->pkt.n_packets == 2);
  uint8_t *earlier = NULL, *later = NULL;
  if (pl->pkt.packet_index == 0)
  {
    earlier = pl->pkt.packed_chars;
  }
  else if (pl->pkt.packet_index == 1)
  {
    later = pl->pkt.packed_chars;
  }
  if (pl->next->pkt.packet_index == 0)
  {
    earlier = pl->next->pkt.packed_chars;
  }
  else if (pl->next->pkt.packet_index == 1)
  {
    later = pl->next->pkt.packed_chars;
  }
  cr_assert(earlier && later);
  cr_assert(earlier[0] == 1);
  cr_assert(earlier[1] == 2);
  cr_assert(earlier[2] == 3);
  cr_assert(earlier[3] == 4);
  cr_assert(earlier[4] == 5);
  cr_assert(later[0] == 6);
  cr_assert(later[1] == 7);
  cr_assert(later[2] == 8);
  cr_assert(later[3] == 0);
  cr_assert(later[4] == 0);
  free(pl->next);
  free(pl);
}

Test(hw7_sos_packetize, sos_packetize01)
{
  uint8_t *packed = NULL;
  packet_list *pl = sos_packetize(packed, 0);
  cr_assert(!pl);
  free(pl);
}

Test(hw7_sos_packetize, sos_packetize02)
{
  uint8_t packed[] = {'W', 'X', 'Y', 'Z', ' ', 1, 2, 3, 4, ' ', 9, ' ', 1, ' ', 1};
  packet_list *pl = sos_packetize(packed, 15);
  cr_assert(pl);
  cr_assert(pl->next);
  cr_assert(pl->next->next);
  cr_assert(!pl->next->next->next);
  cr_assert(pl->pkt.id.year == 24);
  cr_assert(pl->next->pkt.id.year == 24);
  cr_assert(pl->next->next->pkt.id.year == 24);
  cr_assert(pl->pkt.id.month == 6);
  cr_assert(pl->next->pkt.id.month == 6);
  cr_assert(pl->next->next->pkt.id.month == 6);
  cr_assert(pl->pkt.id.day == pl->next->pkt.id.day && pl->pkt.id.day == pl->next->next->pkt.id.day);
  cr_assert(pl->pkt.id.hour == pl->next->pkt.id.hour && pl->pkt.id.hour == pl->next->next->pkt.id.hour);
  cr_assert(pl->pkt.id.minute == pl->next->pkt.id.minute && pl->pkt.id.minute == pl->next->next->pkt.id.minute);
  cr_assert(pl->pkt.id.random_tag == pl->next->pkt.id.random_tag && pl->pkt.id.random_tag == pl->next->next->pkt.id.random_tag);
  cr_assert(pl->pkt.magic[0] == 'S');
  cr_assert(pl->pkt.magic[1] == 'O');
  cr_assert(pl->pkt.magic[2] == 'S');
  cr_assert(pl->next->pkt.magic[0] == 'S');
  cr_assert(pl->next->pkt.magic[1] == 'O');
  cr_assert(pl->next->pkt.magic[2] == 'S');
  cr_assert(pl->next->next->pkt.magic[0] == 'S');
  cr_assert(pl->next->next->pkt.magic[1] == 'O');
  cr_assert(pl->next->next->pkt.magic[2] == 'S');
  cr_assert(pl->pkt.n_packets == 3);
  cr_assert(pl->next->pkt.n_packets == 3);
  cr_assert(pl->next->next->pkt.n_packets == 3);
  uint8_t *earlier = NULL, *later = NULL, *latest = NULL;
  if (pl->pkt.packet_index == 0)
  {
    earlier = pl->pkt.packed_chars;
  }
  else if (pl->pkt.packet_index == 1)
  {
    later = pl->pkt.packed_chars;
  }
  else if (pl->pkt.packet_index == 2)
  {
    latest = pl->pkt.packed_chars;
  }
  if (pl->next->pkt.packet_index == 0)
  {
    earlier = pl->next->pkt.packed_chars;
  }
  else if (pl->next->pkt.packet_index == 1)
  {
    later = pl->next->pkt.packed_chars;
  }
  else if (pl->next->pkt.packet_index == 2)
  {
    latest = pl->next->pkt.packed_chars;
  }
  if (pl->next->next->pkt.packet_index == 0)
  {
    earlier = pl->next->next->pkt.packed_chars;
  }
  else if (pl->next->next->pkt.packet_index == 1)
  {
    later = pl->next->next->pkt.packed_chars;
  }
  else if (pl->next->next->pkt.packet_index == 2)
  {
    latest = pl->next->next->pkt.packed_chars;
  }
  cr_assert(earlier && later);
  cr_assert(latest);
  cr_assert(earlier[0] == 'W');
  cr_assert(earlier[1] == 'X');
  cr_assert(earlier[2] == 'Y');
  cr_assert(earlier[3] == 'Z');
  cr_assert(earlier[4] == ' ');
  cr_assert(later[0] == 1);
  cr_assert(later[1] == 2);
  cr_assert(later[2] == 3);
  cr_assert(later[3] == 4);
  cr_assert(later[4] == ' ');
  cr_assert(latest[0] == 9);
  cr_assert(latest[1] == ' ');
  cr_assert(latest[2] == 1);
  cr_assert(latest[3] == ' ');
  cr_assert(latest[4] == 1);
  free(pl->next->next);
  free(pl->next);
  free(pl);
}

// -------- filter_packets

Test(hw7_filter_packets, filter_packets00)
{
  packet_list *pl = (packet_list *)malloc(sizeof(packet_list));
  pl->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next->next->next = NULL;
  packet_list *t = pl;
  unsigned int i;
  for (i = 0; i < 4; i++)
  {
    t->pkt.id.year = 24;
    t->pkt.id.month = 5;
    t->pkt.id.day = 14;
    t->pkt.id.hour = 12;
    t->pkt.id.minute = 34;
    t->pkt.id.random_tag = (i % 2) ? 12345 : 6789;
    t->pkt.magic[0] = t->pkt.magic[2] = 'S';
    t->pkt.magic[1] = 'O';
    t->pkt.n_packets = 2;
    t->pkt.packet_index = i / 2;
    t->pkt.packed_chars[0] = t->pkt.packed_chars[1] = t->pkt.packed_chars[2] =
        t->pkt.packed_chars[3] = t->pkt.packed_chars[4] = 0;
    t = t->next;
  }
  packet_list *f = filter_packets(pl, pl->next->pkt.id);
  cr_assert(f);
  cr_assert(f->next);
  cr_assert(!f->next->next);
  cr_assert(f->pkt.id.random_tag == 12345);
  cr_assert(f->next->pkt.id.random_tag == 12345);
  cr_assert((f->pkt.packet_index == 0 && f->next->pkt.packet_index == 1) ||
            (f->pkt.packet_index == 1 && f->next->pkt.packet_index == 0));
  free(f->next);
  free(f);
  free(pl->next->next->next);
  free(pl->next->next);
  free(pl->next);
  free(pl);
}

Test(hw7_filter_packets, filter_packets01)
{
  packet_list *f = filter_packets(NULL, (message_id){24, 5, 14, 12, 34, 12345});
  cr_assert(f == NULL);
}

Test(hw7_filter_packets, filter_packets02)
{
  packet_list *pl = (packet_list *)malloc(sizeof(packet_list));
  pl->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next->next->next = NULL;
  packet_list *t = pl;
  unsigned int i;
  for (i = 0; i < 4; i++)
  {
    t->pkt.id.year = 24;
    t->pkt.id.month = 5;
    t->pkt.id.day = 14;
    t->pkt.id.hour = 12;
    t->pkt.id.minute = 34;
    t->pkt.id.random_tag = 6789;
    t->pkt.magic[0] = t->pkt.magic[2] = 'S';
    t->pkt.magic[1] = 'O';
    t->pkt.n_packets = 2;
    t->pkt.packet_index = i / 2;
    t->pkt.packed_chars[0] = t->pkt.packed_chars[1] = t->pkt.packed_chars[2] =
        t->pkt.packed_chars[3] = t->pkt.packed_chars[4] = 0;
    t = t->next;
  }
  message_id id = {24, 5, 14, 12, 34, 12345};
  packet_list *f = filter_packets(pl, id);
  cr_assert(f == NULL);
  free(pl->next->next->next);
  free(pl->next->next);
  free(pl->next);
  free(pl);
}

Test(hw7_filter_packets, filter_packets03)
{
  packet_list *pl = (packet_list *)malloc(sizeof(packet_list));
  pl->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next->next->next = NULL;
  packet_list *t = pl;
  unsigned int i;
  for (i = 0; i < 4; i++)
  {
    t->pkt.id.year = 24;
    t->pkt.id.month = 5;
    t->pkt.id.day = 14;
    t->pkt.id.hour = 12;
    t->pkt.id.minute = 34;
    t->pkt.id.random_tag = 12345;
    t->pkt.magic[0] = t->pkt.magic[2] = 'S';
    t->pkt.magic[1] = 'O';
    t->pkt.n_packets = 2;
    t->pkt.packet_index = i / 2;
    t->pkt.packed_chars[0] = t->pkt.packed_chars[1] = t->pkt.packed_chars[2] =
        t->pkt.packed_chars[3] = t->pkt.packed_chars[4] = 0;
    t = t->next;
  }
  message_id id = {24, 5, 14, 12, 34, 12345};
  packet_list *f = filter_packets(pl, id);
  cr_assert(f);
  cr_assert(f->next);
  cr_assert(f->next->next);
  cr_assert(f->next->next->next);
  cr_assert(!f->next->next->next->next);
  t = f;
  for (i = 0; i < 4; i++)
  {
    cr_assert(t->pkt.id.random_tag == 12345);
    t = t->next;
  }
  free(f->next->next->next);
  free(f->next->next);
  free(f->next);
  free(f);
  free(pl->next->next->next);
  free(pl->next->next);
  free(pl->next);
  free(pl);
}

// -------- sort_packets

Test(hw7_sort_packets, sort_packets00)
{
  packet_list *pl = (packet_list *)malloc(sizeof(packet_list));
  pl->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next->next = NULL;
  packet_list *t = pl;
  unsigned int i;
  for (i = 0; i < 3; i++)
  {
    t->pkt.id.year = 24;
    t->pkt.id.month = 5;
    t->pkt.id.day = 14;
    t->pkt.id.hour = 12;
    t->pkt.id.minute = 34;
    t->pkt.id.random_tag = 4567;
    t->pkt.magic[0] = t->pkt.magic[2] = 'S';
    t->pkt.magic[1] = 'O';
    t->pkt.n_packets = 3;
    t->pkt.packet_index = (i == 0) ? 2 : ((i == 1) ? 0 : 1);
    t->pkt.packed_chars[0] = t->pkt.packed_chars[1] = t->pkt.packed_chars[2] =
        t->pkt.packed_chars[3] = t->pkt.packed_chars[4] = 0;

    t = t->next;
  }
  packet_list *s = sort_packets(pl);
  cr_assert(s);
  cr_assert(s->next);
  cr_assert(s->next->next);
  cr_assert(!s->next->next->next);
  cr_assert(s->pkt.packet_index == 0);
  cr_assert(s->next->pkt.packet_index == 1);
  cr_assert(s->next->next->pkt.packet_index == 2);
  free(s->next->next);
  free(s->next);
  free(s);
}

Test(hw7_sort_packets, sort_packets01)
{
  packet_list *s = sort_packets(NULL);
  cr_assert(s == NULL);
}

Test(hw7_sort_packets, sort_packets02)
{
  packet_list *pl = (packet_list *)malloc(sizeof(packet_list));
  pl->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next->next = NULL;
  packet_list *t = pl;
  unsigned int i;
  for (i = 0; i < 3; i++)
  {
    t->pkt.id.year = 24;
    t->pkt.id.month = 5;
    t->pkt.id.day = 14;
    t->pkt.id.hour = 12;
    t->pkt.id.minute = 34;
    t->pkt.id.random_tag = 4567;
    t->pkt.magic[0] = t->pkt.magic[2] = 'S';
    t->pkt.magic[1] = 'O';
    t->pkt.n_packets = 3;
    t->pkt.packet_index = i;
    t->pkt.packed_chars[0] = t->pkt.packed_chars[1] = t->pkt.packed_chars[2] =
        t->pkt.packed_chars[3] = t->pkt.packed_chars[4] = 0;

    t = t->next;
  }
  packet_list *s = sort_packets(pl);
  cr_assert(s);
  cr_assert(s->next);
  cr_assert(s->next->next);
  cr_assert(!s->next->next->next);
  cr_assert(s->pkt.packet_index == 0);
  cr_assert(s->next->pkt.packet_index == 1);
  cr_assert(s->next->next->pkt.packet_index == 2);
  free(s->next->next);
  free(s->next);
  free(s);
}

Test(hw7_sort_packets, sort_packets03)
{
  packet_list *pl = (packet_list *)malloc(sizeof(packet_list));
  pl->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next->next = NULL;
  packet_list *t = pl;
  unsigned int i;
  for (i = 0; i < 3; i++)
  {
    t->pkt.id.year = 24;
    t->pkt.id.month = 5;
    t->pkt.id.day = 14;
    t->pkt.id.hour = 12;
    t->pkt.id.minute = 34;
    t->pkt.id.random_tag = 4567;
    t->pkt.magic[0] = t->pkt.magic[2] = 'S';
    t->pkt.magic[1] = 'O';
    t->pkt.n_packets = 3;
    t->pkt.packet_index = 2 - i;
    t->pkt.packed_chars[0] = t->pkt.packed_chars[1] = t->pkt.packed_chars[2] =
        t->pkt.packed_chars[3] = t->pkt.packed_chars[4] = 0;

    t = t->next;
  }
  packet_list *s = sort_packets(pl);
  cr_assert(s);
  cr_assert(s->next);
  cr_assert(s->next->next);
  cr_assert(!s->next->next->next);
  cr_assert(s->pkt.packet_index == 0);
  cr_assert(s->next->pkt.packet_index == 1);
  cr_assert(s->next->next->pkt.packet_index == 2);
  free(s->next->next);
  free(s->next);
  free(s);
}

Test(hw7_sort_packets, sort_packets04)
{
  packet_list *pl = (packet_list *)malloc(sizeof(packet_list));
  pl->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next->next->next = NULL;
  packet_list *t = pl;
  unsigned int i;
  for (i = 0; i < 4; i++)
  {
    t->pkt.id.year = 24;
    t->pkt.id.month = 5;
    t->pkt.id.day = 14;
    t->pkt.id.hour = 12;
    t->pkt.id.minute = 34;
    t->pkt.id.random_tag = 4567;
    t->pkt.magic[0] = t->pkt.magic[2] = 'S';
    t->pkt.magic[1] = 'O';
    t->pkt.n_packets = 3;
    t->pkt.packet_index = (i % 2);
    t->pkt.packed_chars[0] = t->pkt.packed_chars[1] = t->pkt.packed_chars[2] =
        t->pkt.packed_chars[3] = t->pkt.packed_chars[4] = 0;

    t = t->next;
  }
  packet_list *s = sort_packets(pl);
  cr_assert(s);
  cr_assert(s->next);
  cr_assert(s->next->next);
  cr_assert(s->next->next->next);
  cr_assert(!s->next->next->next->next);
  cr_assert(s->pkt.packet_index == 0);
  cr_assert(s->next->pkt.packet_index == 0);
  cr_assert(s->next->next->pkt.packet_index == 1);
  cr_assert(s->next->next->next->pkt.packet_index == 1);
  free(s->next->next->next);
  free(s->next->next);
  free(s->next);
  free(s);
}

// -------- sos_unpack

Test(hw7_sos_unpack, sos_unpack00)
{
  packet pkt;
  pkt.id.year = 24;
  pkt.id.month = 5;
  pkt.id.day = 14;
  pkt.id.hour = 12;
  pkt.id.minute = 34;
  pkt.id.random_tag = 1248;
  pkt.magic[0] = pkt.magic[2] = 'S';
  pkt.magic[1] = 'O';
  pkt.n_packets = 2;
  pkt.packet_index = 1;
  //    30    29    27    26    25    11    10    12
  // 11110 11101 11011 11010 11001 01011 01010 01100
  // 1111011101110111101011001010110101001100
  // 1111011101110111101011001010110101001100
  // 1111 0111  0111 0111  1010 1100  1010 1101  0100 1100
  //     F7         77         AC         AD         4C
  pkt.packed_chars[0] = 0xF7;
  pkt.packed_chars[1] = 0x77;
  pkt.packed_chars[2] = 0xAC;
  pkt.packed_chars[3] = 0xAD;
  pkt.packed_chars[4] = 0x4C;

  uint8_t *up = sos_unpack(pkt);

  cr_assert(up[0] == 30);
  cr_assert(up[1] == 29);
  cr_assert(up[2] == 27);
  cr_assert(up[3] == 26);
  cr_assert(up[4] == 25);
  cr_assert(up[5] == 11);
  cr_assert(up[6] == 10);
  cr_assert(up[7] == 12);

  free(up);
}

Test(hw7_sos_unpack, sos_unpack01)
{
  packet pkt;
  pkt.id.year = 24;
  pkt.id.month = 5;
  pkt.id.day = 14;
  pkt.id.hour = 12;
  pkt.id.minute = 34;
  pkt.id.random_tag = 1248;
  pkt.magic[0] = pkt.magic[2] = 'S';
  pkt.magic[1] = 'O';
  pkt.n_packets = 2;
  pkt.packet_index = 1;

  for (int i = 0; i < 5; i++)
  {
    pkt.packed_chars[i] = 0x00;
  }

  uint8_t *up = sos_unpack(pkt);

  for (int i = 0; i < 8; i++)
  {
    cr_assert(up[i] == 0);
  }

  free(up);
}

Test(hw7_sos_unpack, sos_unpack02)
{
  packet pkt;
  pkt.id.year = 24;
  pkt.id.month = 5;
  pkt.id.day = 14;
  pkt.id.hour = 12;
  pkt.id.minute = 34;
  pkt.id.random_tag = 1248;
  pkt.magic[0] = pkt.magic[2] = 'S';
  pkt.magic[1] = 'O';
  pkt.n_packets = 2;
  pkt.packet_index = 1;

  for (int i = 0; i < 5; i++)
  {
    pkt.packed_chars[i] = 0xFF;
  }

  uint8_t *up = sos_unpack(pkt);

  for (int i = 0; i < 8; i++)
  {
    cr_assert(up[i] == 31); // 11111 in binary
  }

  free(up);
}

Test(hw7_sos_unpack, sos_unpack03)
{
  packet pkt;
  pkt.id.year = 24;
  pkt.id.month = 5;
  pkt.id.day = 14;
  pkt.id.hour = 12;
  pkt.id.minute = 34;
  pkt.id.random_tag = 1248;
  pkt.magic[0] = pkt.magic[2] = 'S';
  pkt.magic[1] = 'O';
  pkt.n_packets = 2;
  pkt.packet_index = 1;

  //    15    14    13    12    11    10    9    8
  // 01111 01110 01101 01100 01011 01010 01001 01000
  // 01111011100110101100010110101001001000
  // 0111 1011  1001 1010 1100 0101 1010 1001 0010 1000
  //     7B         9A         C5         A9         28
  pkt.packed_chars[0] = 0x7B;
  pkt.packed_chars[1] = 0x9A;
  pkt.packed_chars[2] = 0xC5;
  pkt.packed_chars[3] = 0xA9;
  pkt.packed_chars[4] = 0x28;

  uint8_t *up = sos_unpack(pkt);

  cr_assert(up[0] == 15);
  cr_assert(up[1] == 14);
  cr_assert(up[2] == 13);
  cr_assert(up[3] == 12);
  cr_assert(up[4] == 11);
  cr_assert(up[5] == 10);
  cr_assert(up[6] == 9);
  cr_assert(up[7] == 8);

  free(up);
}

// -------- sos_to_ascii

Test(hw7_sos_to_ascii, sos_to_ascii00)
{
  uint8_t enc[7] = {16, 13, 19, 22, 31, 26, 5};
  char *expected = "HELP U5  ";
  char *actual = sos_to_ascii(enc, 7);

  for (int i = 0; i < 7; i++)
    cr_assert(actual[i] == expected[i]);

  free(actual);
}

Test(hw7_sos_to_ascii, sos_to_ascii01)
{
  uint8_t *enc = NULL;
  char *actual = sos_to_ascii(enc, 0);
  cr_assert(!actual);
  free(actual);
}

Test(hw7_sos_to_ascii, sos_to_ascii02)
{
  uint8_t enc[11] = {1, 2, 3, 31, 16, 3, 19, 22, 31, 20, 3};
  char *expected = "123 H3LP M3 ";
  char *actual = sos_to_ascii(enc, 11);

  for (int i = 0; i < 11; i++)
    cr_assert(actual[i] == expected[i]);

  free(actual);
}

// -------- sos_decode

Test(hw7_sos_decode, sos_decode00)
{
  packet_list *pl = (packet_list *)malloc(sizeof(packet_list));
  pl->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next = NULL;
  packet_list *t = pl;
  unsigned int i;
  for (i = 0; i < 2; i++)
  {
    t->pkt.id.year = 24;
    t->pkt.id.month = 5;
    t->pkt.id.day = 14;
    t->pkt.id.hour = 12;
    t->pkt.id.minute = 34;
    t->pkt.id.random_tag = 9876;
    t->pkt.magic[0] = t->pkt.magic[2] = 'S';
    t->pkt.magic[1] = 'O';
    t->pkt.n_packets = 2;
    t->pkt.packet_index = 1 - i;

    if (i == 0)
    {
      t->pkt.packed_chars[0] = 0x28;
      t->pkt.packed_chars[1] = 0x2B;
      t->pkt.packed_chars[2] = 0xFF;
      t->pkt.packed_chars[3] = 0x80;
      t->pkt.packed_chars[4] = 0x00;
    }
    else
    {
      t->pkt.packed_chars[0] = 0x83;
      t->pkt.packed_chars[1] = 0x67;
      t->pkt.packed_chars[2] = 0x6F;
      t->pkt.packed_chars[3] = 0xC3;
      t->pkt.packed_chars[4] = 0x4C;
    }

    t = t->next;
  }

  char *actual = sos_decode(pl);

  cr_assert(!strcmp(actual, "HELP HUD50N"));

  free(actual);
}

Test(hw7_sos_decode, sos_decode01)
{
  char *actual = sos_decode(NULL);
  cr_assert(actual == NULL);
}

Test(hw7_sos_decode, sos_decode02)
{
  packet_list *pl = (packet_list *)malloc(sizeof(packet_list));
  pl->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next = NULL;

  packet_list *t = pl;

  unsigned int i;
  for (i = 0; i < 2; i++)
  {
    t->pkt.id.year = 24;
    t->pkt.id.month = 5;
    t->pkt.id.day = 14;
    t->pkt.id.hour = 12;
    t->pkt.id.minute = 34;
    t->pkt.id.random_tag = 9876;
    t->pkt.magic[0] = t->pkt.magic[2] = 'S';
    t->pkt.magic[1] = 'O';
    t->pkt.n_packets = 1;
    t->pkt.packet_index = 0;

    t->pkt.packed_chars[0] = 0x80;
    t->pkt.packed_chars[1] = 0xE7;
    t->pkt.packed_chars[2] = 0x30;
    t->pkt.packed_chars[3] = 0x7F;
    t->pkt.packed_chars[4] = 0x80;

    t = t->next;
  }

  char *actual = sos_decode(pl);

  cr_assert(!strcmp(actual, "H3LL0 W0"));

  free(actual);
  free(pl->next);
  free(pl);
}

Test(hw7_sos_decode, sos_decode03)
{
  packet_list *pl = (packet_list *)malloc(sizeof(packet_list));
  pl->next = (packet_list *)malloc(sizeof(packet_list));
  pl->next->next = NULL;
  packet_list *t = pl;

  unsigned int i;
  for (i = 0; i < 2; i++)
  {
    t->pkt.id.year = 24;
    t->pkt.id.month = 5;
    t->pkt.id.day = 14;
    t->pkt.id.hour = 12;
    t->pkt.id.minute = 34;
    t->pkt.id.random_tag = 9876;
    t->pkt.magic[0] = t->pkt.magic[2] = 'S';
    t->pkt.magic[1] = 'O';
    t->pkt.n_packets = 3;
    t->pkt.packet_index = 2 - i;

    if (i == 0)
    {
      t->pkt.packed_chars[0] = 0x28;
      t->pkt.packed_chars[1] = 0x2B;
      t->pkt.packed_chars[2] = 0xFF;
      t->pkt.packed_chars[3] = 0x80;
      t->pkt.packed_chars[4] = 0x00;
    }
    else
    {
      t->pkt.packed_chars[0] = 0x83;
      t->pkt.packed_chars[1] = 0x67;
      t->pkt.packed_chars[2] = 0x6F;
      t->pkt.packed_chars[3] = 0xC3;
      t->pkt.packed_chars[4] = 0x4C;
    }

    t = t->next;
  }

  char *actual = sos_decode(pl);

  cr_assert(!strcmp(actual, "????????HELP HUD50N"));

  free(actual);
  free(pl->next);
  free(pl);
}
