/*
 * Mercury: A configurable open-source software-defined modem.
 * Copyright (C) 2022-2024 Fadi Jerji
 * Author: Fadi Jerji
 * Email: fadi.jerji@  <gmail.com, rhizomatica.org, caisresearch.com, ieee.org>
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

#ifndef INC_TELECOM_SYSTEM_H_
#define INC_TELECOM_SYSTEM_H_

#include "data_container.h"
#include "psk.h"
#include "awgn.h"
#include "error_rate.h"
#include "plot.h"
#include "ofdm.h"
#include "ldpc.h"
#include "alsa_sound_dev.h"
#include "interleaver.h"
#include "physical_config.h"
#include "physical_defines.h"
#include "datalink_layer/tcp_socket.h"
#include <iomanip>


struct st_receive_stats{
	int iterations_done;
	int delay;
	int delay_of_last_decoded_message;
	int time_peak_symb_location;
	int time_peak_subsymb_location;
	int sync_trials;
	double phase_error_avg;
	double freq_offset;
	double freq_offset_of_last_decoded_message;
	int message_decoded;
	double SNR;
	double signal_stregth_dbm;
};


class cl_telecom_system
{
private:


public:
	cl_telecom_system();
	~cl_telecom_system();
	cl_data_container data_container;
	cl_psk psk;
	cl_awgn awgn_channel;
	cl_error_rate error_rate;
	cl_ofdm ofdm;
	cl_error_rate passband_test_EsN0(float EsN0,int max_frame_no);
	cl_error_rate baseband_test_EsN0(float EsN0,int max_frame_no);
	cl_ldpc ldpc;
	cl_alsa_sound_device microphone;
	cl_alsa_sound_device speaker;
	double sampling_frequency;
	double carrier_frequency;
	double carrier_amplitude;
	int frequency_interpolation_rate;
	int time_sync_trials_max;
	int use_last_good_time_sync;
	int use_last_good_freq_offset;
	st_receive_stats receive_stats;
	cl_tcp_socket tcp_socket_test;

	double output_power_Watt;

	void transmit(const int* data, double* out, int message_location);
	st_receive_stats receive(const double* data, int* out);
	int operation_mode;
	float test_tx_AWGN_EsN0,test_tx_AWGN_EsN0_calibration;

	double M;
	double bandwidth;
	double LDPC_real_CR;
	double Tu;
	double Ts;
	double Tf;
	double rb;
	double rbc;
	double Shannon_limit;

	int bit_interleaver_block_size;
	int time_freq_interleaver_block_size;

	void calculate_parameters();

	void init();
	void deinit();
	cl_plot BER_plot, constellation_plot;

	void TX_TEST_process_main();
	void RX_TEST_process_main();
	void BER_PLOT_baseband_process_main();
	void BER_PLOT_passband_process_main();

	void load_configuration();
	void load_configuration(int configuration);
	int last_configuration;
	int current_configuration;
	void return_to_last_configuration();
	char get_configuration(double SNR);

	cl_configuration_telecom_system default_configurations_telecom_system;

};



#endif
