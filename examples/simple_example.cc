#include <cstdio>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()
#include <string>

#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include "rocksdb/slice.h"

using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::PinnableSlice;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteBatch;
using ROCKSDB_NAMESPACE::WriteOptions;

std::string kDBPath = "/users/jcarin/rocksdb-database";
int num_entries = 1000000;


int main(int argc, char* argv[]) {
  if (argc > 1)
    num_entries = std::atoi(argv[1]);

  srand(time(0));

  DB* db;
  Options options;
  options.IncreaseParallelism();
  options.OptimizeLevelStyleCompaction();
  options.create_if_missing = true;

  options.write_buffer_size = 4 * 1024 * 1024;
  options.max_write_buffer_number = 2;
  options.target_file_size_base = 2 * 1024 * 1024;

  Status s = DB::Open(options, kDBPath, &db);
  assert(s.ok());

  for (int i = 0; i < num_entries; i++) {
    int random_number = rand();  // Generate a random number
    std::string key = "key" + std::to_string(random_number);  // Generate a key
    std::string value = "value" + std::to_string(i);  // Generate a value

    s = db->Put(WriteOptions(), key, value);
    assert(s.ok());
  }

  return 0;
}
