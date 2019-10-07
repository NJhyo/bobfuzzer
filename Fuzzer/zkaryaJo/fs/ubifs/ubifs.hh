#ifndef FS_FUZZ_BTRFS_HH
#define FS_FUZZ_BTRFS_HH

#include <stdint.h>

#define UUID_LEN 0x10
#define CHECKSUM_LEN 0x20
#define SUPERBLOCK_SIZE 0x1000

#define UBIFS_NODE_MAGIC  0x06101831 /* UBIFS node magic number */
#define UBIFS_FORMAT_VERSION 5
#define UBIFS_RO_COMPAT_VERSION 0
#define UBIFS_MIN_LEB_SZ (15*1024)/* Minimum logical eraseblock size in bytes */
#define UBIFS_CRC32_INIT 0xFFFFFFFFU/* Initial CRC32 value used when calculating CRC checksums */
#define UBIFS_MIN_COMPR_LEN 128
#define UBIFS_MIN_COMPRESS_DIFF 64
#define UBIFS_ROOT_INO 1 /* Root inode number */
#define UBIFS_FIRST_INO 64 /* Lowest inode number used for regular inodes (not UBIFS-only internal ones) */
#define UBIFS_MAX_NLEN 255
#define UBIFS_MAX_JHEADS 1 /* Maximum number of data journal heads */
#define UBIFS_BLOCK_SIZE  4096
#define UBIFS_BLOCK_SHIFT 12
#define UBIFS_PADDING_BYTE 0xCE /* UBIFS padding byte pattern (must not be first or last byte of node magic) */
#define UBIFS_MAX_KEY_LEN 16 /* Maximum possible key length */
#define UBIFS_SK_LEN 8 /* Key length ("simple" format) */
#define UBIFS_MIN_FANOUT 3 /* Minimum index tree fanout */
#define UBIFS_MAX_LEVELS 512 /* Maximum number of levels in UBIFS indexing B-tree */
#define UBIFS_MAX_INO_DATA UBIFS_BLOCK_SIZE /* Maximum amount of data attached to an inode in bytes */
#define UBIFS_LPT_FANOUT 4 /* LEB Properties Tree fanout (must be power of 2) and fanout shift */
#define UBIFS_LPT_FANOUT_SHIFT 2
#define UBIFS_LPT_CRC_BITS 16 /* LEB Properties Tree bit field sizes */
#define UBIFS_LPT_CRC_BYTES 2
#define UBIFS_LPT_TYPE_BITS 4
#define UBIFS_KEY_OFFSET offsetof(struct ubifs_ino_node, key) /* The key is always at the same position in all keyed nodes */
#define UBIFS_GC_HEAD   0 /* Garbage collector journal head number */
#define UBIFS_BASE_HEAD 1 /* Base journal head number */
#define UBIFS_DATA_HEAD 2 /* Data journal head number */

#define KB 1024ull
#define MB 1024ull*KB
#define GB 1024ull*MB
#define TB 1024ull*GB

const uint64_t UBIFS_SUPERBLOCK_OFFSETS[] = {0};

/*
 * LEB Properties Tree node types.
 *
 * UBIFS_LPT_PNODE: LPT leaf node (contains LEB properties)
 * UBIFS_LPT_NNODE: LPT internal node
 * UBIFS_LPT_LTAB: LPT's own lprops table
 * UBIFS_LPT_LSAVE: LPT's save table (big model only)
 * UBIFS_LPT_NODE_CNT: count of LPT node types
 * UBIFS_LPT_NOT_A_NODE: all ones (15 for 4 bits) is never a valid node type
 */
enum {
	UBIFS_LPT_PNODE,
	UBIFS_LPT_NNODE,
	UBIFS_LPT_LTAB,
	UBIFS_LPT_LSAVE,
	UBIFS_LPT_NODE_CNT,
	UBIFS_LPT_NOT_A_NODE = (1 << UBIFS_LPT_TYPE_BITS) - 1,
};

