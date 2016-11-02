/*
 * struktury.h
 *
 *  Created on: 11 kwi 2016
 *      Author: root
 */

#ifndef STRUKTURY_H_
#define STRUKTURY_H_

#define ETHERNET_SOURCE 6
#define ETHERNET_DESTINATION 6

#define IPV6_SOURCEIPV6 16
#define IPV6_DESTINATIONIPV6 16


#pragma pack(1) // włączenie pakowania

struct nagl_eth {

	unsigned char source[ETHERNET_SOURCE];
	unsigned char destination[ETHERNET_DESTINATION];
	unsigned short int type;

};

struct nagl_ipv6
{
unsigned int version:4,
             priorytet:4,
			 etykieta:20;

unsigned short lenght;
unsigned char limit;

unsigned char nextheader;
unsigned char sourceipv6[IPV6_SOURCEIPV6];
unsigned char destinationipv6[IPV6_DESTINATIONIPV6];

};


struct nagl_icmp
{
	unsigned char icmp_type;
	unsigned char icmp_code;
	unsigned short icmp_checksum;

};

struct dane
{
	unsigned char data[1472];
};


//structura laczaca wszytko w jedno
struct pakiet{
	struct nagl_eth etha;
	struct nagl_ipv6 ipv6a;
	struct nagl_icmp icmpa;
	//unsigned char data[1472]; // inna wartosc chyba
	struct dane data;


};
// structura do tworzenia list wiazanej
struct element_bufora{
		struct element_bufora *nastepny;
		struct element_bufora *poprzedni;
		struct element_bufora *pierwszy;
		struct pakiet *pakiet;
	};


#pragma pack() // wyłączenie pakowania
#endif /* STRUKTURY_H_ */
