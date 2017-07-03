/*
 * nplay.c
 *
 * Created on: Jul 17, 2012
 *     Author: Ekawahyu Susilo
 *
 * Copyright (c) 2012, Chongqing Aisenke Electronic Technology Co., Ltd.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the copyright holder.
 * 
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "system.h"
#include "ioctl.h"

#define NPLAY_TRACK_JUMP_LEN	0xFFFF

int nplay_app(int argc, char **argv)
{
	char file_buffer[32];
	int fd = 0, br;
	int filesize = 0;

	int key = 0;
	int paused = 0;
	int vol;

	int audio_stream;

	/* trying to get access to audio device */
	audio_stream = open("/dev/audio1", O_RDWR);

	if (audio_stream < 0) {
		if (errno == ENODEV) fprintf(stderr, "%s: no audio device\n", argv[0]);
		if (errno == ETXTBSY) fprintf(stderr, "%s: audio device busy\n", argv[0]);
		return audio_stream;
	}

	/* set the audio volume */
	ioctl(audio_stream, IOCTL_SOUND_DEVICE_SOFT_RESET, NULL);
	ioctl(audio_stream, IOCTL_SOUND_MIXER_READ_VOLUME, &vol);

	/* opening a media file */
	fd = open(argv[1], O_RDONLY, 0);

	if (fd < 0) {
		if (errno == ENODEV) fprintf(stderr, "%s: open failed\n", argv[0]);
		if (errno == ETXTBSY) fprintf(stderr, "%s: file busy\n", argv[0]);
		if (errno == ENOENT) fprintf(stderr, "%s: no file/dir\n", argv[0]);
		close(audio_stream);
		return fd;
	}

	/* play the media file */
	do {
		br = read(fd, file_buffer, sizeof(file_buffer));
		if (br < 0) fprintf(stderr, "%s: read failed\n", argv[0]);
		if (br > 0) {
			filesize += br;
			write(audio_stream, file_buffer, sizeof(file_buffer));
		}
		/*if (keypressed()) {
			do {
				key = getchar();
				//if (key == KEY_ETX) break; // Ctrl-C
				if (key == 'p' || key == 'P') paused = !paused;
			} while (paused);
			//if (key == KEY_ETX) break; // Ctrl-C
			if (key == 'A') {
				if (vol < 255) vol++;
				ioctl(audio_stream, IOCTL_SOUND_MIXER_WRITE_VOLUME, &vol);
			}
			if (key == 'B') {
				if (vol > 0) vol--;
				ioctl(audio_stream, IOCTL_SOUND_MIXER_WRITE_VOLUME, &vol);
			}
			if (key == 'C') lseek(fd, NPLAY_TRACK_JUMP_LEN, SEEK_CUR); // right arrow
			if (key == 'D') lseek(fd, -NPLAY_TRACK_JUMP_LEN, SEEK_CUR); // left arrow
		}*/
	} while (br);
	printf("nplay: %i bytes\n", filesize);

	/* closing audio device and media file */
	if (close(fd) < 0) fprintf(stderr, "%s: close failed\n", argv[0]);
	if (close(audio_stream) < 0) fprintf(stderr, "%s: close failed\n", argv[0]);

	return 0;
}

int nplay_help(int argc, char **argv)
{
	printf("NodinoSYS nplay v0.1\nusage: nplay [filename|playlist]\nkey:\n");
	printf("  volume+ ('A'/up)\n  volume- ('B'/down)\n  fastfwd ('C'/right)\n");
	printf("  rewind  ('D'/left)\n  pause   ('P')\n");

	return 0;
}