/*
 * UBIFS inode types.
 *
 * UBIFS_ITYPE_REG: regular file
 * UBIFS_ITYPE_DIR: directory
 * UBIFS_ITYPE_LNK: soft link
 * UBIFS_ITYPE_BLK: block device node
 * UBIFS_ITYPE_CHR: character device node
 * UBIFS_ITYPE_FIFO: fifo
 * UBIFS_ITYPE_SOCK: socket
 * UBIFS_ITYPES_CNT: count of supported file types
 */
enum {
	UBIFS_ITYPE_REG,
	UBIFS_ITYPE_DIR,
	UBIFS_ITYPE_LNK,
	UBIFS_ITYPE_BLK,
	UBIFS_ITYPE_CHR,
	UBIFS_ITYPE_FIFO,
	UBIFS_ITYPE_SOCK,
	UBIFS_ITYPES_CNT,
};

/*
 * Supported key hash functions.
 *
 * UBIFS_KEY_HASH_R5: R5 hash
 * UBIFS_KEY_HASH_TEST: test hash which just returns first 4 bytes of the name
 */
enum {
	UBIFS_KEY_HASH_R5,
	UBIFS_KEY_HASH_TEST,
};

/*
 * Supported key formats.
 *
 * UBIFS_SIMPLE_KEY_FMT: simple key format
 */
enum {
	UBIFS_SIMPLE_KEY_FMT,
};

/*
 * The simple key format uses 29 bits for storing UBIFS block number and hash
 * value.
 */
#define UBIFS_S_KEY_BLOCK_BITS 29
#define UBIFS_S_KEY_BLOCK_MASK 0x1FFFFFFF
#define UBIFS_S_KEY_HASH_BITS  UBIFS_S_KEY_BLOCK_BITS
#define UBIFS_S_KEY_HASH_MASK  UBIFS_S_KEY_BLOCK_MASK

/*
 * Key types.
 *
 * UBIFS_INO_KEY: inode node key
 * UBIFS_DATA_KEY: data node key
 * UBIFS_DENT_KEY: directory entry node key
 * UBIFS_XENT_KEY: extended attribute entry key
 * UBIFS_KEY_TYPES_CNT: number of supported key types
 */
enum {
	UBIFS_INO_KEY,
	UBIFS_DATA_KEY,
	UBIFS_DENT_KEY,
	UBIFS_XENT_KEY,
	UBIFS_KEY_TYPES_CNT,
};


#define UBIFS_SB_LEBS 1 /* Count of LEBs reserved for the superblock area */
#define UBIFS_MST_LEBS 2 /* Count of LEBs reserved for the master area */
#define UBIFS_SB_LNUM 0 /* First LEB of the superblock area */
#define UBIFS_MST_LNUM (UBIFS_SB_LNUM + UBIFS_SB_LEBS) /* First LEB of the master area */
#define UBIFS_LOG_LNUM (UBIFS_MST_LNUM + UBIFS_MST_LEBS) /* First LEB of the log area */

/*
 * The below constants define the absolute minimum values for various UBIFS
 * media areas. Many of them actually depend of flash geometry and the FS
 * configuration (number of journal heads, orphan LEBs, etc). This means that
 * the smallest volume size which can be used for UBIFS cannot be pre-defined
 * by these constants. The file-system that meets the below limitation will not
 * necessarily mount. UBIFS does run-time calculations and validates the FS
 * size.
 */


#define UBIFS_MIN_LOG_LEBS 2 /* Minimum number of logical eraseblocks in the log */
#define UBIFS_MIN_BUD_LEBS 3 /* Minimum number of bud logical eraseblocks (one for each head) */
#define UBIFS_MIN_JNL_LEBS (UBIFS_MIN_LOG_LEBS + UBIFS_MIN_BUD_LEBS) /* Minimum number of journal logical eraseblocks */
#define UBIFS_MIN_LPT_LEBS 2 /* Minimum number of LPT area logical eraseblocks */
#define UBIFS_MIN_ORPH_LEBS 1 /* Minimum number of orphan area logical eraseblocks */

