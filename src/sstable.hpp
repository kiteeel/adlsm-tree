#ifndef ADL_LSM_TREE_SSTABLE_H__
#define ADL_LSM_TREE_SSTABLE_H__

#include <openssl/sha.h>
#include <string>
#include "block.hpp"
#include "filter_block.hpp"
#include "footer_block.hpp"
#include "rc.hpp"

using namespace std;

namespace adl {

class WritAbleFile;
class MmapReadAbleFile;
struct DBOptions;

/* Sorted Strings Table */
class SSTableWriter {
 public:
  SSTableWriter(string_view dbname, WritAbleFile *file,
                const DBOptions &options);
  /* Build */
  RC Add(string_view key, string_view value);
  RC Final(unsigned char sha256_digit[]);

 private:
  RC FlushDataBlock();

  static constexpr unsigned int need_flush_size_ = (1UL << 12); /* 4KB */
  string dbname_;
  WritAbleFile *file_;

  /* 数据块 */
  BlockWriter data_block_;
  BlockHandle data_block_handle_;

  /* 索引块 */
  BlockWriter index_block_;
  BlockHandle index_block_handle_;

  /* 过滤器块 */
  FilterBlockWriter filter_block_;
  BlockHandle filter_block_handle_;

  /* 元数据块 */
  BlockWriter meta_data_block_;
  BlockHandle meta_data_block_handle_;

  /* 尾信息块 */
  FooterBlockWriter foot_block_;
  // BlockHandle foot_block_handle_;

  SHA256_CTX sha256_;
  string buffer_;   /* 数据 */
  int offset_;      /* 数据的偏移量 */
  string last_key_; /* 最后一次 add 的 key */
};

class DB;

class SSTableReader {
 public:
  static RC Open(MmapReadAbleFile *file, SSTableReader **table,
                 const string &oid, DB *db = nullptr);
  RC Get(string_view key, string &value);
  string GetFileName();

  SSTableReader(MmapReadAbleFile *file, const string &oid, DB *db = nullptr);
  ~SSTableReader();
  SSTableReader &operator=(const SSTableReader &) = delete;
  SSTableReader(const SSTableReader &) = delete;

 private:
  RC ReadFooterBlock();
  RC ReadMetaBlock();
  RC ReadIndexBlock();
  RC ReadFilterBlock();
  // RC ReadDataBlock();

  MmapReadAbleFile *file_;
  size_t file_size_;
  string oid_;

  FilterBlockReader filter_block_reader_;
  BlockReader index_block_reader_;
  BlockReader meta_block_reader_;
  FooterBlockReader foot_block_reader_;

  DB *db_;
};

}  // namespace adl

#endif  // ADL_LSM_TREE_SSTABLE_H__