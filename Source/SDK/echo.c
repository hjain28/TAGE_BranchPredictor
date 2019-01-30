

#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"
#if defined (__arm__) || defined (__aarch64__)
#include "xil_printf.h"
#endif

//User Edit
#include "xparameters.h"
#include "xgpiops.h"
#include "xil_io.h"

unsigned char k1,k2,k3,k4,k5,k6,k7,k8;
unsigned int k;

int Data1,Data2,Data3,Data4;
int Corrected_Pred, total_BR;

int i;
int addr=0;

static XGpioPs pin;

int transfer_data() {
	return 0;
}

void print_app_header()
{
	xil_printf("\n\r\n\r-----lwIP TCP echo server ------\n\r");
	xil_printf("TCP packets sent to port 6001 will be echoed back\n\r");
}

//Input DATA Processing Code

unsigned int pc_add(unsigned char p1, unsigned char p2){
		int check1 =1;
		int check2 =1;
		int K1 = (int)p1;
		int K2 = (int)p2;

		if (K1 < 97){
			check1 = 0;
		}
		if  (K2 <97){
			check2 =0;
		}
		if (check1 == 0){
			K1 = (K1-48) << 4;
		}
		else {
		    K1 = (K1 -87) <<4;
		}
		if (check2  == 0){
			K2 = (K2 -48);
		}
		else {
			K2 = (K2  - 87);
		}
		return  (K1+ K2);
	}



	// BRAM synchronization, Interrupt generation code.

err_t recv_callback(void *arg, struct tcp_pcb *tpcb,
                               struct pbuf *p, err_t err)
{
	/* do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		return ERR_OK;
	}

	/* indicate that the packet has been received */
	tcp_recved(tpcb, p->len);

	// Interrupt initalization from Ps part.
	XGpioPs_Config *Configptr;
	Configptr = XGpioPs_LookupConfig(0);
	XGpioPs_CfgInitialize(&pin, Configptr, Configptr -> BaseAddr);
	XGpioPs_SetDirectionPin(&pin, 54, 1);