#define UBIFS_MIN_MAIN_LEBS (UBIFS_MIN_BUD_LEBS + 6)

/* Minimum number of logical eraseblocks */
#define UBIFS_MIN_LEB_CNT (UBIFS_SB_LEBS + UBIFS_MST_LEBS + \
			   UBIFS_MIN_LOG_LEBS + UBIFS_MIN_LPT_LEBS + \
			   UBIFS_MIN_ORPH_LEBS + UBIFS_MIN_MAIN_LEBS)

/* Node sizes (N.B. these are guaranteed to be multiples of 8) */
#define UBIFS_CH_SZ        sizeof(struct ubifs_ch)
#define UBIFS_INO_NODE_SZ  sizeof(struct ubifs_ino_node)
#define UBIFS_DATA_NODE_SZ sizeof(struct ubifs_data_node)
#define UBIFS_DENT_NODE_SZ sizeof(struct ubifs_dent_node)
#define UBIFS_TRUN_NODE_SZ sizeof(struct ubifs_trun_node)
#define UBIFS_PAD_NODE_SZ  sizeof(struct ubifs_pad_node)
#define UBIFS_SB_NODE_SZ   sizeof(struct ubifs_sb_node)
#define UBIFS_MST_NODE_SZ  sizeof(struct ubifs_mst_node)
#define UBIFS_REF_NODE_SZ  sizeof(struct ubifs_ref_node)
#define UBIFS_IDX_NODE_SZ  sizeof(struct ubifs_idx_node)
#define UBIFS_CS_NODE_SZ   sizeof(struct ubifs_cs_node)
#define UBIFS_ORPH_NODE_SZ sizeof(struct ubifs_orph_node)
/* Extended attribute entry nodes are identical to directory entry nodes */
#define UBIFS_XENT_NODE_SZ UBIFS_DENT_NODE_SZ
/* Only this does not have to be multiple of 8 bytes */
#define UBIFS_BRANCH_SZ    sizeof(struct ubifs_branch)

/* Maximum node sizes (N.B. these are guaranteed to be multiples of 8) */
#define UBIFS_MAX_DATA_NODE_SZ  (UBIFS_DATA_NODE_SZ + UBIFS_BLOCK_SIZE)
#define UBIFS_MAX_INO_NODE_SZ   (UBIFS_INO_NODE_SZ + UBIFS_MAX_INO_DATA)
#define UBIFS_MAX_DENT_NODE_SZ  (UBIFS_DENT_NODE_SZ + UBIFS_MAX_NLEN + 1)
#define UBIFS_MAX_XENT_NODE_SZ  UBIFS_MAX_DENT_NODE_SZ

/* The largest UBIFS node */
#define UBIFS_MAX_NODE_SZ UBIFS_MAX_INO_NODE_SZ

/*
 * xattr name of UBIFS encryption context, we don't use a prefix
 * nor a long name to not waste space on the flash.
 */
#define UBIFS_XATTR_NAME_ENCRYPTION_CONTEXT "c"


enum {
	UBIFS_COMPR_FL     = 0x01,
	UBIFS_SYNC_FL      = 0x02,
	UBIFS_IMMUTABLE_FL = 0x04,
	UBIFS_APPEND_FL    = 0x08,
	UBIFS_DIRSYNC_FL   = 0x10,
	UBIFS_XATTR_FL     = 0x20,
	UBIFS_CRYPT_FL     = 0x40,
};


#define UBIFS_FL_MASK 0x0000001F /* Inode flag bits used by UBIFS */

enum {
	UBIFS_COMPR_NONE,
	UBIFS_COMPR_LZO,
	UBIFS_COMPR_ZLIB,
	UBIFS_COMPR_TYPES_CNT,
};

