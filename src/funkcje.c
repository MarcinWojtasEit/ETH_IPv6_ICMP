/*
 * funkcje.c
 *
 *  Created on: 11 kwi 2016
 *      Author: root
 */

#include <stdio.h>
#include "struktury.h"
#include <unistd.h>
#include <string.h>
/////////////////////////////////////
#include <stdlib.h>

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <sys/ioctl.h>

#define INTERFACE	"eth0"
//#define INTERFACE	"eno16777736"

///////////////////////////////////////
void copy_eth(struct nagl_eth *nagl_eth, unsigned char *bufor)
{
	short int b;

	memcpy (nagl_eth-> source, bufor, 6);
	memcpy (nagl_eth->destination, bufor+6, 6);
	memcpy (&b, bufor+12, 2);
	nagl_eth->type=((b>>8)&0xff) | ((b<<8)&0xff00);
 };

void copy_naglowek_IPv6(struct nagl_ipv6 *nagl_ipv6, unsigned char *bufor)
{

	    unsigned int a;
	    unsigned short le;


	    memcpy (&a, bufor+14, 4);
	    nagl_ipv6-> version=a >> 4;
	    //nagl_ipv6-> priorytet=a>>8;
	    //nagl_ipv6-> etykieta=a>>20;
	    nagl_ipv6-> priorytet=(a>>12 & 0x000f) | (a<<4 & 0x00f0);
	    nagl_ipv6-> etykieta=((a<<8 & 0x0f0000) | (a>>8 & 0x00f00) | (a>>24 & 0x0000ff));

		memcpy (&le, bufor+18, 2);
		nagl_ipv6->lenght=((le>>8)&0xff) | ((le<<8)&0xff00);

		memcpy (&nagl_ipv6-> nextheader, bufor+20, 1);
		memcpy (&nagl_ipv6-> limit, bufor+21, 1);

	    memcpy (nagl_ipv6->sourceipv6, bufor+22, 16);
	    memcpy (nagl_ipv6->destinationipv6, bufor+38, 16);

};


void copy_naglowek_ICMP(struct nagl_icmp *nagl_icmp, unsigned char *bufor)
{

	unsigned short e;
	memcpy (&nagl_icmp-> icmp_type, bufor+54, 1);
	memcpy (&nagl_icmp-> icmp_code, bufor+55, 1);
	memcpy (&e, bufor+56, 2);
		     nagl_icmp->icmp_checksum=((e>>8)&0xff) | ((e<<8)&0xff00);


};
/*
void copy_data(struct data *data, unsigned char *buffer)
{
	memcpy (data-> pdata, buffer+58, 60);
};
*/
/////////////////////////////////////////////////
void copy_pakiet(struct pakiet *pakiet, unsigned char *bufor, int length){
	int l=length-58;
	memcpy(&pakiet->data, bufor+58,l);
};
//argumenty funkcji sa structurami z def zmiennych w postaci wskaznikow
void copy_pak(struct pakiet *pakiet, struct nagl_eth *nagl_eth, struct nagl_ipv6 *nagl_ipv6, struct nagl_icmp *nagl_icmp, struct dane *dane){

	//pakiet jest wskaznikiem do struktury, ktora jest rowna wskaznikowi
	pakiet->etha = *nagl_eth;
	pakiet->ipv6a = *nagl_ipv6;
	pakiet->icmpa = *nagl_icmp;
	pakiet->data =*dane;

};
//Jest to funkcja ktora zwraca wskaznik(lista_wiazana) do struktury(element_bufora)
//Pakujemy structure do listy wiazanej
struct element_bufora* lista_wiazana(struct pakiet *pakiet1, struct element_bufora *ostatni){

