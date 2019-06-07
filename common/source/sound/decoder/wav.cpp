/*
 *   This file is part of PKSM
 *   Copyright (C) 2016-2019 Bernardo Giordano, Admiral Fish, piepie62
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
 *       * Requiring preservation of specified reasonable legal notices or
 *         author attributions in that material or in the Appropriate Legal
 *         Notices displayed by works containing it.
 *       * Prohibiting misrepresentation of the origin of that material,
 *         or requiring that modified versions of such material be marked in
 *         reasonable ways as different from the original version.
 */

#define DR_WAV_IMPLEMENTATION
#include <dr_libs/dr_wav.h>

#include "wav.hpp"

static const size_t    buffSize    = 16 * 1024;
static drwav*          pWav        = NULL;
static uint32_t        wavprogress;

WavDecoder::WavDecoder(const char* filename) {
    pWav = drwav_open_file(filename);
    wavprogress = 0;
    if (pWav == NULL)
        return;

    initialized = true;
}

WavDecoder::~WavDecoder(void) {
    drwav_close(pWav);
    initialized = false;
}

uint32_t WavDecoder::pos(void) {
    return wavprogress;
}

uint32_t WavDecoder::length(void) {
    return pWav->totalSampleCount;
}

uint32_t WavDecoder::decode(void* buffer)
{
    uint64_t out = drwav_read_s16(pWav, buffSize, reinterpret_cast<drwav_int16*>(buffer));
    wavprogress += out;
    return out;
}

bool WavDecoder::stereo(void)
{
    return pWav->channels-1;
}

uint32_t WavDecoder::sampleRate(void)
{
    return pWav->sampleRate;
}

uint32_t WavDecoder::bufferSize(void)
{
    return buffSize;
}