/*
 * UBIFS node types.
 *
 * UBIFS_INO_NODE: inode node
 * UBIFS_DATA_NODE: data node
 * UBIFS_DENT_NODE: directory entry node
 * UBIFS_XENT_NODE: extended attribute node
 * UBIFS_TRUN_NODE: truncation node
 * UBIFS_PAD_NODE: padding node
 * UBIFS_SB_NODE: superblock node
 * UBIFS_MST_NODE: master node
 * UBIFS_REF_NODE: LEB reference node
 * UBIFS_IDX_NODE: index node
 * UBIFS_CS_NODE: commit start node
 * UBIFS_ORPH_NODE: orphan node
 * UBIFS_NODE_TYPES_CNT: count of supported node types
 *
 */
enum {
	UBIFS_INO_NODE,
	UBIFS_DATA_NODE,
	UBIFS_DENT_NODE,
	UBIFS_XENT_NODE,
	UBIFS_TRUN_NODE,
	UBIFS_PAD_NODE,
	UBIFS_SB_NODE,
	UBIFS_MST_NODE,
	UBIFS_REF_NODE,
	UBIFS_IDX_NODE,
	UBIFS_CS_NODE,
	UBIFS_ORPH_NODE,
	UBIFS_NODE_TYPES_CNT,
};

/*
 * Master node flags.
 *
 * UBIFS_MST_DIRTY: rebooted uncleanly - master node is dirty
 * UBIFS_MST_NO_ORPHS: no orphan inodes present
 * UBIFS_MST_RCVRY: written by recovery
 */
enum {
	UBIFS_MST_DIRTY = 1,
	UBIFS_MST_NO_ORPHS = 2,
	UBIFS_MST_RCVRY = 4,
};

/*
 * Node group type (used by recovery to recover whole group or none).
 *
 * UBIFS_NO_NODE_GROUP: this node is not part of a group
 * UBIFS_IN_NODE_GROUP: this node is a part of a group
 * UBIFS_LAST_OF_NODE_GROUP: this node is the last in a group
 */
enum {
	UBIFS_NO_NODE_GROUP = 0,
	UBIFS_IN_NODE_GROUP,
	UBIFS_LAST_OF_NODE_GROUP,
};

/*
 * Superblock flags.
 *
 * UBIFS_FLG_BIGLPT: if "big" LPT model is used if set
 * UBIFS_FLG_SPACE_FIXUP: first-mount "fixup" of free space within LEBs needed
 * UBIFS_FLG_DOUBLE_HASH: store a 32bit cookie in directory entry nodes to
 *			  support 64bit cookies for lookups by hash
 * UBIFS_FLG_ENCRYPTION: this filesystem contains encrypted files
 */
enum {
	UBIFS_FLG_BIGLPT = 0x02,
	UBIFS_FLG_SPACE_FIXUP = 0x04,
	UBIFS_FLG_DOUBLE_HASH = 0x08,
	UBIFS_FLG_ENCRYPTION = 0x10,
};

#define UBIFS_FLG_MASK (UBIFS_FLG_BIGLPT|UBIFS_FLG_SPACE_FIXUP|UBIFS_FLG_DOUBLE_HASH|UBIFS_FLG_ENCRYPTION)

/**
 * struct ubifs_ch - common header node.
 * @magic: UBIFS node magic number (%UBIFS_NODE_MAGIC)
 * @crc: CRC-32 checksum of the node header
 * @sqnum: sequence number
 * @len: full node length
 * @node_type: node type
 * @group_type: node group type
 * @padding: reserved for future, zeroes
 *
 * Every UBIFS node starts with this common part. If the node has a key, the
 * key always goes next.
 */
struct ubifs_ch{
	uint32_t magic;
	uint32_t crc;
	uint64_t sqnum;
	uint32_t len;
	uint8_t node_type;
	uint8_t group_type;
	uint8_t padding[2];
} __attribute__((packed));

/**
 * union ubifs_dev_desc - device node descriptor.
 * @new: new type device descriptor
 * @huge: huge type device descriptor
 *
 * This data structure describes major/minor numbers of a device node. In an
 * inode is a device node then its data contains an object of this type. UBIFS
 * uses standard Linux "new" and "huge" device node encodings.
 */


/*union ubifs_dev_desc {
	uint32_t new;
	uint64_t huge;
} __packed;*/

