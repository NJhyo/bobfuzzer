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
 * SUPER_BLOCK Parsing
 * offset          = 0            //superblock is fixed in ubifs FileSystem 
 * SUPERBLOCK_SIZE = 0x1000 (4096)
 */
void ubifs_fuzzer::ubifs_parse_superblock(ubifs_sb_node *sb) {

  uint64_t offset = 0;   

  memcpy(sb, (uint8_t*)image_buffer_ + offset, SUPERBLOCK_SIZE);
  this->metadata_blocks.insert(offset);

}


/*
 * MASTER_NODE1 Parsing       // current MasterNode2 is not parsing
 * offset = UBIFS_LEB_SIZE    // mstNode is fixed in ubifs FileSystem 
 */
void ubifs_fuzzer::ubifs_parse_mstNode(ubifs_mst_node *mst) {

  uint64_t offset = UBIFS_LEB_SIZE; 
  memcpy(mst, (uint8_t*)image_buffer_ + offset, MST_NODE_SIZE);

  this->metadata_blocks.insert(offset);
}

/*
 *
 * LPT : LEB Properties Tree
 * offset = 1LEB = 15872 Byte
 *
 */
void ubifs_fuzzer::ubifs_parse_LPT(ubifs_mst_node *mst) {

  //1. find LPT by mst
  uint64_t LPT_LNUM = mst->lpt_lnum;
  uint64_t LPT_OFFS = mst->lpt_offs;

  uint64_t offset = LPT_LNUM*UBIFS_LEB_SIZE+LPT_OFFS;

  this->metadata_blocks.insert(offset);

//  memcpy(buf, (uint8_t*)image_buffer_ + offset, sizeof(buf));


//2. find rootTree by LPT

  /*uint64_t offset = UBIFS_LEB_SIZE;
  uint64_t LEB_CNT = sb.leb_cnt;

  uint64_t LPT_LEB = sb->lpt_lebs

  for(int i=3;i<LEB_CNT;i++){
  
  offset = i*UBIFS_LEB_SIZE;

  struct ubifs_ch *ch = (ubifs_ch*)((uint8_t*)image_buffer_ + offset); // 
  
  if(ch.node_type)

  this->metadata_blocks.insert(node_paddr);

  memcpy(ch, (uint8_t*)image_buffer_ + offset, CH_NODE_SIZE);
  }

   this->metadata_blocks.insert(offset);*/

}

/*
 *  rootTree Parsing
 */
void ubifs_fuzzer::ubifs_parse_rootTree(ubifs_mst_node *mst, ubifs_idx_node *idx) {

  //1. find rootIndexNode with mst?
  uint64_t ROOT_LNUM = mst->root_lnum;
  uint64_t ROOT_OFFS = mst->root_offs;
  uint64_t ROOT_LEN = mst->root_len;
  uint64_t offset = ROOT_LNUM*UBIFS_LEB_SIZE+ROOT_OFFS; // reached mst > idx

  memcpy(idx, (uint8_t*)image_buffer_ + offset, ROOT_LEN);

  this->metadata_blocks.insert(offset);

/**
 * struct ubifs_branch - key/reference/length branch
 * @lnum: LEB number of the target node
 * @offs: offset within @lnum
 * @len: target node length
 * @key: key
 */
// typedef struct {
//   uint32_t lnum;
//   uint32_t offs;
//   uint32_t len;
//   uint8_t key[];
// } __attribute__((packed)) ubifs_branch;

    //we can find idx + offset = branches address
    offset += 28; //reached at mst > idx > branches's start

    this->metadata_blocks.insert(offset);

    ubifs_branch *root = (ubifs_branch*)malloc(sizeof(ubifs_branch));

    memcpy(root, (uint8_t*)image_buffer_ + offset, sizeof(ubifs_branch));

    printf("==================\n");
    printf("root : lnum : %lld\n", root->lnum);
    printf("root : offs : %lld\n", root->offs);
    printf("root : len : %lld\n", root->len);
    printf("root : key : %lld\n, %d", root->key, sizeof(root->key));
    printf("==================\n");



    offset += sizeof(root);

    this->metadata_blocks.insert(offset);
    
    ubifs_branch *root2 = (ubifs_branch*)malloc(sizeof(ubifs_branch));

    memcpy(root2, (uint8_t*)image_buffer_ + offset, sizeof(ubifs_branch));

    printf("==================\n");
    printf("root2 : lnum : %lld\n", root2->lnum);
    printf("root2 : offs : %lld\n", root2->offs);
    printf("root2 : len : %lld\n", root2->len);
    printf("root2 : key : %lld\n", root2->key);
    printf("==================\n");

  //2. find metaData with rootIndexNode 
  //

// typedef struct {
//   struct ubifs_ch ch;
//   uint16_t child_cnt;  @child_cnt: number of child index nodes
//   uint16_t level;      @level: tree level
//   uint8_t branches[];  @branches: LEB number / offset / length / key branches
// } __attribute__((packed)) ubifs_idx_node;

/*  for(int i=0;i<100;i++){

    if()

  }

*/

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

  ubifs_mst_node *mst = (ubifs_mst_node*)malloc(MST_NODE_SIZE);
  ubifs_parse_mstNode(mst);

  ubifs_idx_node *idx = (ubifs_idx_node*)malloc(mst->root_len);
  ubifs_parse_rootTree(mst,idx);
  ubifs_parse_LPT(mst);


  //print_superblock(sb);
  print_masternode(mst);
  print_rootTree(idx);




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
    printf("=====================print Meta Data=============================\n");
    for (std::set<uint64_t>::iterator it = this->metadata_blocks.begin();
      it != this->metadata_blocks.end(); ++it) {
      printf("metaData Offset: %lld\n", *it);

    }
    printf("=====================print End====================================\n");
}

