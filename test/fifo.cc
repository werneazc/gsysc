/*
 * fifo.cc
 *
 *  Created on: 05.09.2018
 *      Author: andrewerner
 */

#include "fifo.h"

void fifo::get_data()
{
	wait();
	// Warten, um Fehler durch Anfangszustand zu vermeiden
	while (true)
	{
		if (fifo_content.size() < fifo_size)
		{
			fifo_content.push_back(fifo_pdata);
			cout << "\tPaket in FIFO aufgenommen: " << fifo_content.back()
					<< endl;
			cout << "\t\taktueller FIFO-Fullstand (nach push): ";
			cout << fifo_content.size() << endl;
		}
		fifo_full = (fifo_content.size() >= fifo_size);
		fifo_empty = (fifo_content.empty());
// gSysC: Aktualisieren der Laufleisten-Anzeige f"ur FIFO-F"ullstand
		gsys_refreshBuffer((void* ) 534, fifo_content.size(), fifo_size);
		wait();
	}
}
void fifo::put_data()
{
	if (fifo_content.size() > 0)
	{
		fifo_cdata = fifo_content.front();
		cout << "\tPaket auf Datenport gelegt: " << fifo_content.front()
				<< endl;
		fifo_content.pop_front();
		cout << "\t\taktueller FIFO-Fullstand (nach pop): "
				<< fifo_content.size() << endl;
	}
	fifo_full = !(fifo_content.size() < fifo_size);
	fifo_empty = (fifo_content.empty());
// gSysC: Aktualisieren der Laufleisten-Anzeige f"ur FIFO-F"ullstand
	gsys_refreshBuffer((void* ) 534, fifo_content.size(), fifo_size);
}