	/* echo back the payload */
	/* in this case, we assume that the payload is < TCP_SND_BUF */
	if (tcp_sndbuf(tpcb) > p->len) {
		err = tcp_write(tpcb, p->payload, p->len, 1);
		i=0;
		//xil_printf("length of %d\n",p->len);

		while(p->len >i) {

				k1=*(char*)(p->payload +i);
				k2=*(char*)(p->payload +i+1);
				k3=*(char*)(p->payload +i+2);
				k4=*(char*)(p->payload +i+3);
				k5=*(char*)(p->payload +i+4);
				k6=*(char*)(p->payload +i+5);
				k7=*(char*)(p->payload +i+6);
				k8=*(char*)(p->payload +i+7);
				xil_printf("%d | ",addr );
				xil_printf("given value @Br  %c,%c,%c,%c,%c,%c,%c,%c | ", k1,k2,k3,k4,k5,k6,k7,k8);
				k = (pc_add(k1,k2)) + (pc_add(k3,k4)<<8) + (pc_add(k5,k6)<<16)+ (pc_add(k7,k8)<<24);

				//xil_printf("Actual value @Br %x |", k);

				if (addr<128)
				{
					Xil_Out32(XPAR_AXI_BRAM_CTRL_2_S_AXI_BASEADDR + addr,(u32)k);
					Data1 = Xil_In8(XPAR_AXI_BRAM_CTRL_2_S_AXI_BASEADDR + addr);
					Data2 = Xil_In8(XPAR_AXI_BRAM_CTRL_2_S_AXI_BASEADDR + addr+1);
					Data3 = Xil_In8(XPAR_AXI_BRAM_CTRL_2_S_AXI_BASEADDR + addr+2);
					Data4 = Xil_In8(XPAR_AXI_BRAM_CTRL_2_S_AXI_BASEADDR + addr+3);
					xil_printf("Br_@1 = %x, %x,%x,%x,%x \n\r",XPAR_AXI_BRAM_CTRL_2_S_AXI_BASEADDR + addr,Data1,Data2,Data3,Data4);

				}
				else if  ((addr>=128) & (addr < 256))
				{
					Xil_Out32(XPAR_AXI_BRAM_CTRL_3_S_AXI_BASEADDR + addr-128,(u32)k);
					Data1 = Xil_In8(XPAR_AXI_BRAM_CTRL_3_S_AXI_BASEADDR + (addr-128));
					Data2 = Xil_In8(XPAR_AXI_BRAM_CTRL_3_S_AXI_BASEADDR + (addr-127));
					Data3 = Xil_In8(XPAR_AXI_BRAM_CTRL_3_S_AXI_BASEADDR + (addr-126));
					Data4 = Xil_In8(XPAR_AXI_BRAM_CTRL_3_S_AXI_BASEADDR + (addr-125));
					xil_printf("Br_@2 = %x, %x,%x,%x,%x \n\r",XPAR_AXI_BRAM_CTRL_3_S_AXI_BASEADDR + addr-128,Data1,Data2,Data3,Data4);
				}
				else if  ((addr>=256) & (addr < 384))
				{
					XGpioPs_SetOutputEnablePin(&pin, 54, 1);
					XGpioPs_WritePin(&pin, 54, 0x0);
					Xil_Out32(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR + addr-256,(u32)k);
					Data1 = Xil_In8(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR + addr-256);
					Data2 = Xil_In8(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR + addr- 255);
					Data3 = Xil_In8(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR + addr-254);
					Data4 = Xil_In8(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR + addr-253);
					xil_printf("pc_@1 = %x, %x,%x,%x,%x \n\r",XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR + addr-256,Data1,Data2,Data3,Data4);
				}
				else if  ((addr>=384) & (addr < 512))
				{
					Xil_Out32(XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR + addr-384,(u32)k);
					Data1 = Xil_In8(XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR + addr-384);
					Data2 = Xil_In8(XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR + addr - 383);
					Data3 = Xil_In8(XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR + addr - 382);
					Data4 = Xil_In8(XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR + addr - 381);
					xil_printf("pc_@2 = %x, %x,%x,%x,%x \n\r",XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR + addr-384,Data1,Data2,Data3,Data4);
				}
				//xil_printf("%d | ",addr );
				addr = addr+4;
				i= i+8;

				if(addr>=512) {
					addr = 0;
					xil_printf("\nThe End");
					//XGpioPs_WritePin(&pin, 54, 0x0);
					XGpioPs_SetOutputEnablePin(&pin, 54, 1);
					XGpioPs_WritePin(&pin, 54, 0x1);
					int l=0;
					/*
					while (l<128){
							Corrected_Pred=Xil_In8(XPAR_AXI_BRAM_CTRL_4_S_AXI_BASEADDR + l);
							total_BR = Xil_In8(XPAR_AXI_BRAM_CTRL_5_S_AXI_BASEADDR + l);
							xil_printf("Corrected_Pred &  Total_Branches= %d, %d,",Corrected_Pred,total_BR);
							l=l+1;
					}*/

					while(1);
				}
		}

	} else
		xil_printf("no space in tcp_sndbuf\n\r");

	/* free the received pbuf */
	pbuf_free(p);

	return ERR_OK;
}

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	static int connection = 1;

	/* set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);

	/* just use an integer number indicating the connection id as the
	   callback argument */
	tcp_arg(newpcb, (void*)(UINTPTR)connection);

	/* increment for subsequent accepted connections */
	connection++;

	return ERR_OK;
}


int start_application()
{
	struct tcp_pcb *pcb;
	err_t err;
	unsigned port = 7;

	/* create new TCP PCB structure */
	pcb = tcp_new();
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = tcp_bind(pcb, IP_ADDR_ANY, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* we do not need any arguments to callback functions */
	tcp_arg(pcb, NULL);

	/* listen for connections */
	pcb = tcp_listen(pcb);
	if (!pcb) {
		xil_printf("Out of memory while tcp_listen\n\r");
		return -3;
	}

	/* specify callback to use for incoming connections */
	tcp_accept(pcb, accept_callback);

	xil_printf("TCP echo server started @ port %d\n\r", port);

	return 0;
}