/* Print MasterNode */
void ubifs_fuzzer::print_masternode(ubifs_mst_node *mst)
{
    	printf("=====================print MasterNode=============================\n");

	printf("mst->ch->len:%lld \n", mst->ch.len);
	printf("mst->highest_inum:%lld \n", mst->highest_inum);
	printf("mst->cmt_no:%lld \n", mst->cmt_no);
	printf("log_lnum:%lld \n", mst->log_lnum);
	printf("\n");
	printf("root_lnum:%lld \n", mst->root_lnum);
	printf("root_offs:%lld \n", mst->root_offs);
	printf("root_len:%lld \n", mst->root_len);
	printf("\n");
	printf("ihead_lnum:%lld \n", mst->ihead_lnum);
	printf("ihead_offs:%lld \n", mst->ihead_offs);
	printf("\n");
	printf("index_size:%lld \n", mst->index_size);
	printf("\n");
	printf("lpt_lnum:%lld \n", mst->lpt_lnum);
	printf("lpt_offs:%lld \n", mst->lpt_offs);
	printf("\n");
	printf("nhead_lnum:%lld \n", mst->nhead_lnum);
	printf("nhead_offs:%lld \n", mst->nhead_offs);
	printf("\n");
	printf("ltab_lnum:%lld \n", mst->ltab_lnum);
	printf("ltab_offs:%lld \n", mst->ltab_offs);
	printf("\n");
	printf("lsave_lnum:%lld \n", mst->lsave_lnum);
	printf("lsave_offs:%lld \n", mst->lsave_offs);
	printf("\n");
	printf("lscan_lnum:%lld \n", mst->lscan_lnum);
	printf("\n");
	printf("leb_cnt:%lld \n", mst->leb_cnt);
	printf("\n");
	printf("idx_lebs:%lld \n", mst->idx_lebs);

	printf("=====================print End====================================\n");
}

/* Print SuperBlock */
void ubifs_fuzzer::print_superblock(ubifs_sb_node *sb)
{
    	printf("=====================print SuperBlock=============================\n");

	printf("sb->ch->len:%ld \n", sb->ch.len);
	printf("key_hash:%ld \n", sb->key_hash);
	printf("key_fmt:%ld \n", sb->key_fmt);
	printf("flags:%ld \n", sb->flags);
	printf("min_io_size:%ld \n", sb->min_io_size);
	printf("leb_size:%ld \n", sb->leb_size);
	printf("sb->leb_cnt:%lld \n", sb->leb_cnt);
	printf("sb->max_leb_cnt:%lld \n", sb->max_leb_cnt);
	printf("\n");
	printf("sb->log_lebs:%lld \n", sb->log_lebs);
	printf("sb->lpt_lebs:%lld \n", sb->lpt_lebs);
	printf("sb->orph_lebs:%lld \n", sb->orph_lebs);
	printf("sb->jhead_cnt:%lld \n", sb->jhead_cnt);
	printf("sb->fanout:%lld \n", sb->fanout);
	printf("\n");
	printf("sb->lsave_cnt:%lld \n", sb->lsave_cnt);
	printf("sb->default_compr:%lld \n", sb->default_compr);
	printf("\n");
	printf("sb->rp_uid:%lld \n", sb->rp_uid);
	printf("sb->rp_gid:%lld \n", sb->rp_gid);
	printf("sb->rp_size:%lld \n", sb->rp_size);
	printf("\n");
	//  printf("sb->hash_mst:%lld \n", sb->hash_mst);
	//  printf("sb->hmac_wkm:%lld \n", sb->hmac_wkm);
	//  printf("sb->hmac:%lld \n", sb->hmac);
	printf("=====================print End====================================\n");
}

/* Print rootTree */
void ubifs_fuzzer::print_rootTree(ubifs_idx_node *idx)
{

// typedef struct {
//   struct ubifs_ch ch;
//   uint16_t child_cnt;  @child_cnt: number of child index nodes
//   uint16_t level;      @level: tree level
//   uint8_t branches[];  @branches: LEB number / offset / length / key branches
// } __attribute__((packed)) ubifs_idx_node;

  printf("=====================print rootIDX=============================\n");
  printf("idx->ch->len:%ld \n", idx->ch.len);
  printf("child_cnt:%ld \n", idx->child_cnt);
  printf("level:%ld \n", idx->level);
  printf("branches:%lld \n", idx->branches);
  printf("=====================print End====================================\n");
}

void ubifs_fuzzer::fix_checksum() 
{
  /*deploy ubifs's fix_checksum*/
}

void ubifs_fuzzer::fix_general_checksum() 
{
  /* deploy ubifs's fix_general_checksum */
}


