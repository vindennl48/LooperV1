#ifdef INCLUDE_AUDIO_AUDIOFILE
#ifndef AUDIO_AUDIOFILE_H
#define AUDIO_AUDIOFILE_H

int16_t testbuf1[512] = {0};
int16_t testbuf2[512] = {0};
uint8_t testbuf1_sw = false;
uint8_t testbuf2_sw = false;

struct FileHeader {
  uint32_t file_size = sizeof(FileHeader);
  uint8_t  beats     = 0;

  uint32_t get_playhead() { return playhead; }
  void add_to_playhead(uint32_t x) {
    if ( playhead + x >= file_size ) {
      x = (playhead + x) - file_size;
      playhead = x + sizeof(FileHeader);
      return;
    }
    playhead += x;

//    SP("--> playhead: ", playhead);
  }
  void reset_playhead() {
    playhead = sizeof(FileHeader);
  }

private:
  uint32_t playhead = sizeof(FileHeader);
};


struct AudioFile {
  static char file_name[][7];

  uint8_t    layer;
  FileHeader header;
  ExFile     file;
  uint8_t    is_read = false;
  BiBuffer<BUFFER_SAMPLES> data;

  AudioFile(uint8_t layer) {
    this->layer = layer; 
  }
  ~AudioFile() {
    update_header();
    file.close();
  }

  void setup() {
    file.open(file_name[layer], O_RDWR | O_CREAT | O_TRUNC);
    update_header();
    //file.read(&header, sizeof(FileHeader));
  }

  void update_header() {
    file.seek(0);
    file.write(&header, sizeof(FileHeader));
    file.sync();
  }

  void write_to_sd() {
    if (is_read) { is_read = false; data.reset(); }

    if ( !data.pop_is_full() ) { return; }
    file.seek(EOF);
    file.sync();
//    SP("pos: ", file.curPosition());
    file.write(data.get_pop_buffer(), data.max_bytes());
    if (!testbuf1_sw) {
      testbuf1_sw = true;
      memcpy(testbuf1, data.get_pop_buffer(), data.max_bytes());
    }
    header.file_size += data.max_bytes();
    data.clear_pop_buffer();
  }

  void read_from_sd() {
    if (!is_read) { is_read = true; data.reset(); }

    if ( data.insert_is_empty() ) {
//      SP("playhead:  ", header.get_playhead());
//      SP("filesize:  ", header.file_size);
//      SP("max_bytes: ", data.max_bytes());
//      SP("1 position: ", file.position());
      file.seek(header.get_playhead());
//      SP("2 position: ", file.position());
      if ( header.get_playhead() + data.max_bytes() > header.file_size ) {
        file.read(data.get_insert_buffer(), header.file_size - header.get_playhead());
        header.add_to_playhead(header.file_size - header.get_playhead());
      } else {
        file.read(data.get_insert_buffer(), data.max_bytes());
        if (!testbuf2_sw) {
          testbuf2_sw = true;
//          SP("playhead: ", header.get_playhead());
          memcpy(testbuf2, data.get_insert_buffer(), data.max_bytes());
        }
        header.add_to_playhead(data.max_bytes());
      }
//    } else {
//      data.flush();
//      if ( digitalRead(BA_EXPAND_LED2_PIN) ) { SP("MADE IT!"); }
      data.set_insert_full();
    }
  }
};
char AudioFile::file_name[][7] = {
  "00.bin",
  "01.bin",
  "02.bin"
};

AudioFile audio_temp(0);


#endif
#endif
