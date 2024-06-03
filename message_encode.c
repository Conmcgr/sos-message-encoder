#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include "message_encode.h"

char *trim(char *s)
{
  int start_index = 0;
  int end_index = strlen(s) - 1;
  while (s[start_index] == ' ' && s[start_index] != '\0')
  {
    start_index++;
  }

  while (end_index >= start_index && s[end_index] == ' ')
  {
    end_index--;
  }

  char *result = malloc(end_index - start_index + 2);
  if (result == NULL)
  {
    return NULL;
  }

  for (int i = start_index; i <= end_index; i++)
  {
    result[i - start_index] = s[i];
  }

  result[end_index - start_index + 1] = '\0';
  return result;
}

char *sos_translate(char *s)
{
  int len = strlen(s);
  int current = 0;

  char *result = malloc(len + 1);
  if (result == NULL)
  {
    fprintf(stderr, "Heap Allocation Failed");
    exit(1);
  }

  for (int i = 0; i < len; i++)
  {
    switch (s[i])
    {
    case 'B':
    case 'b':
      result[current++] = '8';
      break;
    case 'I':
    case 'i':
      result[current++] = '1';
      break;
    case 'O':
    case 'o':
      result[current++] = '0';
      break;
    case 'S':
    case 's':
      result[current++] = '5';
      break;
    case 'Z':
    case 'z':
      result[current++] = '2';
      break;
    default:
      if ('a' <= s[i] && s[i] <= 'z')
      {
        result[current++] = s[i] - 32;
      }
      else if (('A' <= s[i] && s[i] <= 'Z') || ('0' <= s[i] && s[i] <= '9') || s[i] == ' ')
      {
        result[current++] = s[i];
      }
      break;
    }
  }
  result[current] = '\0';
  return result;
}

char *squeeze_spaces(char *s)
{
  int len = strlen(s);
  char *result = malloc(len + 1);
  if (result == NULL)
  {
    fprintf(stderr, "Heap Allocation Failed");
    exit(1);
  }

  int current = 0;
  int i = 0;

  while (s[i] != '\0')
  {
    if (s[i] != ' ')
    {
      result[current++] = s[i];
    }
    else if (current > 0 && result[current - 1] != ' ')
    {
      result[current++] = s[i];
    }
    i++;
  }

  result[current] = '\0';
  return result;
}

char *sos_terminate(char *s)
{
  int len = strlen(s);
  char *result = malloc(len + 3);

  if (result == NULL)
  {
    fprintf(stderr, "Heap Allocation Failed");
    exit(1);
  }

  for (int i = 0; i < len; i++)
  {
    result[i] = s[i];
  }

  result[len] = ' ';
  result[len + 1] = ' ';
  result[len + 2] = '\0';

  return result;
}

uint8_t *ascii_to_sos(char *s)
{
  int len = strlen(s);
  uint8_t *result = malloc(len * sizeof(uint8_t));
  if (result == NULL)
  {
    fprintf(stderr, "Heap Allocation Failed");
    exit(1);
  }
  for (int i = 0; i < len; i++)
  {
    if (s[i] == 'A')
    {
      result[i] = 10;
    }
    else if (s[i] == ' ')
    {
      result[i] = 31;
    }
    else if ('0' <= s[i] && s[i] <= '9')
    {
      result[i] = s[i] - 48;
    }
    else if ('C' <= s[i] && s[i] <= 'H')
    {
      result[i] = s[i] - 56;
    }
    else if ('J' <= s[i] && s[i] <= 'N')
    {
      result[i] = s[i] - 57;
    }
    else if ('P' <= s[i] && s[i] <= 'R')
    {
      result[i] = s[i] - 58;
    }
    else if ('T' <= s[i] && s[i] <= 'Y')
    {
      result[i] = s[i] - 59;
    }
  }

  return result;
}

uint8_t *sos_pack(uint8_t *is, int len, int *outlen)
{
  *outlen = (int)ceil(len * 5.0 / 8.0);
  uint8_t *result = malloc(sizeof(uint8_t) * *outlen);
  if (result == NULL)
  {
    fprintf(stderr, "Heap Allocation Failed");
    exit(1);
  }

  int current_pos = 0;
  int current_row = 0;
  result[current_row] = 0;

  for (int i = 0; i < len; i++)
  {
    for (int j = 4; j >= 0; j--)
    {
      unsigned char bit = (is[i] >> j) & 1;
      unsigned char bit_val = (bit << (7 - current_pos));
      result[current_row] = result[current_row] | bit_val;
      current_pos++;
      if (current_pos >= 8)
      {
        current_row++;
        current_pos = 0;
        result[current_row] = 0;
      }
    }
  }

  return result;
}

