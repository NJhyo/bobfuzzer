/* 
	made by zkaryaJo 
*/

#include <sys/stat.h>
#include <set>
#include <map>
#include "ubifs_fuzzer.hh"
#include "ubifs.hh"
#include "utils.hh"


/*
	superBlock Parsing
*/
void ubifs_fuzzer::ubifs_parse_superblock(ubifs_sb_node *sb) {
  // Load superblock

/*for (int i = 0; UBIFS_SUPERBLOCK_OFFSETS[i] != 0; i++) {
    uint64_t offset = UBIFS_SUPERBLOCK_OFFSETS[i];

    if (image_size_ < offset) {
      break;
    }

    // Only parse the first supberblock.
    if (i == 0) {
      memcpy(sb, (uint8_t*)image_buffer_ + offset, SUPERBLOCK_SIZE);
    }

    this->metadata_blocks.insert(offset);
  }
*/

   uint64_t offset = 0;   //superblock offset=0

   // SUPERBLOCK_SIZE = 0x1000 (4096)
   memcpy(sb, (uint8_t*)image_buffer_ + offset, SUPERBLOCK_SIZE);   //image_buffer_ -> sb로 0x1000만큼 복사.
   this->metadata_blocks.insert(offset);

}


/*
	MasterNode Parsing ( ~ing)
*/
void ubifs_fuzzer::ubifs_parse_masterNode(ubifs_sb_node *sb) {
  // Load masterNode

   uint64_t offset = 0;
   memcpy(sb, (uint8_t*)image_buffer_ + offset, SUPERBLOCK_SIZE); 
   this->metadata_blocks.insert(offset);

}


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

  int meta_image_fd = -1;
  if (generate_meta_image) {
    meta_image_fd = open(meta_path, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (meta_image_fd < 0) {
      FATAL("[-] image %s compression failed.", in_path);
    }
  }


  ubifs_sb_node *sb = (ubifs_sb_node*)malloc(SUPERBLOCK_SIZE);

  ubifs_parse_superblock(sb);
  printf("sb->ch->len:%ld \n", sb->ch.len);
  printf("leb_size:%ld \n", sb->leb_size);
  printf("min_io_size:%lld \n", sb->min_io_size);

/*  if (sb->node_size != sb->leaf_size) {
    // How often will this happen?
    printf("Need some attention.\n");
  }
  block_size_ = sb->node_size;
*/
/*
  ubifs_parse_chunk_tree(sb->chunk_tree_root_addr);
  ubifs_parse_root_tree(sb->root_tree_root_addr);
  ubifs_parse_tree(sb->log_tree_root_addr);
*/

  if (!release_metadata(this->metadata_blocks, meta_image_fd, false)) 
    FATAL("[-] image %s compression failed.", in_path);

  if (generate_meta_image) 
    close(meta_image_fd);

   print_metadata();

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

/* Print MetaData Offset Information */
void ubifs_fuzzer::print_metadata()
{
    for (std::set<uint64_t>::iterator it = this->metadata_blocks.begin();
      it != this->metadata_blocks.end(); ++it) {
    printf("metaData Offset: %lld\n", *it);

  }

}
