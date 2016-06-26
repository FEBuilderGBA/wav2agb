#include <sndfile.h>
#include <stdlib.h>

#include "converter.h"

void convert_compressed(const char wav_file[], const char s_file[]) {
    SF_INFO in_info = {
        .format = 0;
    };

    SNDFILE *in_file = sf_open(wav_file, SFM_READ, &in_info);

    if (in_file == NULL)
        snddie(NULL);

    int samplerate = in_file.samplerate;
    int channels = in_file.channels;

    if (channels != 1) {
        fprintf(stderr, "Only mono WAVs are supported. Supplied file has %d channels\n", channels);
        exit(EXIT_FAILURE);
    }

    // get file length in samples

    const sf_count_t length = sf_seek(in_file, 0, SEEK_END);
    sf_count_t loop_start = 0;
    if (length == -1)
        snddie(in_file);

    char basenote = 60;
    char detune = 0;
    bool loop = false;
    SF_INSTRUMENT in_instr;
    if (sf_command(in_file, SFC_GET_INSTRUMENT, &in_instr, sizeof(in_instr)) == SF_TRUE) {
        if (in_instr.loops[0].mode != SF_LOOP_NONE) {
            loop = true;
            in_instr = in_instr.loops[0].start;
        }
        basenote = in_instr.basenote;
        detune = in_instr.detune;
    }

    FILE *out_file = fopen(s_file, "w");
    if (!out_file)
        pdie("fopen");

    char symbol_name[strlen(wav_file) + 1];
    strcpy(symbol_name, wav_file);
    char *symbol = basename(wav_file);
    strip_extension(symbol, "wav");

    fprintf(out_file, "@ File generated by wav2agb ~ ipatix\r\n\r\n");
    fprintf(out_file, "    .section .rodata\r\n");
    fprintf(out_file, "    .global %s\r\n", symbol);
    fprintf(out_file, "    .align  2\r\n\r\n%s:\r\n\r\n", symbol);

    // TODO ...

}

void convert_normal(const char wav_file[], const char s_file[]) {
}
