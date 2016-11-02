/*
 * funkcje.h
 *
 *  Created on: 11 kwi 2016
 *      Author: root
 */

#ifndef FUNKCJE_H_
#define FUNKCJE_H_


void copy_eth(struct nagl_eth *nagl_eth, unsigned char *bufor);
void copy_naglowek_IPv6(struct nagl_ipv6 *nagl_ipv6, unsigned char *bufor);
void copy_naglowek_ICMP(struct nagl_icmp *nagl_icmp, unsigned char *bufor);
//void copy_data(struct data *data, unsigned char *buffer);
/////////////////////////////////////////////////////////////////////////////////
void copy_pakiet(struct pakiet *pakiet, unsigned char *bufor, int length );
void copy_pak(struct pakiet *pakiet, struct nagl_eth *nagl_eth, struct nagl_ipv6 *nagl_ipv6, struct nagl_icmp *nagl_icmp, struct dane *dane);
//tworzenie listy wiazanej
struct element_bufora* lista_wiazana(struct pakiet *pakiet, struct element_bufora *ostatni);
//
//struct element_bufora* modyfikacja(struct pakiet *pakiet1, struct element_bufora *ostatni);

void wyswietl(struct element_bufora *pakiet);
void edycja(struct element_bufora *pakiet);
int wysylanie(void *buffer, int l);
void copy_eth2(struct eth *eth1, unsigned char *bufor );
void copy2_nagl_ipv6(struct nagl_ipv6 *nagl_ipv6, unsigned char *bufor);
void copy2_icmp(struct nagl_icmp *nagl_icmp, unsigned char *bufor);
void copy_pakiet2(struct pakiet *pakiet, unsigned char *bufor);

#endif /* FUNKCJE_H_ */
