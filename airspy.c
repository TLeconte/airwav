#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <complex.h>
#include <libairspy/airspy.h>
#include "airwav.h"

extern int verbose;
extern int gain;

extern void am_filter(complex float V);

static struct airspy_device *device = NULL;

static complex float V = 0;
static int idx = 0;

static int rx_callback(airspy_transfer_t * transfer)
{
	unsigned short *pt_rx_buffer;
	int i;

	pt_rx_buffer = (unsigned short *)(transfer->samples);

	for (i = 0; i < transfer->sample_count;) {
		float S0, S1;

		S0 = (float)((short)(pt_rx_buffer[i] & 0xfff) - 2048);
		i++;
		S1 = (float)((short)(pt_rx_buffer[i] & 0xfff) - 2048);
		i++;

		if (idx & 1)
			V += (S0 + S1) + (S1 - S0) * I;
		else
			V -= (S0 + S1) + (S1 - S0) * I;

		idx++;

		if (idx == DOWNSC) {
			am_filter(V / DOWNSC / 2048.0);
			V = 0;
			idx = 0;
		}
	}

	return 0;
}

int init_airspy(int freq)
{
	int result;
	uint32_t i, count;
	uint32_t *supported_samplerates;

	result = airspy_open(&device);
	if (result != AIRSPY_SUCCESS) {
		fprintf(stderr, "airspy_open() failed: %s (%d)\n",
			airspy_error_name(result), result);
		airspy_exit();
		return -1;
	}

	airspy_get_samplerates(device, &count, 0);
	supported_samplerates = (uint32_t *) malloc(count * sizeof(uint32_t));
	airspy_get_samplerates(device, supported_samplerates, count);
	for (i = 0; i < count; i++)
		if (supported_samplerates[i] == INRATE)
			break;
	if (i >= count) {
		fprintf(stderr, "did not find needed sampling rate\n");
		airspy_exit();
		return -1;
	}
	free(supported_samplerates);

	result = airspy_set_samplerate(device, i);
	if (result != AIRSPY_SUCCESS) {
		fprintf(stderr, "airspy_set_samplerate() failed: %s (%d)\n",
			airspy_error_name(result), result);
		airspy_close(device);
		airspy_exit();
		return -1;
	}

	result = airspy_set_sample_type(device, AIRSPY_SAMPLE_UINT16_REAL);
	if (result != AIRSPY_SUCCESS) {
		fprintf(stderr, "airspy_set_sample_type() failed: %s (%d)\n",
			airspy_error_name(result), result);
		airspy_close(device);
		airspy_exit();
		return -1;
	}

	if (gain > 15)
		gain = 15;
	result = airspy_set_linearity_gain(device, gain);
	if (result != AIRSPY_SUCCESS) {
		fprintf(stderr, "airspy_set_linearity_gain() failed: %s (%d)\n",
			airspy_error_name(result), result);
	}

	result = airspy_set_freq(device, freq);
	if (result != AIRSPY_SUCCESS) {
		fprintf(stderr, "airspy_set_freq() failed: %s (%d)\n",
			airspy_error_name(result), result);
		airspy_close(device);
		airspy_exit();
		return -1;
	}

	return 0;
}

int runAirspy(void)
{
	int result;

	result = airspy_start_rx(device, rx_callback, NULL);
	if (result != AIRSPY_SUCCESS) {
		fprintf(stderr, "airspy_start_rx() failed: %s (%d)\n",
			airspy_error_name(result), result);
		return -1;
	}

	do {
		sleep(1);
	} while (airspy_is_streaming(device) == AIRSPY_TRUE);

	return 0;
}