	//sprawdzanie czy jest to pierwszy element listy
		if(ostatni==NULL){
		//Funkcja służy do dynamicznego rezerwowania miejsca w pamięci. Gdy funkcja zostanie wywołana,
	    //w przypadku sukcesu zwróci wskaźnik do nowo zarezerwowanego miejsca w pamięci;
	    // w przypadku błędu zwraca wartość NULL

		//zwraca wskaźnik do początku zarezerwowanego obszaru jako wskaźnik typu void*.
	    //void *malloc(size_t size);


		//zwraca wskaznik typu structura element_bufura
		ostatni = (struct element_bufora*) malloc (sizeof(struct element_bufora));

		//ostatni jest wskaznikiem do struktury  i odwolujemy sie do jej skladowej
		//do skaldowej pakiet przepisujemy adres nowego pakietu
		ostatni->pakiet = pakiet1;
		//do skladowej nastepny wartpsc NULL przypisujemy
		ostatni->nastepny=NULL;
		//do skladowej poprzedni wartpsc NULL przypisujemy bo nie byl wczesniej zadnego pakietu
		ostatni->poprzedni=NULL;
		ostatni->pierwszy=ostatni;
		//zwracany jest wskanik do struktury
		//printf("marcinmaraicnmarcin");
		return ostatni;
		}
		//jesli nie jest to pierwszy element
		else{

			//tworzymy wskaznik na strukture element_bufora
			struct element_bufora *nowy_element;
			//rezerwacja miejsca w pamieci
			nowy_element = (struct element_bufora*) malloc (sizeof(struct element_bufora));
			//do skladowej poprzedni przypisanie adresu wczesniejszego element
			nowy_element -> poprzedni = ostatni ;
			//
			nowy_element -> pierwszy = ostatni->pierwszy;
			//do skladowej nastepny wartpsc NULL przypisujemy
			nowy_element -> nastepny = NULL;
			//do skladowej pakiet przypisanie adresu nowego pakietu
			nowy_element->pakiet=pakiet1;
			/* do wskaznika ostatni kopiujemy zawartoscc wskaznika nowy_element. W efekcie ostatni
			   wskazuje ten sam obiekt co nowy_element*/
			ostatni->nastepny = nowy_element;
			//
			//printf(&nowy_element);
			return nowy_element;
		}

};

//funkcja nie zwraca nic a przyjumuje jako argument wskazniki do obiektu
//typu element_bufora ktory jest structura(lista wiazana)
void wyswietl(struct element_bufora *pakiet){
	//tworzymy wskaznik do obiektu typu element_bufora ktory jest structura(lista wiazana)
	struct element_bufora *tmp;
		//int i = 0;
//do wskaznikia temp kopiujemy wartosc pierwszy pobrany z structury na ktora wskazuje wskaznik pakiet
//sprawdzamy czy temp nie jest rozny od NULL
//do wskaznika temp przypisujemy wartsoc nastepny
		for (tmp = pakiet->pierwszy; tmp!= NULL; tmp = tmp->nastepny){
			//wyswietla adres elementu w pamieci
			printf("Adres elementu %p\n", tmp);

			//drukujemy adres docelowy
			printf("   Adres docelowy:     %.2x:%.2x:%.2x:%.2x:%.2x:%.2x \n",
					tmp->pakiet->etha.destination[0],
					tmp->pakiet->etha.destination[1],
					tmp->pakiet->etha.destination[2],
					tmp->pakiet->etha.destination[3],
					tmp->pakiet->etha.destination[4],
					tmp->pakiet->etha.destination[5]);
			//printf("version: %x", tmp->pakiet->ipv6a.version);
		}


	};
//funkcja do zmiany adresu MAC destination
void edycja(struct element_bufora *pakiet) {
	struct element_bufora *tmp;
	//	int i=0;
		puts("Zmiana adresu mac(destination)");
		for (tmp = pakiet->pierwszy; tmp!= NULL; tmp = tmp->nastepny){
			tmp->pakiet->etha.destination[0] =0x00 ;
			tmp->pakiet->etha.destination[1] =0x00 ;
			tmp->pakiet->etha.destination[2] =0x00 ;
			tmp->pakiet->etha.destination[3] =0x00 ;
			tmp->pakiet->etha.destination[4] =0x00 ;
			tmp->pakiet->etha.destination[5] =0x00 ;
		}
};


void copy_eth2(struct nagl_eth *eth1, unsigned char *bufor ){
	short int a;
	memcpy(bufor,eth1->destination,6);
	memcpy(bufor+6,eth1->source,6);
	memcpy(&a,&eth1->type,2);
	*(bufor+12) = ((a>>8)&0xff) | ((a<<8)&0xff00);
};

