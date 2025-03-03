/*
 * Copyright (c) 2019-2023, NVIDIA CORPORATION.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <wholememory/wholememory.h>

#include "communicator.hpp"
#include "file_io.h"
#include "initialize.hpp"
#include "memory_handle.hpp"
#include "parallel_utils.hpp"

#ifdef __cplusplus
extern "C" {
#endif

wholememory_error_code_t wholememory_init(unsigned int flags) { return wholememory::init(flags); }

wholememory_error_code_t wholememory_finalize() { return wholememory::finalize(); }

wholememory_error_code_t wholememory_create_unique_id(wholememory_unique_id_t* unique_id)
{
  return wholememory::create_unique_id(unique_id);
}

wholememory_error_code_t wholememory_create_communicator(wholememory_comm_t* comm,
                                                         wholememory_unique_id_t unique_id,
                                                         int rank,
                                                         int size)
{
  return wholememory::create_communicator(comm, unique_id, rank, size);
}

wholememory_error_code_t wholememory_destroy_communicator(wholememory_comm_t comm)
{
  return wholememory::destroy_communicator(comm);
}

wholememory_error_code_t wholememory_communicator_get_rank(int* rank, wholememory_comm_t comm)
{
  return wholememory::communicator_get_rank(rank, comm);
}

wholememory_error_code_t wholememory_communicator_get_size(int* size, wholememory_comm_t comm)
{
  return wholememory::communicator_get_size(size, comm);
}

wholememory_error_code_t wholememory_communicator_barrier(wholememory_comm_t comm)
{
  wholememory::communicator_barrier(comm);
  return WHOLEMEMORY_SUCCESS;
}

wholememory_error_code_t wholememory_malloc(wholememory_handle_t* wholememory_handle_ptr,
                                            size_t total_size,
                                            wholememory_comm_t comm,
                                            wholememory_memory_type_t memory_type,
                                            wholememory_memory_location_t memory_location,
                                            size_t data_granularity)
{
  return wholememory::create_wholememory(
    wholememory_handle_ptr, total_size, comm, memory_type, memory_location, data_granularity);
}

wholememory_error_code_t wholememory_free(wholememory_handle_t wholememory_handle)
{
  return wholememory::destroy_wholememory(wholememory_handle);
}

wholememory_error_code_t wholememory_get_communicator(wholememory_comm_t* comm,
                                                      wholememory_handle_t wholememory_handle)
{
  return wholememory::get_communicator_from_handle(comm, wholememory_handle);
}

wholememory_memory_type_t wholememory_get_memory_type(wholememory_handle_t wholememory_handle)
{
  return wholememory::get_memory_type(wholememory_handle);
}

wholememory_memory_location_t wholememory_get_memory_location(
  wholememory_handle_t wholememory_handle)
{
  return wholememory::get_memory_location(wholememory_handle);
}

size_t wholememory_get_total_size(wholememory_handle_t wholememory_handle)
{
  return wholememory::get_total_size(wholememory_handle);
}

size_t wholememory_get_data_granularity(wholememory_handle_t wholememory_handle)
{
  return wholememory::get_data_granularity(wholememory_handle);
}

wholememory_error_code_t wholememory_get_local_memory(void** local_ptr,
                                                      size_t* local_size,
                                                      size_t* local_offset,
                                                      wholememory_handle_t wholememory_handle)
{
  return wholememory::get_local_memory_from_handle(
    local_ptr, local_size, local_offset, wholememory_handle);
}

wholememory_error_code_t wholememory_get_rank_memory(void** rank_memory_ptr,
                                                     size_t* rank_memory_size,
                                                     size_t* rank_memory_offset,
                                                     int rank,
                                                     wholememory_handle_t wholememory_handle)
{
  return wholememory::get_rank_memory_from_handle(
    rank_memory_ptr, rank_memory_size, rank_memory_offset, rank, wholememory_handle);
}

wholememory_error_code_t wholememory_get_global_pointer(void** global_ptr,
                                                        wholememory_handle_t wholememory_handle)
{
  return wholememory::get_global_pointer_from_handle(global_ptr, wholememory_handle);
}

wholememory_error_code_t wholememory_get_global_reference(wholememory_gref_t* wholememory_gref,
                                                          wholememory_handle_t wholememory_handle)
{
  return wholememory::get_global_reference_from_handle(wholememory_gref, wholememory_handle);
}

wholememory_error_code_t wholememory_determine_partition_plan(size_t* size_per_rank,
                                                              size_t total_size,
                                                              size_t data_granularity,
                                                              int world_size)
{
  return wholememory::determine_partition_plan(
    size_per_rank, total_size, data_granularity, world_size);
}

wholememory_error_code_t wholememory_determine_entry_partition_plan(size_t* entry_per_rank,
                                                                    size_t total_entry_count,
                                                                    int world_size)
{
  if (entry_per_rank == nullptr) { return WHOLEMEMORY_INVALID_INPUT; }
  *entry_per_rank = wholememory::determine_entry_partition_plan(total_entry_count, world_size);
  return WHOLEMEMORY_SUCCESS;
}

wholememory_error_code_t wholememory_get_partition_plan(size_t* size_per_rank,
                                                        wholememory_handle_t wholememory_handle)
{
  return wholememory::get_partition_plan_from_handle(size_per_rank, wholememory_handle);
}

int fork_get_device_count()
{
  try {
    return ForkGetDeviceCount();
  } catch (...) {
    WHOLEMEMORY_ERROR("fork_get_device_count failed.");
    return -1;
  }
}

wholememory_error_code_t wholememory_load_from_file(wholememory_handle_t wholememory_handle,
                                                    size_t memory_offset,
                                                    size_t memory_entry_size,
                                                    size_t file_entry_size,
                                                    const char** file_names,
                                                    int file_count)
{
  return wholememory::load_file_to_handle(
    wholememory_handle, memory_offset, memory_entry_size, file_entry_size, file_names, file_count);
}

wholememory_error_code_t wholememory_store_to_file(wholememory_handle_t wholememory_handle,
                                                   size_t memory_offset,
                                                   size_t memory_entry_stride,
                                                   size_t file_entry_size,
                                                   const char* local_file_name)
{
  return wholememory::store_handle_to_file(
    wholememory_handle, memory_offset, memory_entry_stride, file_entry_size, local_file_name);
}

wholememory_error_code_t wholememory_load_hdfs_support() { return WHOLEMEMORY_NOT_IMPLEMENTED; }

wholememory_error_code_t wholememory_load_from_hdfs_file(wholememory_handle_t wholememory_handle,
                                                         size_t memory_offset,
                                                         size_t memory_entry_size,
                                                         size_t file_entry_size,
                                                         const char* hdfs_host,
                                                         int hdfs_port,
                                                         const char* hdfs_user,
                                                         const char* hdfs_path,
                                                         const char* hdfs_prefix)
{
  return WHOLEMEMORY_NOT_IMPLEMENTED;
}

#ifdef __cplusplus
}
#endif