/**
 * struct ubifs_ino_node - inode node.
 * @ch: common header
 * @key: node key
 * @creat_sqnum: sequence number at time of creation
 * @size: inode size in bytes (amount of uncompressed data)
 * @atime_sec: access time seconds
 * @ctime_sec: creation time seconds
 * @mtime_sec: modification time seconds
 * @atime_nsec: access time nanoseconds
 * @ctime_nsec: creation time nanoseconds
 * @mtime_nsec: modification time nanoseconds
 * @nlink: number of hard links
 * @uid: owner ID
 * @gid: group ID
 * @mode: access flags
 * @flags: per-inode flags (%UBIFS_COMPR_FL, %UBIFS_SYNC_FL, etc)
 * @data_len: inode data length
 * @xattr_cnt: count of extended attributes this inode has
 * @xattr_size: summarized size of all extended attributes in bytes
 * @padding1: reserved for future, zeroes
 * @xattr_names: sum of lengths of all extended attribute names belonging to
 *               this inode
 * @compr_type: compression type used for this inode
 * @padding2: reserved for future, zeroes
 * @data: data attached to the inode
 *
 * Note, even though inode compression type is defined by @compr_type, some
 * nodes of this inode may be compressed with different compressor - this
 * happens if compression type is changed while the inode already has data
 * nodes. But @compr_type will be use for further writes to the inode.
 *
 * Note, do not forget to amend 'zero_ino_node_unused()' function when changing
 * the padding fields.
 */
typedef struct {
	struct ubifs_ch ch;
	uint8_t key[UBIFS_MAX_KEY_LEN];
	uint64_t creat_sqnum;
	uint64_t size;
	uint64_t atime_sec;
	uint64_t ctime_sec;
	uint64_t mtime_sec;
	uint32_t atime_nsec;
	uint32_t ctime_nsec;
	uint32_t mtime_nsec;
	uint32_t nlink;
	uint32_t uid;
	uint32_t gid;
	uint32_t mode;
	uint32_t flags;
	uint32_t data_len;
	uint32_t xattr_cnt;
	uint32_t xattr_size;
	uint8_t padding1[4]; /* Watch 'zero_ino_node_unused()' if changing! */
	uint32_t xattr_names;
	uint16_t compr_type;
	uint8_t padding2[26]; /* Watch 'zero_ino_node_unused()' if changing! */
	uint8_t data[];
} __attribute__((packed)) ubifs_ino_node;

/**
 * struct ubifs_dent_node - directory entry node.
 * @ch: common header
 * @key: node key
 * @inum: target inode number
 * @padding1: reserved for future, zeroes
 * @type: type of the target inode (%UBIFS_ITYPE_REG, %UBIFS_ITYPE_DIR, etc)
 * @nlen: name length
 * @cookie: A 32bits random number, used to construct a 64bits
 *          identifier.
 * @name: zero-terminated name
 *
 * Note, do not forget to amend 'zero_dent_node_unused()' function when
 * changing the padding fields.
 */
typedef struct {
	struct ubifs_ch ch;
	uint8_t key[UBIFS_MAX_KEY_LEN];
	uint64_t inum;
	uint8_t padding1;
	uint8_t type;
	uint16_t nlen;
	uint32_t cookie;
	uint8_t name[];
} __attribute__((packed)) ubifs_dent_node;

/**
 * struct ubifs_data_node - data node.
 * @ch: common header
 * @key: node key
 * @size: uncompressed data size in bytes
 * @compr_type: compression type (%UBIFS_COMPR_NONE, %UBIFS_COMPR_LZO, etc)
 * @compr_size: compressed data size in bytes, only valid when data is encrypted
 * @data: data
 *
 */
typedef struct {
	struct ubifs_ch ch;
	uint8_t key[UBIFS_MAX_KEY_LEN];
	uint32_t size;
	uint16_t compr_type;
	uint16_t compr_size;
	uint8_t data[];
} __attribute__((packed)) ubifs_data_node;

