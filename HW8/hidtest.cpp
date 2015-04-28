#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"

#define MAX_STR 255
#define maxvalue 32767
#define samples 500

short combine(unsigned char msb, unsigned char lsb);

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[65];
	unsigned char bufr[8];
	signed short accel[samples][3];
	signed short xaccel[100];
	signed short yaccel[100];
	signed short zaccel[100];
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	// Initialize the hidapi library
	res = hid_init();

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x4d8, 0x3f, NULL);

		// Toggle LED (cmd 0x80). The first byte is the report number (0x0).
	buf[0] = 0x0;
	buf[1] = 1;
	printf("%s", "Please enter row number: ");
	scanf("%d", &buf[2]);
	printf("%s", "Please enter string: ");
	scanf("%s", &buf[3]);
	res = hid_write(handle, buf, 65);

	// Read requested state
	i = 0;
	while (i <= samples)
	{
		buf[0] = 0x0;
		buf[1] = 2;
		res = hid_write(handle, buf, 65);

		res = hid_read(handle, bufr, 8);
		if (bufr[0] == 1)
		{
			accel[i][0] = combine(bufr[1], bufr[2]);
			accel[i][1] = combine(bufr[3], bufr[4]);
			accel[i][2] = combine(bufr[5], bufr[6]);
			i = i + 1;
		}
	};
	// Finalize the hidapi library

	FILE *ofp;

	ofp = fopen("accels.txt", "w");

	for (i = 0; i<= samples; i++) {

		fprintf(ofp, "%d %d %d\n", accel[i][0], accel[i][1], accel[i][2]);

	}

	fclose(ofp);

	res = hid_exit();

	return 0;
}

short combine(unsigned char msb, unsigned char lsb) {
	return ~((msb << 8) | lsb) + 1;
}