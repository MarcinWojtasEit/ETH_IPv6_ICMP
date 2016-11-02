/*
 * zadanie1.c
 *
 *  Created on: 12 kwi 2016
 *      Author: marcin
 */

#ifdef HAVE_CONFIG_H
	#include <config.h>
	#endif

	#include <stdio.h>
	#include <stdlib.h>

	#include <sys/socket.h>
	#include <linux/if_packet.h>
	#include <linux/if_ether.h>
	#include <linux/if_arp.h>

#include "funkcje.h"
#include "struktury.h"

struct nagl_eth eth1;
struct nagl_ipv6 ipv6;
struct nagl_icmp icmp;
struct dane dane;
//struct pakiet pakiet;
//zmienna globalna
struct element_bufora *ostatni;


int main(void){
	//definicja zmiennych dla pobierania z karty sieciowej
		int s; /*deskryptor gniazda*/
		int j;
		int i = 0;
		int length = 0;
		//przypisanie do wskaznika ostatni wartosci NULL
		ostatni=NULL;
		//bufor dla ramek z Ethernetu
		void* bufor = (void*)malloc(ETH_FRAME_LEN);
		//wskaznik do naglowka Eth
		unsigned char* etherhead = bufor;
		//wskaznik do miejsca rozpoczecia danych
		unsigned char* data = bufor + 14;
		//wskaznik do miejsca w ktorej jest informacja o protokole
		unsigned char* t = bufor + 12;
		unsigned char* t1 = bufor + 13;
		unsigned char* prot = bufor + 20;




		printf("Program do odbierania ramek Ethernet z NIC!\n");

		//otworz gniazdo
		s = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ALL));
		if (s == -1) {printf ("Nie moge otworzyc gniazda\n");}

		while (i<10) {


			//odbierz ramke Eth
			length = recvfrom(s, bufor, ETH_FRAME_LEN, 0, NULL, NULL);

			if ((*t == 0x86) & (*t1 == 0xdd)){
				if ((*prot == 0x3a)){ // sprawdzenie typu protokołu

			if (length == -1)
				printf ("Problem z odbiorem ramki \n");

			else {

				struct pakiet *pakiet;

				pakiet = malloc(sizeof(struct pakiet));
				i++;

				//kopiowanie
				copy_eth(&eth1,bufor);
				copy_naglowek_IPv6(&ipv6,bufor);
				copy_naglowek_ICMP(&icmp, bufor);

				//kopiowanie danych
				//copy_pakiet(pakiet, bufor, length);
				copy_pakiet(&dane, bufor, length);

				//printf("   %s   \n\n", &pakiet->data);
				//eth+ipv6+icmp
				copy_pak(pakiet, &eth1, &ipv6, &icmp, &dane);

				/*przypisanie do wskaznika ostatni wyniku dzialania funkcji lista_wiazana
				  argumentami funkcji jest adres struktury pakiet i wskaznik ostatni*/
				/*ostatni = lista_wiazana(&pakiet, ostatni);
				printf ("Ramka: %d, dlugosc: %d [B]\n", i, length);*/

				ostatni = lista_wiazana(pakiet, ostatni);

				//printf("   %s   \n\n", &ostatni->pakiet->data);
				//memset(pakiet->data, 0, sizeof(pakiet->data)); ???
				printf ("Ramka: %d, dlugosc: %d [B]\n", i, length);
				printf(" %s \n", pakiet->data);//dodany
			}

			#if 1
			//wypisz zawartosc bufora
				for (j=0;j<length; j++) {

					printf ("%02x ", *(etherhead+j));
				}
				printf ("\n\n\n\n");
			#endif
			}
	}
	}

		printf("Wyswietlanie zawartość list wiązanych \n");
		//jest to wywolanie funkcji do wyswietlania ktora jako argument przyjmuje odwolanie do
		//okreslonej sladowej structury
		//ostatni jest wskaznikiem globalnym do obiektu ttypu element_bufora
		wyswietl(ostatni->pierwszy);
		//jest wywolanie funkcji ktora zmienia addres mac destination
		//ostatni jest wskaznikiem globalnym do obiektu ttypu element_bufora
		edycja(ostatni->pierwszy);
		printf("Wyswietlanie zawartość list wiązanych po edycji \n");
		//wywolanie funkcji do wyswietlania
		wyswietl(ostatni->pierwszy);
///////////////////////////////////////////////////////

		printf(" \n\n\n\n");
		struct element_bufora *tmp;
        int l;
		for (tmp = ostatni->pierwszy; tmp!= NULL; tmp = tmp->nastepny){
			memset(bufor, 0, sizeof(bufor));
			l=0;
			l=tmp->pakiet->ipv6a.lenght+14;

			copy_eth2(&tmp->pakiet->etha, bufor);
			copy2_nagl_ipv6(&tmp->pakiet->ipv6a, bufor);
			copy2_icmp(&tmp->pakiet->icmpa, bufor);
			copy_pakiet2(&tmp->pakiet->data, bufor);

			unsigned char* etherhead = bufor;
/*
			for(int i=0;i<45;i++){
				printf ("%02x ", *(etherhead+i));
			}
			printf("\n\n\n"); */
			wysylanie(bufor, l);
		};
/*
	//przypisanie do wskaznika ostatni wartosci NULL
	ostatni=NULL;
	int licz=0;
	for(licz;licz<10;licz++){
	ostatni=modyfikacja(&pakiet, ostatni);
	}
*/
		return 0;
}



