# airwav
A simple narrow band AM (ie : Aircraft band) recorder for RTL or airspy SDR.
Record in mp3 files 8k  mono.


## Usage
> airwav [-g gain] [-t threshold ] [-l interval ] [-d silenttime] [-v] [-s stationid] [-d directoty] [-r device] frequency (in Mhz)

 -t threshold :	squelch thresold in db (default -70 for rtlsdr -80 for airspy)

 -v : show signal level (usefull to set squelch thresold)

 -l interval : max duration of each mp3 file in second (0=no limit)

 -d silenttime : closed squelch time before closing mp3 file (default 300s)

 -s stationid :	station id (ie : -s LFRN) used in mp3 file name

 -d dir : mp3 directoy where files are saved (default .)

 -p ppm :  ppm freq shift
 -g gain in tens of db (ie : -g 400 ) 

 -r n : rtl device number (mandatory with rtl sdr)

## Example

> airwav -d audiodir -s LFRN -r 0 120.5

will create the mp3 file LFRN_120.50_2017-12-23T17:50:50Z.mp3 in dir audiodir

Note : mp3 files are created only if squelch opens. Recording stop when squelch is closed more than silenttime secondes

## Build

airwav depends of the mp3lame lib, usb lib and rtlsdr or airspy lib

For rtl sdr :
> make -f Makefile.rtl

for airspy :
> make -f Makefile.airspy

adapt CFLAGS to your needs and processor (ie : -mfpu=neon on recent ARM)

