#ifndef FS_FUZZ_UBIFS_FUZZER_HH
#define FS_FUZZ_UBIFS_FUZZER_HH

#include "ubifs.hh"
#include "fsfuzzer.hh"

static class ubifs_fuzzer: public fsfuzzer 
{
    public:
      ubifs_fuzzer(): fsfuzzer("ubifs") { }

      void fix_checksum();

      void fix_general_checksum();

      void compress(const char *in_path, void *buffer, const char *meta_path = NULL);

      void decompress(const void *meta_buffer, size_t meta_len, bool checksum = true);

      void general_decompress(const void *meta_buffer, size_t meta_len, bool checksum = true);

      void ubifs_parse_superblock(ubifs_sb_node *sb);
      void ubifs_parse_mstNode(ubifs_mst_node *mst);
      void ubifs_parse_LPT(ubifs_mst_node *mst);
      void ubifs_parse_rootTree(ubifs_mst_node *mst, ubifs_idx_node *idx);

      void print_superblock(ubifs_sb_node *sb);
      void print_masternode(ubifs_mst_node *mst);
      void print_metadata();
      void print_rootTree(ubifs_idx_node *idx);

    private:
    std::set<uint64_t> metadata_blocks;

} ubifs_fuzzer;

#endif
