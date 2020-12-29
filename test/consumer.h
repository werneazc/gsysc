/*
 * consumer.h
 *
 *  Created on: 05.09.2018
 *      Author: andrewerner
 */

#ifndef CONSUMER_H_
#define CONSUMER_H_

#include "gsysc.h"
using namespace std;
SC_MODULE(consumer)
{
// Ports deklarieren
	sc_in_clk cons_takt;
	sc_in<int> cons_data;
	sc_out<bool> cons_ready;
	sc_in<bool> cons_empty;
// Variablen deklarieren
	int data_packet;
// Funktionen deklarieren
	void consume_it();
// Konstruktor
	SC_CTOR(consumer)
	{
// gSysC: Ports umbenennen bzw. mit Namen versehen
		RENAME_PORT(&cons_takt, "C_takt");
		RENAME_PORT(&cons_data, "C_data");
		RENAME_PORT(&cons_ready, "C_ready");
		RENAME_PORT(&cons_empty, "C_empty");
// Port initialisieren
		cons_ready.initialize(false);

// Thread Process, reagiert auf Taktflanke
		SC_CTHREAD(consume_it, cons_takt);
	}
};

#endif /* CONSUMER_H_ */