void copy2_nagl_ipv6(struct nagl_ipv6 *nagl_ipv6, unsigned char *bufor)
{

	    unsigned int a,a1,a2;
	    unsigned short le;

	    a=nagl_ipv6-> version;
	 	    //nagl_ipv6-> priorytet=a>>8;
	 	    //nagl_ipv6-> etykieta=a>>20;
	 	//a1=(a>>12 & 0x000f) | (a<<4 & 0x00f0) | nagl_ipv6-> priorytet;
	 	//a2=((a<<8 & 0x0f0000) | (a>>8 & 0x00f00) | (a>>24 & 0x0000ff)) | nagl_ipv6-> etykieta;

	    a1=a<<4 | nagl_ipv6->priorytet;
	    a1=(a>>12 & 0x000f) | (a<<4 & 0x00f0);
        a2=a<<12 | nagl_ipv6->etykieta;
	    a2=((a<<8 & 0x0f0000) | (a>>8 & 0x00f00) | (a>>24 & 0x0000ff));

	 	//nagl_ipv6-> etykieta=((a<<8 & 0x0f0000) | (a>>8 & 0x00f00) | (a>>24 & 0x0000ff));
	    memcpy ( bufor+14, &a2,4);

	    ///////////////////////////////////////////////////

	    //to dobrze
	    le=nagl_ipv6->lenght;
	    le=((le>>8)&0xff) | ((le<<8)&0xff00);
		memcpy ( bufor+18, &le,2);


		memcpy (bufor+20,&nagl_ipv6-> nextheader, 1);
		memcpy (bufor+21,&nagl_ipv6-> limit, 1);
	    memcpy (bufor+22, nagl_ipv6->sourceipv6,16);
	    memcpy (bufor+38, nagl_ipv6->destinationipv6,16);

};

//zmieniona
void copy2_icmp(struct nagl_icmp *nagl_icmp, unsigned char *bufor)
{

	unsigned short e;
	memcpy (bufor+54, &nagl_icmp-> icmp_type, 1);
	memcpy (bufor+55, &nagl_icmp-> icmp_code, 1);
	e=nagl_icmp->icmp_checksum;
	e=((e>>8)&0xff) | ((e<<8)&0xff00);
	memcpy (bufor+56, &e, 2);



};

void copy_pakiet2(struct dane *dane, unsigned char *bufor){
	int l=sizeof(dane->data);

	memcpy(bufor+58,dane->data, l);
};



int wysylanie(void *buffer, int l) {
	puts("Wysłanie ramki przez NIC");

	//definicja zmiennych
	int s_out; /*deskryptor gniazda*/
	int j;

	//bufor dla ramek z Ethernetu
	//void* buffer = (void*)malloc(ETH_FRAME_LEN);  moj koment
	//wskaxnik do naglowka Eth
	unsigned char* etherhead = buffer;
	//wskaznik do miejsca rozpoczecia danych
	unsigned char* data = buffer + 14;

	//inny wskaznik do naglowka Eth
	struct ethhdr *eh = (struct ethhdr *)etherhead;
	//adres docelowy
	struct sockaddr_ll socket_address;
	int send_result = 0;
	struct ifreq if_idx;
	int ifindex = 0;
	socket_address.sll_halen    = ETH_ALEN;

#if 1 //tu mozna zablokowac wysylanie
		s_out = socket(AF_PACKET, SOCK_RAW, ETH_P_ALL);
		if (s_out == -1) {printf ("Nie moge otworzyc gniazda s_out\n");}

		memset(&if_idx, 0, sizeof(struct ifreq));
		strncpy(if_idx.ifr_name, "eth0", IFNAMSIZ-1);
		if (ioctl(s_out, SIOCGIFINDEX, &if_idx) < 0){
		    perror("SIOCGIFINDEX");
	        exit(1);
	    }
	    ifindex = if_idx.ifr_ifindex;
	    printf("Pobrano indeks karty NIC: %i\n", ifindex);
	    //usatwiono index urzadzenia siecowego
	    socket_address.sll_ifindex  = ifindex;

		send_result = sendto(s_out, buffer, l, 0,(struct sockaddr*)&socket_address, sizeof(socket_address));
		if (send_result == -1) { printf ("Nie moge wyslac danych! \n"); } else { printf ("Wyslalem dane do intefejsu: %s \n", INTERFACE);}

		//=======wypisz zawartosc bufora do wyslania===========
		#if 1
			printf ("Dane do wyslania: \n");
			for (j=0;j<send_result; j++) {
				printf ("%02x ", *(etherhead+j));
			}
			printf ("\n");
		#endif
		//========koniec wypisywania===========================

	#endif //konic blokady wysylania
	//*******************************************************************************

	return EXIT_SUCCESS;
}




