uint8_t *sos_encode(char *message, int *outlen)
{

  char *translated = sos_translate(message);
  char *trimmed = trim(translated);
  free(translated);
  char *squeezed = squeeze_spaces(trimmed);
  free(trimmed);
  char *terminated = sos_terminate(squeezed);
  free(squeezed);
  uint8_t *ascii_values = ascii_to_sos(terminated);
  unsigned char *packed = sos_pack(ascii_values, strlen(terminated), outlen);

  return packed;
}

packet_list *pl_cons(packet pkt, packet_list *next)
{
  packet_list *pl = malloc(sizeof(packet_list));
  if (!pl)
  {
    fprintf(stderr, "Heap Allocation Failed\n");
    exit(1);
  }
  pl->pkt = pkt;
  pl->next = next;
  return pl;
}

void pl_free(packet_list *pl)
{
  while (pl)
  {
    packet_list *next = pl->next;
    free(pl);
    pl = next;
  }
}

packet_list *sos_packetize(uint8_t *packed_message, int len)
{

  if (!packed_message)
  {
    return NULL;
  }

  time_t tt = time(NULL);
  struct tm *tm = gmtime(&tt);
  srandom(time(NULL));
  message_id msg_id = {tm->tm_year - 100, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, random()};
  packet_list *pack_list = NULL;
  int num_packets = (int)ceil(len / 5.0);
  int pos = 0;

  for (int i = 0; i < num_packets; i++)
  {
    packet pkt = {"SOS", msg_id, num_packets, i, {0}};
    for (int j = 0; j < 5; j++)
    {
      if (pos < len)
      {
        pkt.packed_chars[j] = packed_message[pos];
      }
      else
      {
        pkt.packed_chars[j] = 0;
      }
      pos++;
    }
    pack_list = pl_cons(pkt, pack_list);
  }

  return pack_list;
}

packet_list *append_packet(packet_list *list, packet pkt)
{
  packet_list *node = (packet_list *)malloc(sizeof(packet_list));
  if (!node)
  {
    fprintf(stderr, "Heap Allocation Failed\n");
    exit(0);
  }
  node->pkt = pkt;
  node->next = NULL;

  if (list == NULL)
  {
    return node;
  }
  else
  {
    packet_list *temp = list;
    while (temp->next != NULL)
    {
      temp = temp->next;
    }
    temp->next = node;
    return list;
  }
}

packet_list *filter_packets(packet_list *packets, message_id id)
{
  packet_list *result = NULL;
  while (packets)
  {
    if (packets->pkt.id.year == id.year && packets->pkt.id.month == id.month &&
        packets->pkt.id.day == id.day && packets->pkt.id.hour == id.hour &&
        packets->pkt.id.minute == id.minute && packets->pkt.id.random_tag == id.random_tag)
    {
      result = append_packet(result, packets->pkt);
    }
    packets = packets->next;
  }
  return result;
}

packet_list *filterLT(u_int8_t id, packet_list *pl)
{
  if (!pl)
  {
    return NULL;
  }
  else if (pl->pkt.packet_index < id)
  {
    return pl_cons(pl->pkt, filterLT(id, pl->next));
  }
  else
  {
    return filterLT(id, pl->next);
  }
}

packet_list *filterGE(u_int8_t id, packet_list *pl)
{
  if (!pl)
  {
    return NULL;
  }
  else if (pl->pkt.packet_index >= id)
  {
    return pl_cons(pl->pkt, filterGE(id, pl->next));
  }
  else
  {
    return filterGE(id, pl->next);
  }
}

packet_list *sort_packets(packet_list *packets)
{
  if (!packets)
  {
    return NULL;
  }
  packet pivot = packets->pkt;
  packet_list *lt = filterLT(pivot.packet_index, packets->next);
  packet_list *ge = filterGE(pivot.packet_index, packets->next);
  packet_list *qlt = sort_packets(lt);
  packet_list *qge = sort_packets(ge);
  packet_list *pivot_qge = pl_cons(pivot, qge);
  packet_list *temp = qlt;

  if (!qlt)
  {
    return pivot_qge;
  }

  while (temp->next != NULL)
  {
    temp = temp->next;
  }
  temp->next = pivot_qge;
  return qlt;
}

