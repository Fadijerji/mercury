/*
 * Mercury: A configurable open-source software-defined modem.
 * Copyright (C) 2022-2024 Fadi Jerji
 * Author: Fadi Jerji
 * Email: fadi.jerji@  <gmail.com, caisresearch.com, ieee.org>
 * ORCID: 0000-0002-2076-5831
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, version 3 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <complex>
#include <fstream>
#include <math.h>
#include <iostream>
#include <complex>
#include "physical_layer/telecom_system.h"
#include "datalink_layer/arq.h"

int main(int argc, char *argv[])
{
	srand(time(0));

	cl_arq_controller ARQ;
	cl_telecom_system telecom_system;
	telecom_system.operation_mode=BER_PLOT_baseband;
	telecom_system.default_configurations_telecom_system.init_configuration=CONFIG_0;
	telecom_system.default_configurations_telecom_system.speaker_dev_name="plughw:0,0";
	telecom_system.default_configurations_telecom_system.microphone_dev_name="plughw:0,0";
	telecom_system.default_configurations_telecom_system.plot_plot_active=NO;
	ARQ.default_configuration_ARQ.tcp_socket_control_port=7002;
	ARQ.default_configuration_ARQ.tcp_socket_data_port=7003;
	ARQ.default_configuration_ARQ.gear_shift_on=NO;

	for (int i=1; i<argc; i++)
	{
		std::string arg(argv[i]);

		if (arg  == "-h" || arg  == "-help")
		{
			std::cout<<"-version"<<std::endl;
			std::cout<<"-mode [operation mode]"<<"              operation mode: ARQ_MODE, TX_TEST, RX_TEST, BER_PLOT_baseband, BER_PLOT_passband."<<std::endl;

			std::cout<<"-config [robustness configuration]"<<"  config: 0 to 16."<<std::endl;

			std::cout<<"-input [ALSA microphone device]"<<"     ALSA microphone device: (eg: \"plughw:0,0\")."<<std::endl;

			std::cout<<"-output [ALSA speaker device]"<<"       ALSA speaker device: (eg: \"plughw:0,0\")."<<std::endl;

			std::cout<<"-port [ARQ data port] "<<"              ARQ control port: TCP port."<<std::endl;

			std::cout<<"-gearshift"<<std::endl;
			std::cout<<"-plot"<<std::endl;

			std::cout<<std::endl;

			std::cout<<"Usage Example: mercury -mode RX_TEST -config 0  -input \"plughw:0,0\" -output \"plughw:0,0\" -plot"<<std::endl;

			exit(0);
		}
		if (arg  == "-version" || arg  == "-v")
		{
			std::cout<<"0"<<"."<<"2"<<" build:"<<"0"<<std::endl;
			exit(0);
		}
		else if (arg  == "-mode" && i!=(argc-1))
		{
			std::string arg_val(argv[i+1]);
			if(arg_val == "ARQ_MODE") {	telecom_system.operation_mode=ARQ_MODE;}
			else if(arg_val == "TX_TEST") {	telecom_system.operation_mode=TX_TEST;}
			else if(arg_val == "RX_TEST") {	telecom_system.operation_mode=RX_TEST;}
			else if(arg_val == "BER_PLOT_baseband") {telecom_system.operation_mode=BER_PLOT_baseband;}
			else if(arg_val == "BER_PLOT_passband") {telecom_system.operation_mode=BER_PLOT_passband;}
			else
			{
				std::cout<<"Wrong Mode.(try -h) Exiting.."<<std::endl;
				exit(0);
			}
		}
		else if (arg  == "-config" && i!=(argc-1))
		{
			std::string arg_val(argv[i+1]);
			try
			{
				int config=std::stoi(arg_val);
				if(config<0 || config>16)
				{
					throw(config);
				}
				telecom_system.default_configurations_telecom_system.init_configuration=config;
			}
			catch(...)
			{
				std::cout<<"Wrong config.(try -h) Exiting.."<<std::endl;
				exit(0);
			}

		}
		else if (arg  == "-input" && i!=(argc-1))
		{
			std::string arg_val(argv[i+1]);
			telecom_system.default_configurations_telecom_system.microphone_dev_name=arg_val;
		}
		else if (arg  == "-output" && i!=(argc-1))
		{
			std::string arg_val(argv[i+1]);
			telecom_system.default_configurations_telecom_system.speaker_dev_name=arg_val;
		}
		else if (arg  == "-port" && i!=(argc-1))
		{
			std::string arg_val(argv[i+1]);
			try
			{
				int port=std::stoi(arg_val);
				if(port<0 || port>65534)
				{
					throw(port);
				}
				ARQ.default_configuration_ARQ.tcp_socket_control_port=port;
				ARQ.default_configuration_ARQ.tcp_socket_data_port=port+1;
			}
			catch(...)
			{
				std::cout<<"Wrong port.(try -h) Exiting.."<<std::endl;
				exit(0);
			}

		}
		else if (arg  == "-plot")
		{
			telecom_system.default_configurations_telecom_system.plot_plot_active=YES;
		}
		else if (arg  == "-gearshift")
		{
			ARQ.default_configuration_ARQ.gear_shift_on=YES;
		}
		else if(arg[0]  == '-' && i!=(argc-1))
		{
			std::cout<<"Argument "<<arg <<" was not recognized.(try -h) Exiting.."<<std::endl;
			exit(0);
		}
	}


	if(telecom_system.operation_mode==ARQ_MODE)
	{
		ARQ.telecom_system=&telecom_system;
		ARQ.init();
		ARQ.print_stats();
		while(1)
		{
			ARQ.process_main();
		}
	}
	else if(telecom_system.operation_mode==RX_TEST)
	{
		telecom_system.load_configuration();
		telecom_system.constellation_plot.open("PLOT");
		telecom_system.constellation_plot.reset("PLOT");

		while(1)
		{
			telecom_system.RX_TEST_process_main();
		}
		telecom_system.constellation_plot.close();
	}
	else if (telecom_system.operation_mode==TX_TEST)
	{
		telecom_system.load_configuration();
		while(1)
		{
			telecom_system.TX_TEST_process_main();
		}
	}
	else if (telecom_system.operation_mode==BER_PLOT_baseband)
	{
		telecom_system.load_configuration();
		telecom_system.constellation_plot.open("PLOT");
		telecom_system.constellation_plot.reset("PLOT");
		telecom_system.BER_PLOT_baseband_process_main();
		telecom_system.constellation_plot.close();
	}
	else if(telecom_system.operation_mode==BER_PLOT_passband)
	{
		telecom_system.load_configuration();
		telecom_system.constellation_plot.open("PLOT");
		telecom_system.constellation_plot.reset("PLOT");
		telecom_system.BER_PLOT_passband_process_main();
		telecom_system.constellation_plot.close();
	}

	return 0;
}

