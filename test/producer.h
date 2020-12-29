/*
 * producer.h
 *
 *  Created on: 05.09.2018
 *      Author: andrewerner
 */

#ifndef PRODUCER_H_
#define PRODUCER_H_

#ifndef _producer_h_
#define _producer_h_
#include "gsysc.h"
using namespace std;

SC_MODULE(producer)
{
// Ports deklarieren
	sc_in_clk prod_takt;
	sc_out<int> prod_data;
	sc_in<bool> prod_full;
	sc_out<bool> prod_data_ok;
// Funktionen deklarieren
	void produce_sth();
// Konstruktor
	SC_CTOR(producer)
	{
// gSysC: Ports umbenennen bzw. mit Namen versehen
		RENAME_PORT(&prod_takt, "P_takt");
		RENAME_PORT(&prod_data, "P_data");
		RENAME_PORT(&prod_full, "P_full");
		RENAME_PORT(&prod_data_ok, "P_data_ok");

// Port initialisieren
		prod_data_ok.initialize(false);
// Thread Process, reagiert auf Taktflanke
		SC_CTHREAD(produce_sth, prod_takt);
	}
};
#endif

#endif /* PRODUCER_H_ */
