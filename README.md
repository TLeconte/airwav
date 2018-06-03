# airwav
A simple narrow band AM (ie : Aircraft band) streamer/recorder for RTL or airspy SDR.
Record in VBR mp3 files or stream to stderr in CBR 32Kbps.


## Usage
> airwav [-g gain] [-t threshold ] [-l interval ] [-v] [-s stationid] [-d directoty] [-r device] frequency (in Mhz)

 -t threshold :	squelch thresold in db (default -50)

 -v : show signal level (usefull to set squelch thresold)

 -d dir : store in mp3 files in directoy dir insteed of streaming to stdout

 -l interval : max duration of each mp3 file in second (default 0=no limit)

 -s stationid :	station id (ie : -s LFRN) used in mp3 file name

 -p ppm :  ppm freq shift

 -g gain in tenth of db (ie : -g 400 ) 

 -r n : rtl device number (mandatory for rtl sdr)

## Example

> airwav -d audiodir -s LFRN -r 0 120.5

will create the mp3 file LFRN_120.50_2017-12-23T17:50:50Z.mp3 in directory audiodir.

Note : mp3 files are created only if squelch opens. Recording stop when squelch is closed more than 5 minutes.

> airwav -r 0  120.5 | ezstream -c ezstream.xml

send 32kbps CBR mp3 encoded audio to an Icecast server  (edit the ezstream.xml example file before launching)

Note : All examples are for rtl sdr, remove -r 0 for airspy

## Build

airwav depends of the mp3lame lib, usb lib and rtlsdr or airspy lib

For rtl sdr :
> make -f Makefile.rtl

for airspy :
> make -f Makefile.airspy

adapt CFLAGS to your needs and processor (ie : -mfpu=neon on recent ARM)

