#include <stdio.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <lame/lame.h>
#include "airwav.h"

FILE *mp3fd = NULL;

static lame_global_flags *gfp;

int mp3_init(void)
{
	time_t tu;
	char tstr[64];
	char name[128];
	char frstr[16];

	gfp = lame_init();
	if (gfp == NULL)
		return 1;

	lame_set_num_channels(gfp, 1);
	lame_set_in_samplerate(gfp, 8000);
	lame_set_mode(gfp, 3);

	if(directory) {

		/* store in mp3 files */
		tu = time(NULL);
		strftime(tstr, 128, "%Y-%m-%dT%H:%M:%SZ", gmtime(&tu));

		sprintf(frstr, "%3.2f", (float)freq / 1000000.0);

		if(stid)
			sprintf(name, "%s/%s_%s_%s.mp3", directory, stid, frstr, tstr);
		else
			sprintf(name, "%s/%s_%s.mp3", directory, frstr, tstr);

		mp3fd = fopen(name, "w+");
		if (mp3fd == NULL)
			return -1;

		lame_set_VBR(gfp, vbr_default);

		id3tag_init(gfp);
		id3tag_set_artist(gfp, stid);
		id3tag_set_album(gfp, frstr);
		id3tag_set_title(gfp, tstr);
		id3tag_set_comment(gfp, "airwav SDR receiver");

	} else {
		/* stream to stderr */
		mp3fd=stdout;

		lame_set_brate(gfp, 32);
	}

	lame_init_params(gfp);

	return 0;
}

int mp3_encode(short *buffer, int nbs)
{
	int len;
	unsigned char mp3buf[LAME_MAXMP3BUFFER];
	int mp3buf_size;

	len =
	    lame_encode_buffer(gfp, buffer, buffer, nbs, mp3buf,
			       LAME_MAXMP3BUFFER);

	if (len) {
		fwrite(mp3buf, 1, len, mp3fd);
	}

}

int mp3_close(void)
{
	int len;
	unsigned char mp3buf[LAME_MAXMP3BUFFER];
	int mp3buf_size;

	len = lame_encode_flush(gfp, mp3buf, LAME_MAXMP3BUFFER);
	if (len)
		fwrite(mp3buf, 1, len, mp3fd);

	if(directory)
		lame_mp3_tags_fid(gfp, mp3fd);

	lame_close(gfp);

	if(mp3fd!=stdout)
		 fclose(mp3fd);

	mp3fd = NULL;

}