/**
 * struct ubifs_trun_node - truncation node.
 * @ch: common header
 * @inum: truncated inode number
 * @padding: reserved for future, zeroes
 * @old_size: size before truncation
 * @new_size: size after truncation
 *
 * This node exists only in the journal and never goes to the main area. Note,
 * do not forget to amend 'zero_trun_node_unused()' function when changing the
 * padding fields.
 */
typedef struct {
	struct ubifs_ch ch;
	uint32_t inum;
	uint8_t padding[12]; /* Watch 'zero_trun_node_unused()' if changing! */
	uint64_t old_size;
	uint64_t new_size;
} __attribute__((packed)) ubifs_trun_node;

/**
 * struct ubifs_pad_node - padding node.
 * @ch: common header
 * @pad_len: how many bytes after this node are unused (because padded)
 * @padding: reserved for future, zeroes
 */
typedef struct {
	struct ubifs_ch ch;
	uint32_t pad_len;
} __attribute__((packed)) ubifs_pad_node;

/**
 * struct ubifs_sb_node - superblock node.
 * @ch: common header
 * @padding: reserved for future, zeroes
 * @key_hash: type of hash function used in keys
 * @key_fmt: format of the key
 * @flags: file-system flags (%UBIFS_FLG_BIGLPT, etc)
 * @min_io_size: minimal input/output unit size
 * @leb_size: logical eraseblock size in bytes
 * @leb_cnt: count of LEBs used by file-system
 * @max_leb_cnt: maximum count of LEBs used by file-system
 * @max_bud_bytes: maximum amount of data stored in buds
 * @log_lebs: log size in logical eraseblocks
 * @lpt_lebs: number of LEBs used for lprops table
 * @orph_lebs: number of LEBs used for recording orphans
 * @jhead_cnt: count of journal heads
 * @fanout: tree fanout (max. number of links per indexing node)
 * @lsave_cnt: number of LEB numbers in LPT's save table
 * @fmt_version: UBIFS on-flash format version
 * @default_compr: default compression algorithm (%UBIFS_COMPR_LZO, etc)
 * @padding1: reserved for future, zeroes
 * @rp_uid: reserve pool UID
 * @rp_gid: reserve pool GID
 * @rp_size: size of the reserved pool in bytes
 * @padding2: reserved for future, zeroes
 * @time_gran: time granularity in nanoseconds
 * @uuid: UUID generated when the file system image was created
 * @ro_compat_version: UBIFS R/O compatibility version
 */
typedef struct {
	struct ubifs_ch ch;
	uint8_t padding[2];
	uint8_t key_hash;
	uint8_t key_fmt;
	uint32_t flags;
	uint32_t min_io_size;
	uint32_t leb_size;
	uint32_t leb_cnt;
	uint32_t max_leb_cnt;
	uint64_t max_bud_bytes;
	uint32_t log_lebs;
	uint32_t lpt_lebs;
	uint32_t orph_lebs;
	uint32_t jhead_cnt;
	uint32_t fanout;
	uint32_t lsave_cnt;
	uint32_t fmt_version;
	uint16_t default_compr;
	uint8_t padding1[2];
	uint32_t rp_uid;
	uint32_t rp_gid;
	uint64_t rp_size;
	uint32_t time_gran;
	uint8_t uuid[16];
	uint32_t ro_compat_version;
	uint8_t padding2[3968];
} __attribute__((packed)) ubifs_sb_node;

