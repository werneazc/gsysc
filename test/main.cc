#include "consumer.h"
#include "fifo.h"
#include"producer.h"

using namespace std;

int sc_main(int argc, char* argv[])
{
// Signale deklarieren
	sc_signal<int> main_prod_data("prod_data");
	sc_signal<int> main_cons_data("cons_data");
	sc_signal<bool> main_data_ok("data_ok");
	sc_signal<bool> main_ready("ready");
	sc_signal<bool> main_full("full");
	sc_signal<bool> main_empty("empty");

// Clock erzeugen und setzen
	sc_clock theClock("Clock", 1);
// Instanzen erschaffen
	producer main_prod("producer");
	fifo main_fifo1("FIFO");
	consumer main_cons("consumer");
// Signale verbinden
	main_prod.prod_data(main_prod_data);
	main_prod.prod_data_ok(main_data_ok);
	main_prod.prod_full(main_full);
	main_fifo1.fifo_cdata(main_cons_data);
	main_fifo1.fifo_pdata(main_prod_data);
	main_fifo1.fifo_full(main_full);
	main_fifo1.fifo_empty(main_empty);
	main_fifo1.fifo_ready(main_ready);
	main_fifo1.fifo_data_ok(main_data_ok);
	main_cons.cons_data(main_cons_data);
	main_cons.cons_ready(main_ready);
	main_cons.cons_empty(main_empty);
	main_prod.prod_takt(theClock);
	main_cons.cons_takt(theClock);
	main_fifo1.fifo_takt(theClock);
// gSysC: Module und Ports registrieren
	REG_MODULE(&main_fifo1, "FIFO", 0);
	REG_MODULE(&main_prod, "Producer", 0);
	REG_MODULE(&main_cons, "Consumer", 0);
	REG_IN_PORT(&main_prod.prod_data, &main_prod, &main_prod_data);
	REG_OUT_PORT(&main_prod.prod_data_ok, &main_prod, &main_data_ok);
	REG_IN_PORT(&main_prod.prod_full, &main_prod, &main_full);
	REG_OUT_PORT(&main_fifo1.fifo_cdata, &main_fifo1, &main_cons_data);
	REG_IN_PORT(&main_fifo1.fifo_pdata, &main_fifo1, &main_prod_data);
	REG_OUT_PORT(&main_fifo1.fifo_full, &main_fifo1, &main_full);
	REG_OUT_PORT(&main_fifo1.fifo_empty, &main_fifo1, &main_empty);
	REG_IN_PORT(&main_fifo1.fifo_ready, &main_fifo1, &main_ready);
	REG_IN_PORT(&main_fifo1.fifo_data_ok, &main_fifo1, &main_data_ok);
	REG_IN_PORT(&main_cons.cons_data, &main_cons, &main_cons_data);
	REG_OUT_PORT(&main_cons.cons_ready, &main_cons, &main_ready);
	REG_IN_PORT(&main_cons.cons_empty, &main_cons, &main_empty);
// gSysC-Initialiserung: Puffervariable als ProgressBar darstellen
	gsys_addBuffer((void* ) 534, "FIFO");\

// Trace-File erzeugen lassen
	sc_trace_file* tf = sc_create_vcd_trace_file("fifo_bsp");
	sc_trace(tf, main_prod_data, "prod_data");
	sc_trace(tf, main_cons_data, "cons_data");
	sc_trace(tf, main_data_ok, "data_ok");
	sc_trace(tf, main_ready, "ready");
	sc_trace(tf, main_full, "full");
	sc_trace(tf, main_empty, "empty");

	sc_start(1000);
	return 0;
}