uint8_t *sos_unpack(packet pkt)
{
  uint8_t *unpacked = malloc(sizeof(uint8_t) * 8);
  if (!unpacked)
  {
    fprintf(stderr, "Heap Allocation Failed\n");
    exit(0);
  }
  int current_packed = 0;
  int current_bit_pos = 0;
  for (int i = 0; i < 8; i++)
  {
    unpacked[i] = 0;
    for (int j = 0; j < 5; j++)
    {
      uint8_t bit_val = (pkt.packed_chars[current_packed] >> (7 - current_bit_pos)) & 1;
      unpacked[i] = (unpacked[i] << 1) | bit_val;
      current_bit_pos++;
      if (current_bit_pos == 8)
      {
        current_bit_pos = 0;
        current_packed++;
      }
    }
  }
  return unpacked;
}

char *sos_to_ascii(uint8_t *sos, int len)
{
  if (len == 0)
  {
    return NULL;
  }

  char *result = malloc(len * sizeof(char));
  if (!result)
  {
    fprintf(stderr, "Heap Allocation Failed\n");
    exit(0);
  }

  for (int i = 0; i < len; i++)
  {
    if (sos[i] >= 32)
    {
      fprintf(stderr, "Invalid SOS");
      exit(0);
    }
    else if (sos[i] == 10)
    {
      result[i] = 'A';
    }
    else if (sos[i] == 31)
    {
      result[i] = ' ';
    }
    else if (0 <= sos[i] && sos[i] <= 9)
    {
      result[i] = sos[i] + 48;
    }
    else if (11 <= sos[i] && sos[i] <= 16)
    {
      result[i] = sos[i] + 56;
    }
    else if (17 <= sos[i] && sos[i] <= 21)
    {
      result[i] = sos[i] + 57;
    }
    else if (22 <= sos[i] && sos[i] <= 24)
    {
      result[i] = sos[i] + 58;
    }
    else if (25 <= sos[i] && sos[i] <= 30)
    {
      result[i] = sos[i] + 59;
    }
  }

  return result;
}

char *sos_decode(packet_list *packets)
{
  if (!packets)
  {
    return NULL;
  }

  int total_packets = packets->pkt.n_packets;
  int len = total_packets * 8;
  char *result = (char *)malloc(sizeof(char) * (len + 1));
  if (!result)
  {
    fprintf(stderr, "Heap Allocation Failed\n");
    exit(1);
  }

  for (int i = 0; i < len; i++)
  {
    result[i] = '?';
  }
  result[len] = '\0';

  packet_list *sorted_packets = sort_packets(packets);
  int expected_index = 0;

  while (sorted_packets)
  {
    if (sorted_packets->pkt.packet_index == expected_index)
    {
      uint8_t *unpacked = sos_unpack(sorted_packets->pkt);
      char *ascii = sos_to_ascii(unpacked, 8);

      for (int i = 0; i < 8; i++)
      {
        result[expected_index * 8 + i] = ascii[i];
      }

      free(unpacked);
      free(ascii);

      expected_index++;
    }
    else if (sorted_packets->pkt.packet_index > expected_index)
    {
      expected_index++;
      continue;
    }
    sorted_packets = sorted_packets->next;
  }

  for (int i = 0; i < len - 1; i++)
  {
    if (result[i] == ' ' && result[i + 1] == ' ')
    {
      result[i] = '\0';
      break;
    }
  }

  return result;
}

uint8_t hex_char_to_byte(char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'A' && c <= 'F')
    return 10 + c - 'A';
  if (c >= 'a' && c <= 'f')
    return 10 + c - 'a';
  return 0;
}

uint8_t hex_to_byte(char *hex)
{
  return hex_char_to_byte(hex[0]) * 16 + hex_char_to_byte(hex[1]);
}

packet_list *build_packet_list_from_input(char *input)
{
  packet_list *head = NULL, *current = NULL;
  int input_length = strlen(input);
  int byte_count = input_length / 2; // Since 2 hex digits = 1 byte

  for (int i = 0; i < byte_count; i += 5)
  {
    packet new_pkt;
    int j;
    for (j = 0; j < 5 && (i + j) < byte_count; j++)
    {
      char hex_byte[3] = {input[2 * (i + j)], input[2 * (i + j) + 1], '\0'};
      new_pkt.packed_chars[j] = hex_to_byte(hex_byte);
    }
    // Fill the rest of the packet with zeros if necessary
    for (; j < 5; j++)
    {
      new_pkt.packed_chars[j] = 0;
    }

    // Assume packet_index or other relevant metadata is set here
    new_pkt.packet_index = i / 5;

    // Create a new packet list node
    packet_list *new_node = malloc(sizeof(packet_list));
    if (!new_node)
    {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
    }
    new_node->pkt = new_pkt;
    new_node->next = NULL;

    if (!head)
    {
      head = new_node;
      current = new_node;
    }
    else
    {
      current->next = new_node;
      current = new_node;
    }
  }

  return head;
}