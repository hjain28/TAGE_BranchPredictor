/***README***
Assumption Know the basic VIVADO & SDK tool design flowd steps.

1. Make the TAGE IP from the given verilog source code.
2. Configure the connection and Block Diagram as shown in the provided Block diagram jpeg file.
	| Remeber to configure the BRAM in Standalone mode along with desired read/write mode.
3. After verifying (simulation and post Implemenattion simulation), Export Bitstream to SDK.
4. verify mapped address to SDK in decopilation phase.  
5. Establish Ethernet Connection as user guide.
6. New project -> IWP template.
7. Replace the echo.c file with given file.
8. Download and RUN it as configuration file.
9. After Establishing Connection, Transfer DATA file via "HERCULES" software.


NOTE :"HERCULES"  is a spftware act as Ftp terminal fordata transfer. can downlaod it from https://www.hw-group.com/products/hercules/index_en.html.

  