/**
 * struct ubifs_mst_node - master node.
 * @ch: common header
 * @highest_inum: highest inode number in the committed index
 * @cmt_no: commit number
 * @flags: various flags (%UBIFS_MST_DIRTY, etc)
 * @log_lnum: start of the log
 * @root_lnum: LEB number of the root indexing node
 * @root_offs: offset within @root_lnum
 * @root_len: root indexing node length
 * @gc_lnum: LEB reserved for garbage collection (%-1 value means the LEB was
 * not reserved and should be reserved on mount)
 * @ihead_lnum: LEB number of index head
 * @ihead_offs: offset of index head
 * @index_size: size of index on flash
 * @total_free: total free space in bytes
 * @total_dirty: total dirty space in bytes
 * @total_used: total used space in bytes (includes only data LEBs)
 * @total_dead: total dead space in bytes (includes only data LEBs)
 * @total_dark: total dark space in bytes (includes only data LEBs)
 * @lpt_lnum: LEB number of LPT root nnode
 * @lpt_offs: offset of LPT root nnode
 * @nhead_lnum: LEB number of LPT head
 * @nhead_offs: offset of LPT head
 * @ltab_lnum: LEB number of LPT's own lprops table
 * @ltab_offs: offset of LPT's own lprops table
 * @lsave_lnum: LEB number of LPT's save table (big model only)
 * @lsave_offs: offset of LPT's save table (big model only)
 * @lscan_lnum: LEB number of last LPT scan
 * @empty_lebs: number of empty logical eraseblocks
 * @idx_lebs: number of indexing logical eraseblocks
 * @leb_cnt: count of LEBs used by file-system
 * @padding: reserved for future, zeroes
 */
typedef struct {
	struct ubifs_ch ch;
	uint64_t highest_inum;
	uint64_t cmt_no;
	uint32_t flags;
	uint32_t log_lnum;
	uint32_t root_lnum;
	uint32_t root_offs;
	uint32_t root_len;
	uint32_t gc_lnum;
	uint32_t ihead_lnum;
	uint32_t ihead_offs;
	uint64_t index_size;
	uint64_t total_free;
	uint64_t total_dirty;
	uint64_t total_used;
	uint64_t total_dead;
	uint64_t total_dark;
	uint32_t lpt_lnum;
	uint32_t lpt_offs;
	uint32_t nhead_lnum;
	uint32_t nhead_offs;
	uint32_t ltab_lnum;
	uint32_t ltab_offs;
	uint32_t lsave_lnum;
	uint32_t lsave_offs;
	uint32_t lscan_lnum;
	uint32_t empty_lebs;
	uint32_t idx_lebs;
	uint32_t leb_cnt;
	uint8_t padding[344];
} __attribute__((packed)) ubifs_mst_node;

/**
 * struct ubifs_ref_node - logical eraseblock reference node.
 * @ch: common header
 * @lnum: the referred logical eraseblock number
 * @offs: start offset in the referred LEB
 * @jhead: journal head number
 * @padding: reserved for future, zeroes
 */
typedef struct {
	struct ubifs_ch ch;
	uint32_t lnum;
	uint32_t offs;
	uint32_t jhead;
	uint8_t padding[28];
} __attribute__((packed)) ubifs_ref_node;

/**
 * struct ubifs_branch - key/reference/length branch
 * @lnum: LEB number of the target node
 * @offs: offset within @lnum
 * @len: target node length
 * @key: key
 */
typedef struct {
	uint32_t lnum;
	uint32_t offs;
	uint32_t len;
	uint8_t key[];
} __attribute__((packed)) ubifs_branch;

/**
 * struct ubifs_idx_node - indexing node.
 * @ch: common header
 * @child_cnt: number of child index nodes
 * @level: tree level
 * @branches: LEB number / offset / length / key branches
 */
typedef struct {
	struct ubifs_ch ch;
	uint16_t child_cnt;
	uint16_t level;
	uint8_t branches[];
} __attribute__((packed)) ubifs_idx_node;

/**
 * struct ubifs_cs_node - commit start node.
 * @ch: common header
 * @cmt_no: commit number
 */
typedef struct {
	struct ubifs_ch ch;
	uint64_t cmt_no;
} __attribute__((packed)) ubifs_cs_node;

/**
 * struct ubifs_orph_node - orphan node.
 * @ch: common header
 * @cmt_no: commit number (also top bit is set on the last node of the commit)
 * @inos: inode numbers of orphans
 */
typedef struct {
	struct ubifs_ch ch;
	uint64_t cmt_no;
	uint64_t inos[];
} __attribute__((packed)) ubifs_orph_node;

#endif
