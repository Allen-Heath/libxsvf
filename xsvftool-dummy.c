/*
 *  Lib(X)SVF  -  A library for implementing SVF and XSVF JTAG players
 *
 *  Copyright (C) 2009  RIEGL Research ForschungsGmbH
 *  Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
 *  
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "libxsvf.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct user_data_s {
	FILE *file;
};

static int h_setup(struct libxsvf_host* h) {
	return 0;
}

static int h_shutdown(struct libxsvf_host* h) {
	return 0;
}

static void h_udelay(struct libxsvf_host* h, long usecs, int tms, long num_tck) {
}

static int h_getbyte(struct libxsvf_host* h) {
	struct user_data_s* u = h->user_data;
	return fgetc(u->file);
}

static int h_pulse_tck(struct libxsvf_host* h, int tms, int tdi, int tdo, int rmask, int sync) {
	return tdo < 0 ? 1 : tdo;
}

static int h_set_frequency(struct libxsvf_host* h, int f) {
	return 0;
}

static void h_report_error(struct libxsvf_host* h, char const* file, int line, char const* message) {
	fprintf(stderr, "%s:%d - %s\n", file, line, message);
}

static void* h_realloc(struct libxsvf_host* h, void* ptr, int size, enum libxsvf_mem which) {
	return realloc(ptr, size);
}

static struct user_data_s u = {
};

static struct libxsvf_host h = {
	.udelay = h_udelay,
	.setup = h_setup,
	.shutdown = h_shutdown,
	.getbyte = h_getbyte,
	.pulse_tck = h_pulse_tck,
	.set_frequency = h_set_frequency,
	.report_error = h_report_error,
	.realloc = h_realloc,
	.user_data = &u
};

char const* progname;

static int play_file(char const* filename, enum libxsvf_mode mode) {
	u.file = fopen(filename, "rb");

	if (!u.file) {
		fprintf(stderr, "failed to open file '%s' (%s)\n", filename, strerror(errno));
		return 1;
	}

	int ret = libxsvf_play(&h, mode);

	if (ret < 0) {
		fprintf(stderr, "error playing file '%s'\n", filename);
	}

	fclose(u.file);
	return ret;
}

static void help() {
	fprintf(stderr, "\n");
	fprintf(stderr, "A dummy JTAG SVF/XSVF Player based on libxsvf\n");
	fprintf(stderr, "Useful for testing that libxsvf can parse your (X)SVF files ");
	fprintf(stderr, "without needing an actual target device\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: %s { -s svf-file | -x xsvf-file }\n", progname);
	fprintf(stderr, "\n");
	fprintf(stderr, "   -s svf-file\n");
	fprintf(stderr, "          Play the specified SVF file\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "   -x xsvf-file\n");
	fprintf(stderr, "          Play the specified XSVF file\n");
	fprintf(stderr, "\n");
	exit(1);
}

int main(int argc, char** argv) {
	progname = argc > 0 ? argv[0] : "xsvftool-dummy";
	int opt;

	while ((opt = getopt(argc, argv, "x:s:")) != -1) {
		switch (opt) {
			case 'x':
				return play_file(optarg, LIBXSVF_MODE_XSVF);

			case 's':
				return play_file(optarg, LIBXSVF_MODE_SVF);

			default:
				help();
		}
	}

	help();
}
