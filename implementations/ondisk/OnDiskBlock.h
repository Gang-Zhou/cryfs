#pragma once
#ifndef BLOCKSTORE_IMPLEMENTATIONS_ONDISK_ONDISKBLOCK_H_
#define BLOCKSTORE_IMPLEMENTATIONS_ONDISK_ONDISKBLOCK_H_

#include <boost/filesystem/path.hpp>
#include "../../interface/Block.h"
#include <messmer/cpp-utils/data/Data.h>
#include <iostream>

#include <messmer/cpp-utils/pointer/unique_ref.h>
#include <mutex>

namespace blockstore {
namespace ondisk {
class OnDiskBlockStore;

class OnDiskBlock: public Block {
public:
  OnDiskBlock(const Key &key, const boost::filesystem::path &filepath, cpputils::Data data);
  virtual ~OnDiskBlock();

  static boost::optional<cpputils::unique_ref<OnDiskBlock>> LoadFromDisk(const boost::filesystem::path &rootdir, const Key &key);
  static boost::optional<cpputils::unique_ref<OnDiskBlock>> CreateOnDisk(const boost::filesystem::path &rootdir, const Key &key, cpputils::Data data);
  static void RemoveFromDisk(const boost::filesystem::path &rootdir, const Key &key);

  const void *data() const override;
  void write(const void *source, uint64_t offset, uint64_t size) override;

  void flush() override;

  size_t size() const override;

private:
  const boost::filesystem::path _filepath;
  cpputils::Data _data;
  bool _dataChanged;

  void _fillDataWithZeroes();
  void _storeToDisk() const;

  std::mutex _mutex;

  DISALLOW_COPY_AND_ASSIGN(OnDiskBlock);
};

}
}

#endif