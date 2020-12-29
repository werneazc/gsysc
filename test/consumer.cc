/*
 * consumer.cc
 *
 *  Created on: 05.09.2018
 *      Author: andrewerner
 */

#include "consumer.h"

void consumer::consume_it()
{
	int data_read;
	float rand_val = 0.0;
	while (true)
	{
		while (cons_empty || cons_takt == false)
		{
			wait();
		}
		rand_val = rand();
		if (rand_val / RAND_MAX >= 0.5)
		{
// Anfrage stellen an FIFO (bei positiver Takt-Flanke)
			cons_ready = true;
			wait();
// erhaltenes Paket von Port einlesen (bei negativer Takt-Flanke)
			if (cons_ready)
			{
				cons_ready = false;
				data_read = cons_data;
				cout << "Consumer hat Paket " << data_read
						<< " von FIFO erhalten,";
				cout << " FIFO ist leer:" << cons_empty << "." << endl;
			}
			else
			{
				cout << "FEHLER: obwohl cons_ready=1 sein muesste, ";
				cout << " ist es auf false gesetzt!!!" << endl;
			}
			wait();
		}
		else
		{
			wait();
		}
	}
}

