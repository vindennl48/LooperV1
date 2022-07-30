#include "Includes.h"

Ring<audio_block_t*> Buffers::input_l(AUDIO_BUFF_SIZE);
Ring<audio_block_t*> Buffers::input_r(AUDIO_BUFF_SIZE);

Ring<audio_block_t*> Buffers::output_l(AUDIO_BUFF_SIZE);
Ring<audio_block_t*> Buffers::output_r(AUDIO_BUFF_SIZE);
