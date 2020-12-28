/*
 * fifo.h
 *
 *  Created on: 05.09.2018
 *      Author: andrewerner
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <deque>
#include <gsysc.h>

using namespace std;
SC_MODULE(fifo)
{
// Ports deklarieren
	sc_in_clk fifo_takt;
	sc_in<int> fifo_pdata;
	sc_out<int> fifo_cdata;
	sc_out<bool> fifo_full;
	sc_out<bool> fifo_empty;
	sc_in<bool> fifo_ready;
	sc_in<bool> fifo_data_ok;
// Variablen deklarieren
	int fifo_size;
	deque<int> fifo_content;
// Funktionen deklarieren
	void put_data();
	void get_data();
// Konstruktor
	SC_CTOR(fifo)
	{
// gSysC: Ports umbenennen bzw. mit Namen versehen
		RENAME_PORT(&fifo_pdata, "F_pdata");
		RENAME_PORT(&fifo_cdata, "F_cdata");
		RENAME_PORT(&fifo_full, "F_full");
		RENAME_PORT(&fifo_empty, "F_empty");
		RENAME_PORT(&fifo_ready, "F_ready");
		RENAME_PORT(&fifo_data_ok, "F_data_ok");
// Intitialisieren
		fifo_size=10;
		fifo_content.clear();
		fifo_empty.initialize(true);
		fifo_full.initialize(false);
// Method Process, reagiert auf jede Flanke
// von fifo_ready (von Consumer)
		SC_METHOD (put_data);
		sensitive_pos << fifo_ready;
// Thread Process, reagiert auf positive Flanke
// von fifo_data_ok (von Producer)
		SC_THREAD (get_data);
		sensitive_pos << fifo_data_ok;
	}
};

#endif /* FIFO_H_ */
