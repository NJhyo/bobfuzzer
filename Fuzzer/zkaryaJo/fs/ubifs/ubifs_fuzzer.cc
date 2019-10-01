#include <set>
#include <sys/stat.h>

#include "ubifs_fuzzer.hh"
#include "ubifs.hh"
#include "utils.hh"

#include "config.h"


void ubifs_fuzzer::fix_checksum() 
{
	/*deploy ubifs's fix_checksum*/
}

void ubifs_fuzzer::fix_general_checksum() 
{
	/* deploy ubifs's fix_general_checksum */
}

void ubifs_fuzzer::compress(const char *in_path, void *buffer, const char *meta_path) 
{
  bool generate_meta_image = meta_path != NULL;
  
  int in_image_fd = open(in_path, O_RDONLY);

  if (in_image_fd < 0)
    FATAL("[-] image %s compression failed.", in_path);

  struct stat st;
  if (fstat(in_image_fd, &st) != 0) {
      FATAL("[-] image %s compression failed.", in_path);
  }

  image_size_ = st.st_size;
  image_buffer_ = buffer;

  if (read(in_image_fd, image_buffer_, image_size_) != image_size_) 
    FATAL("[-] image %s compression failed.", in_path);

  close(in_image_fd);


  int in_image_fd = open(in_path, O_RDONLY);
  if (in_image_fd < 0)
    FATAL("[-] image %s compression failed.", in_path);

  int meta_image_fd = -1;
  if (generate_meta_image) {
    meta_image_fd = open(meta_path, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (meta_image_fd < 0) {
      FATAL("[-] image %s compression failed.", in_path);
    }
  }

  /*deploy ubifs*/

}


/*-I didn't change-*/
void ubifs_fuzzer::decompress(const void *meta_buffer, size_t meta_len, bool checksum) {

  size_t meta_offset = 0;

  for (extent_t &extent : metadata_) {
    memcpy((char *)image_buffer_ + extent.first, (char *)meta_buffer + meta_offset, extent.second);
    meta_offset += extent.second;
  }

  if (checksum)
    fix_checksum();

}

/*-I didn't change-*/
void ubifs_fuzzer::general_decompress(const void *meta_buffer, size_t meta_len, bool checksum) {

  size_t meta_offset = 0;

  for (extent_t &extent : metadata_) {
    memcpy((char *)image_buffer_ + extent.first, (char *)meta_buffer + meta_offset, extent.second);
    meta_offset += extent.second;
  }

  if (checksum)
    fix_general_checksum();

}
