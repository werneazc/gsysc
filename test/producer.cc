#include "producer.h"

void producer::produce_sth()
{
	int data_packet = 0;
	float rand_val = 0.0;
	while (true)
	{
		while (prod_full || prod_takt == false)
		{
			wait();
		}
		rand_val = rand();
		if (rand_val / RAND_MAX >= 0.5)
		{
			// Paket zu erzeugen
// Datenpaket generieren (Zufallswert)
			data_packet = rand();
// Datenpaket senden (bei positiver Takt-Flanke)
			prod_data = data_packet;
			cout << "Producer hat Paket " << data_packet;
			cout << " an FIFO geschickt." << endl;
			prod_data_ok = true;
			wait();
			prod_data_ok = false; // Signal zur"ucksetzen bei neg. Takt-Flanke
			wait();
		}
		else
		{
			wait();
		}
	}
}
